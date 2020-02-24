/**
 * @file MCMC.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 10/05/2013
 * @section LICENSE
 *
 * Copyright NIWA Science ©2013 - www.niwa.co.nz
 *
 */

// headers
#include "MCMC.h"

#include "../ConfigurationLoader/MPD.h"
#include "../Estimates/Manager.h"
#include "../EstimateTransformations/Manager.h"
#include "../GlobalConfiguration/GlobalConfiguration.h"
#include "../Model/Managers.h"
#include "../Model/Model.h"
#include "../Reports/Manager.h"
#include "../Reports/Common/MCMCObjective.h"
#include "../Reports/Common/MCMCSample.h"


// namespaces
namespace niwa {

/**
 * Constructor
 */
MCMC::MCMC(shared_ptr<Model> model) : model_(model) {
  parameters_.Bind<string>(PARAM_LABEL, &label_, "The label of the MCMC", "");
  parameters_.Bind<string>(PARAM_TYPE, &type_, "The type of MCMC", "", "");
  parameters_.Bind<unsigned>(PARAM_LENGTH, &length_, "The number of iterations in for the MCMC chain", "");
  parameters_.Bind<bool>(PARAM_ACTIVE, &active_, "Indicates if this is the active MCMC algorithm", "", true);
  parameters_.Bind<bool>(PARAM_PRINT_DEFAULT_REPORTS, &print_default_reports_, "Indicates if the output prints the default reports", "", true);
  parameters_.Bind<Double>(PARAM_STEP_SIZE, &step_size_, "Initial stepsize (as a multiplier of the approximate covariance matrix)", "", 0.02);
  parameters_.Bind<Double>(PARAM_MAX_CORRELATION, &max_correlation_, "Maximum absolute correlation in the covariance matrix of the proposal distribution", "", 0.8);
  parameters_.Bind<string>(PARAM_COVARIANCE_ADJUSTMENT_METHOD, &correlation_method_, "Method for adjusting small variances in the covariance proposal matrix"
      , "", PARAM_CORRELATION)->set_allowed_values({PARAM_COVARIANCE, PARAM_CORRELATION,PARAM_NONE});
  parameters_.Bind<Double>(PARAM_CORRELATION_ADJUSTMENT_DIFF, &correlation_diff_, "Minimum non-zero variance times the range of the bounds in the covariance matrix of the proposal distribution", "", 0.0001);

}

/**
 * Validate the parameters defined in the configuration file
 */
void MCMC::Validate() {
  parameters_.Populate(model_);

  if (correlation_method_ != PARAM_CORRELATION && correlation_method_ != PARAM_COVARIANCE && correlation_method_ != PARAM_NONE)
    LOG_ERROR_P(PARAM_COVARIANCE_ADJUSTMENT_METHOD) << "(" << correlation_method_ << ")"
        << " is not supported. Currently supported values are " << PARAM_CORRELATION << ", " << PARAM_COVARIANCE << " and " << PARAM_NONE;
  if (max_correlation_ <= 0.0 || max_correlation_ > 1.0)
    LOG_ERROR_P(PARAM_MAX_CORRELATION) << "(" << max_correlation_ << ") must be between 0.0 (not inclusive) and 1.0 (inclusive)";

  DoValidate();
}

/**
 * Build any relationships we need and objects that will hold data
 * so they can be used during the execution
 */

void MCMC::Build() {
  LOG_TRACE();
  /**
   * We will create the default reports if necessary.
   * But we'll use append if we're resuming.
   *
   * NOTE: Since we're adding reports to the report manager
   * and it's running in a different thread we need to pause
   * and resume the manager thread or we'll get weird crashes.
   */
  if (!model_->global_configuration().resume() && print_default_reports_) {
    model_->managers()->report()->Pause();

    if (model_->managers()->report()->HasType(PARAM_MCMC_OBJECTIVE)) {
//      reports::MCMCObjective* objective_report = new reports::MCMCObjective(model_);
//      objective_report->set_block_type(PARAM_REPORT);
//      objective_report->set_defined_file_name(__FILE__);
//      objective_report->set_defined_line_number(__LINE__);
//      objective_report->parameters().Add(PARAM_LABEL, "mcmc_objective", __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_TYPE, PARAM_MCMC_OBJECTIVE, __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_FILE_NAME, "mcmc_objectives.out", __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_WRITE_MODE, PARAM_INCREMENTAL_SUFFIX, __FILE__, __LINE__);
//      objective_report->Validate();
//      model_->managers()->report()->AddObject(objective_report);
    }

    if (model_->managers()->report()->HasType(PARAM_MCMC_SAMPLE)) {
//      reports::MCMCSample* sample_report = new reports::MCMCSample(model_);
//      sample_report->set_block_type(PARAM_REPORT);
//      sample_report->set_defined_file_name(__FILE__);
//      sample_report->set_defined_line_number(__LINE__);
//      sample_report->parameters().Add(PARAM_LABEL, "mcmc_sample", __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_TYPE, PARAM_MCMC_SAMPLE, __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_FILE_NAME, "mcmc_samples.out", __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_WRITE_MODE, PARAM_INCREMENTAL_SUFFIX, __FILE__, __LINE__);
//      sample_report->Validate();
//      model_->managers()->report()->AddObject(sample_report);
    }

    model_->managers()->report()->Resume();
  } else if (model_->global_configuration().resume() && print_default_reports_) {
    model_->managers()->report()->Pause();

    string objective_name = model_->global_configuration().mcmc_objective_file();
    string sample_name    = model_->global_configuration().mcmc_sample_file();
    LOG_FINE() << "Objective file name: " << objective_name;
    LOG_FINE() << "Sample file name: " << sample_name;

    if (model_->managers()->report()->HasType(PARAM_MCMC_OBJECTIVE)) {
//      reports::MCMCObjective* objective_report = new reports::MCMCObjective(model_);
//      objective_report->set_block_type(PARAM_REPORT);
//      objective_report->set_defined_file_name(__FILE__);
//      objective_report->set_defined_line_number(__LINE__);
//      objective_report->parameters().Add(PARAM_LABEL, "mcmc_objective", __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_TYPE, PARAM_MCMC_OBJECTIVE, __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_FILE_NAME, objective_name, __FILE__, __LINE__);
//      objective_report->parameters().Add(PARAM_WRITE_MODE, PARAM_APPEND, __FILE__, __LINE__);
//      objective_report->Validate();
//      model_->managers()->report()->AddObject(objective_report);
    }

    if (model_->managers()->report()->HasType(PARAM_MCMC_SAMPLE)) {
//      reports::MCMCSample* sample_report = new reports::MCMCSample(model_);
//      sample_report->set_block_type(PARAM_REPORT);
//      sample_report->set_defined_file_name(__FILE__);
//      sample_report->set_defined_line_number(__LINE__);
//      sample_report->parameters().Add(PARAM_LABEL, "mcmc_sample", __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_TYPE, PARAM_MCMC_SAMPLE, __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_FILE_NAME, sample_name, __FILE__, __LINE__);
//      sample_report->parameters().Add(PARAM_WRITE_MODE, PARAM_APPEND, __FILE__, __LINE__);
//      sample_report->Validate();
//      model_->managers()->report()->AddObject(sample_report);
    }

    model_->managers()->report()->Resume();
  }

  DoBuild();
}

/**
 *
 */
void MCMC::Execute() {
	LOG_FINE() << "executing MCMC";
  if (model_->global_configuration().create_mpd_file()) {
    configuration::MPD mpd_loader(model_);
    if (!mpd_loader.LoadFile("mpd.out"))
      LOG_FATAL() << "Failed to load MPD Data from mpd.out file";
  }

  LOG_FINEST() << "Running into MCMC now...";
  DoExecute();
}


/**
 * Get the covariance matrix from the minimiser and then
 * adjust it for our proposal distribution
 */

void MCMC::BuildCovarianceMatrix() {
  LOG_MEDIUM() << "Building covariance matrix";
  // Are we starting at MPD or recalculating the matrix based on an empirical sample
	ublas::matrix<Double> original_correlation;
  if (recalculate_covariance_) {
    LOG_MEDIUM() << "recalculate covariance";
    covariance_matrix_ = covariance_matrix_lt;
  }
  	// Remove for the shared library only used for debugging purposes
  	// Minimiser* minimiser = model_->managers()->minimiser()->active_minimiser();
    // covariance_matrix_ = minimiser->covariance_matrix();
    // original_correlation = minimiser->correlation_matrix();

    // This is already built by MPD.cpp at line 137. in the frontend the minimiser is dropped out before the MCMC state kicks in, so this will return a rubbish covariance matrix

  if (correlation_method_ == PARAM_NONE)
    return;

  /**
   * Adjust the covariance matrix for the proposal distribution
   */
  LOG_MEDIUM() << "printing upper left hand triangle before applying correlation adjustment.";
  for (unsigned i = 0; i < (covariance_matrix_.size1() - 1); ++i) {
    for (unsigned j = i + 1; j < covariance_matrix_.size2(); ++j) {
    	LOG_MEDIUM() << "row = " << i + 1 << " col = " << j + 1 << " value = " << covariance_matrix_(i,j);
    }
  }
  LOG_MEDIUM() << "and we are out of that print out";
  ublas::matrix<Double> original_covariance(covariance_matrix_);

  LOG_MEDIUM() << "Beginning covar adjustment. rows = " << original_covariance.size1() << " cols = " << original_covariance.size2();
  for (unsigned i = 0; i < (covariance_matrix_.size1() - 1); ++i) {
    for (unsigned j = i + 1; j < covariance_matrix_.size2(); ++j) {
      // This is the lower triangle of the covariance matrix
    	Double value = original_covariance(i,j) / sqrt(original_covariance(i,i) * original_covariance(j,j));
    	LOG_MEDIUM() << "row = " << i + 1 << " col = " << j + 1 << " correlation = " << AS_DOUBLE(value);
      if (original_covariance(i,j) / sqrt(original_covariance(i,i) * original_covariance(j,j)) > max_correlation_) {
        covariance_matrix_(i,j) = max_correlation_ * sqrt(original_covariance(i,i) * original_covariance(j,j));
      }
      if (original_covariance(i,j) / sqrt(original_covariance(i,i) * original_covariance(j,j)) < -max_correlation_){
        covariance_matrix_(i,j) = -max_correlation_ * sqrt(original_covariance(i,i) * original_covariance(j,j));
      }
    }
  }
  LOG_MEDIUM() << "printing upper left hand triangle";
  for (unsigned i = 0; i < (covariance_matrix_.size1() - 1); ++i) {
    for (unsigned j = i + 1; j < covariance_matrix_.size2(); ++j) {
    	LOG_MEDIUM() << "row = " << i + 1 << " col = " << j + 1 << " value = " << covariance_matrix_(i,j);
    }
  }

  /**
   * Adjust any non-zero variances less than min_diff_ * difference between bounds
   */
  vector<Double> difference_bounds;
  vector<Estimate*> estimates = model_->managers()->estimate()->GetIsEstimated();
  LOG_MEDIUM() << "upper_bound lower_bound";
  for (Estimate* estimate : estimates) {
    difference_bounds.push_back( estimate->upper_bound() - estimate->lower_bound() );
    LOG_MEDIUM() << estimate->upper_bound() << " " << estimate->lower_bound();
  }

  for (unsigned i = 0; i < covariance_matrix_.size1(); ++i) {
    if (covariance_matrix_(i,i) < (correlation_diff_ * difference_bounds[i]) && covariance_matrix_(i,i) != 0) {
      if (correlation_method_ == PARAM_COVARIANCE) {
        Double multiply_covariance = (sqrt(correlation_diff_) * difference_bounds[i]) / sqrt(covariance_matrix_(i,i));
        LOG_MEDIUM() << "multiplier = " << multiply_covariance << " for parameter = " << i + 1;
        for (unsigned j = 0; j < covariance_matrix_.size1(); ++j) {
          covariance_matrix_(i,j) = covariance_matrix_(i,j) * multiply_covariance;
          covariance_matrix_(j,i) = covariance_matrix_(j,i) * multiply_covariance;
        }
      } else if(correlation_method_ == PARAM_CORRELATION) {
        covariance_matrix_(i,i) = correlation_diff_ * difference_bounds[i];
      }
    }
  }
  LOG_MEDIUM() << "printing upper left hand triangle";
  for (unsigned i = 0; i < (covariance_matrix_.size1() - 1); ++i) {
    for (unsigned j = i + 1; j < covariance_matrix_.size2(); ++j) {
    	LOG_MEDIUM() << "row = " << i + 1 << " col = " << j + 1 << " value = " << covariance_matrix_(i,j);
    }
  }


}

} /* namespace niwa */
