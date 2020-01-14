/**
 * @file Kthohr.cpp
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 14/01/2020
 * @section LICENSE
 *
 * Copyright Scott Rasmussen ©2020 - https://www.zaita.com
 */

// headers
#include "Kthohr.h"

#include <mcmc/mcmc.hpp>

#include "../../Estimates/Estimate.h"
#include "../../Estimates/Manager.h"
#include "../../EstimateTransformations/Manager.h"
#include "../../Model/Model.h"
#include "../../Minimisers/Manager.h"
#include "../../ObjectiveFunction/ObjectiveFunction.h"
#include "../../Reports/Manager.h"
#include "../../Utilities/Math.h"
#include "../../Utilities/RandomNumberGenerator.h"

// namespaces
namespace niwa::mcmcs {

double kthohr_callback(const arma::vec &vals_inp, void *ll_data) {

	shared_ptr<Model> model = *reinterpret_cast< shared_ptr<Model>* >(ll_data);
	cout << model->min_age() << endl;

	// Update our Components with the New Parameters
//	  LOG_FINE() << "model_: " << model_;
//	  vector<Estimate*> estimates = model_->managers()->estimate()->GetIsEstimated();
//
//	  if (Parameters.size() != estimates.size()) {
//	    LOG_CODE_ERROR() << "The number of enabled estimates does not match the number of test solution values";
//	  }
//
//	  for (unsigned i = 0; i < Parameters.size(); ++i)
//	    estimates[i]->set_value(Parameters[i]);
//
//	  model_->managers()->estimate_transformation()->RestoreEstimates();
//	  model_->FullIteration();
//
//	  ObjectiveFunction& objective = model_->objective_function();
//	  objective.CalculateScore();
//
//	  model_->managers()->estimate_transformation()->TransformEstimates();
//	  return objective.score();
	  return 0.0;
}


/**
 * Explicit constructor
 *
 * @param model Shared pointer to model for base thread
 */
Kthohr::Kthohr(shared_ptr<Model> model) : MCMC(model) {

	parameters_.Bind<string>(PARAM_ALGORITHM, &algorithm_, "Kthohr MCMC Algorithm to use", "")
		->set_allowed_values({ PARAM_METROPOLIS_HASTINGS, "HMC"});
	parameters_.Bind<unsigned>(PARAM_KEEP, &keep_, "Keep every nth link in the chain", "", 1);
	parameters_.Bind<unsigned>(PARAM_BURN_IN, &burn_in_, "Number of iterations to burn in for RWMH", "", 0);
}

/**
 * Validate our parameters
 */
void Kthohr::DoValidate() {
  if (length_ <= 0)
    LOG_ERROR_P(PARAM_LENGTH) << "(" << length_ << ") cannot be less than or equal to 0";
}

/**
 * Build our relationships
 */
void Kthohr::DoBuild() {
  LOG_TRACE();

  estimates_ = model_->managers()->estimate()->GetIsEstimated();
  if (estimates_.size() == 0)
    LOG_ERROR() << "While building the MCMC system the number of active estimates was 0. You need at least 1 non-fixed MCMC estimate";
}

/**
 * Run our MCMC
 */
void Kthohr::DoExecute() {
	if (algorithm_ == PARAM_METROPOLIS_HASTINGS)
		RWMH();
	else
		LOG_FATAL() << "MCMC algorithm " << algorithm_ << " is not currently supported";
}


void Kthohr::RWMH() {
	arma::vec initial_vals(estimate_count_);
	arma::vec lower_bounds(estimate_count_);
	arma::vec upper_bounds(estimate_count_);

	for (unsigned i = 0; i < estimate_count_; ++i) {
		initial_vals(i) = estimates_[i]->value();
		lower_bounds(i) = estimates_[i]->lower_bound();
		upper_bounds(i) = estimates_[i]->upper_bound();
	}

	arma::mat chain;
	::mcmc::algo_settings_t settings;
	settings.rwmh_n_burnin = burn_in_;
	settings.rwmh_n_draws = length_;
	settings.vals_bound = true;
	settings.lower_bounds = lower_bounds;
	settings.upper_bounds = upper_bounds;

	BuildCovarianceMatrix();
	arma::mat covariance_matrix(covariance_matrix_.size1(), covariance_matrix_.size1());
	for (int i=0; i < (int)covariance_matrix_.size1(); ++i) {
		for (int j=0 ; j < (int)covariance_matrix_.size1(); ++j) {
			covariance_matrix(i,j) = covariance_matrix_(i,j);
		}
	}
	settings.rwmh_cov_mat = covariance_matrix;
	// settings.rwmh_par_scale - DONT KNOW HOW TO SET THIS

//	bool rwmh(const arma::vec& initial_vals, arma::mat& draws_out, std::function<double (const arma::vec& vals_inp, void* target_data)> target_log_kernel, void* target_data, algo_settings_t& settings);
	::mcmc::rwmh(initial_vals, chain, kthohr_callback, &model_, settings);
//	arma::cube cube_out;
//	mcmc::de(initial_vals, cube_out, kthohr_callback, 0, settings);

	for (unsigned i = 0; i < length_; ++i) {
		if (i % keep_ != 0)
			continue;

		vector<double> values;
		for (unsigned j = 0; j < estimate_count_; ++j)
			values.push_back(chain.row(i)(j));

		mcmc::ChainLink new_link;
		new_link.iteration_                     = i;
		new_link.penalty_                       = 0.0;
		new_link.score_                         = 0.0;
		new_link.prior_                         = 0.0;
		new_link.likelihood_                    = 0.0;
		new_link.additional_priors_             = 0.0;
		new_link.jacobians_                     = 0.0;
		new_link.acceptance_rate_               = 0;
		new_link.acceptance_rate_since_adapt_   = 0;
		new_link.step_size_                     = step_size_;
		new_link.values_                        = values;
		chain_.push_back(new_link);
	}
}

} /* namespace niwa::mcmcs */
