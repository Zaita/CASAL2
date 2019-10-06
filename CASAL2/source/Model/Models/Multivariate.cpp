/**
 * @file Multivariate.cpp
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 20/09/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 */

// headers
#include "Multivariate.h"

#include <Eigen/Eigen>

#include "Estimates/Manager.h"
#include "Model/Managers.h"
#include "MCMCs/Manager.h"
#include "MCMCs/MCMC.h"
#include "ObjectiveFunction/ObjectiveFunction.h"
#include "Penalties/Manager.h"
#include "Reports/Manager.h"
#include "Selectivities/Manager.h"
#include "Utilities/RandomNumberGenerator.h"

// namespaces
namespace niwa {
namespace model {

/**
 *
 */
Multivariate::Multivariate() {
	partition_type_ = PartitionType::kMultivariate;
}

/**
 *
 */
Multivariate::~Multivariate() {
}

/**
 *
 */
bool Multivariate::Start(RunMode::Type run_mode) {
	run_mode_ = run_mode;

	RegisterAsAddressable(PARAM_A, &objective_score_);
	cout << "Validation: Objective Score: " << objective_score_ << endl;

	managers_->mcmc()->Validate(this);
	managers_->report()->Validate();
	managers_->selectivity()->Validate();
	managers_->estimate()->Validate(this);

	managers_->report()->Build();
	managers_->selectivity()->Build();
	managers_->estimate()->Build(this);

	cout << "PiApprox Obj: " << objective_function_ << endl;

	if (run_mode == RunMode::kBasic)
		FullIteration();
	else if (run_mode == RunMode::kMCMC) {
		managers_->mcmc()->Build();

		auto mcmc = managers_->mcmc()->active_mcmc();
		mcmc->Execute();
	} else {
		LOG_CODE_ERROR() << "RunMode not supported";
	}

	objective_function_->CalculateScore();
	managers_->report()->Execute(State::kIterationComplete);
	return true;
}

/**
 *
 */
void Multivariate::DoValidate() { }

//void inverse(double matrix[3][3]) {
//  int n = 3;
//  int i, j, k;
//  double ratio;
//  double a;
//
//  for(i = 0; i < n; i++){
//      for(j = 0; j < n; j++){
//          if(i!=j){
//              ratio = matrix[j][i]/matrix[i][i];
//              for(k = 0; k < 2*n; k++){
//                  matrix[j][k] -= ratio * matrix[i][k];
//              }
//          }
//      }
//  }
//  for(i = 0; i < n; i++){
//      a = matrix[i][i];
//      for(j = 0; j < 2*n; j++){
//          matrix[i][j] /= a;
//      }
//  }
//}
//
//double width = 100.0;
//double height = 100.0;
//double r = 50.0;
//
//void multiply(double matrix[3][3], double vector[3], double ans[3]) {
//			ans[0] = matrix[0][0]*vector[0] + matrix[1][0]*vector[1] + matrix[2][0]*vector[2];
//			ans[1] = matrix[0][1]*vector[0] + matrix[1][1]*vector[1] + matrix[2][1]*vector[2];
//			ans[2] = matrix[0][2]*vector[0] + matrix[1][2]*vector[1] + matrix[2][2]*vector[2];
//}

//#define M_PI 3.14159
//
//class Mvn
//{
//public:
//  Mvn(const Eigen::VectorXd& mu,
//      const Eigen::MatrixXd& s) : mean(mu), sigma(s) { };
//  ~Mvn() { };
//  double pdf(const Eigen::VectorXd& x) const {
//  	double n = x.rows();
//		double sqrt2pi = std::sqrt(2 * M_PI);
//		double quadform  = (x - mean).transpose() * sigma.inverse() * (x - mean);
//		double norm = std::pow(sqrt2pi, - n) *
//									std::pow(sigma.determinant(), - 0.5);
//
//		return norm * exp(-0.5 * quadform);
//  }
//
//  Eigen::VectorXd sample(unsigned int nr_iterations = 20) const;
//  Eigen::VectorXd mean;
//  Eigen::MatrixXd sigma;
//};




void Multivariate::FullIteration() {
	Reset();

	// Define the covariance matrix and the mean
//	Eigen::MatrixXd sigma(2, 2);
//	sigma << 1, 0.1,
//					 0.1, 1;
//	Eigen::VectorXd mean(2);
//	mean << 0, 0;
//	Mvn mvn(mean, sigma);
//
//	Eigen::VectorXd test(2);
//	test << 0, 0;
//	cout << mvn.pdf(test) << endl;

//	double new_x = utilities::RandomNumberGenerator::Instance().uniform(0, 10.0);
//	double new_y = utilities::RandomNumberGenerator::Instance().uniform(0, 10.0);
//	double new_z = utilities::RandomNumberGenerator::Instance().uniform(0, 10.0);
//
//	double xyz[3] = { new_x, new_y, new_z };
//
//	double cov[3][3] = { {1,0.424,0.071}, {0.424,2,0.6}, {0.071,0.6,0.5} };
//	double mean[3] = { 2, 1, 3 };
//
//	// Trying to do components[0] = 0.5 * ((xyz-mean) * (inverse(cov) * (xyz-mean)));
//	inverse(cov);
//
//	// equation now: 0.5 * ((xyz-mean) * (cov * (xyz-mean))
//	double xyz_mean[3] = {0, 0, 0};
//	xyz_mean[0] = xyz[0] - mean[0];
//	xyz_mean[1] = xyz[1] - mean[1];
//	xyz_mean[2] = xyz[2] - mean[2];
//
//	// equation now: 0.5 * (xyz_mean * cov * xyz_mean)
//	double ans[3];
//	multiply(cov, xyz_mean, ans); // ans = xyz_mean
//
//	// equation now: 0.5 * xyz_mean * ans
//	ans[0] = ans[0] * xyz_mean[0];
//	ans[1] = ans[1] * xyz_mean[1];
//	ans[1] = ans[1] * xyz_mean[1];
//
//	// equation now: 0.5 * ans
//	ans[0] *= 0.5;
//	ans[1] *= 0.5;
//	ans[1] *= 0.5;
//
//	// now what?
//	cout << "ans: " << ans[0] << " " << ans[1] << " " << ans[2] << endl;

//	LOG_CODE_ERROR() << "beep";


//	double center_x = width / 2;
//	double center_y = width / 2;

	// Add New random point
//	double new_x = utilities::RandomNumberGenerator::Instance().uniform(0, width);
//	double new_y = utilities::RandomNumberGenerator::Instance().uniform(0, height);
//	double new_x = managers().selectivity()->GetSelectivity("X")->GetAgeResult(5, nullptr);
//	double new_y = managers().selectivity()->GetSelectivity("Y")->GetAgeResult(5, nullptr);

	// Calculate number of points in and out or cicle
//	double distance = sqrt( (new_x - center_x)*(new_x - center_x) + (new_y - center_y)*(new_y - center_y) );
	// Calculate addressable

//	if (distance < r)
//		inside_++;
//	else
//		outside_++;
//
//	double insided = inside_ * 1.0;
//	double outsided = outside_ * 1.0;
//	double ratio = 4 * (insided / (insided + outsided));
//	objective_score_ = ratio;
//
//	managers().penalty()->FlagPenalty("PI_Approximation", objective_score_);
//	cout << "Objective Score: " << objective_score_ << " (" << (inside_ + outside_) << ") - " << "inside_ / outside_: " << inside_ << " : " << outside_ << "; new_x/new_y: " << new_x << "/" << new_y << endl;
}



} /* namespace model */
} /* namespace niwa */
