/**
 * @file EstimateSummary.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 27/02/2013
 * @section LICENSE
 *
 * Copyright NIWA Science ©2013 - www.niwa.co.nz
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */

// Headers
#include "EstimateSummary.h"

#include "../../Estimates/Manager.h"

// Namespaces
namespace niwa {
namespace reports {

/**
 * Default Constructor
 */
EstimateSummary::EstimateSummary() {
  run_mode_    = (RunMode::Type)(RunMode::kBasic | RunMode::kEstimation);
  model_state_ = State::kFinalise;
}

/**
 * This method will execute our estimate summary report
 */
void EstimateSummary::DoExecute(shared_ptr<Model> model) {
  // Print the estimates
  niwa::estimates::Manager& estimate_manager = *model->managers().estimate();
  vector<Estimate*> estimates = estimate_manager.objects();

  cache_ << "*"<< type_ << "[" << label_ << "]" << "\n";
  for (Estimate* estimate : estimates) {
	cache_ << estimate->parameter() << " " << REPORT_R_LIST << "\n";
    cache_ << "lower_bound: " << estimate->lower_bound() << "\n";
    cache_ << "upper_bound: " << estimate->upper_bound() << "\n";
    cache_ << "value: " << AS_DOUBLE(estimate->value()) << "\n";
    map<string, Parameter*> parameters = estimate->parameters().parameters();
    for (auto [label, parameter] : parameters) {
      cache_ << label << ": ";
      for (string parameter_value : parameter->values())
        cache_ << parameter_value << " ";
      cache_ << "\n";
    }
    cache_ << REPORT_R_LIST_END << "\n\n";
  }

  ready_for_writing_ = true;
}

} /* namespace reports */
} /* namespace niwa */
