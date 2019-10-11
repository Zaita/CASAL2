/**
 * @file PartitionMeanWeight.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 7/01/2014
 * @section LICENSE
 *
 * Copyright NIWA Science ©2013 - www.niwa.co.nz
 *
 */

// headers
#include "PartitionMeanWeight.h"

#include "AgeLengths/Manager.h"
#include "Categories/Categories.h"
#include "Model/Model.h"
#include "Partition/Partition.h"
#include "Partition/Accessors/All.h"
#include "LengthWeights/Manager.h"
#include "TimeSteps/Manager.h"

// namespaces
namespace niwa {
namespace reports {
namespace age {

/**
 * default constructor
 */
PartitionMeanWeight::PartitionMeanWeight() {
  run_mode_    = (RunMode::Type)(RunMode::kBasic | RunMode::kProjection | RunMode::kSimulation| RunMode::kEstimation | RunMode::kProfiling);
  model_state_ = State::kExecute;

  parameters_.Bind<string>(PARAM_TIME_STEP, &time_step_, "Time Step label", "", "");
  parameters_.Bind<unsigned>(PARAM_YEARS, &years_, "Years", "", true);
}

/**
 * Build method
 */
void PartitionMeanWeight::DoBuild(shared_ptr<Model> model) {
  if (!parameters_.Get(PARAM_YEARS)->has_been_defined()) {
    years_ = model->years();
  }
}

/**
 * Execute method
 */
void PartitionMeanWeight::DoExecute(shared_ptr<Model> model) {
  LOG_TRACE();
  unsigned time_step_index = model->managers().time_step()->current_time_step();

  niwa::partition::accessors::All all_view(model.get());
  unsigned year = model->current_year();
  if (find(years_.begin(),years_.end(), year) != years_.end()) {
    cache_ << "*"<< type_ << "[" << label_ << "]" << "\n";
    cache_ << "year: " << year << "\n";
    for (auto iterator : all_view) {
      string category = iterator->name_;
      LOG_FINEST() << "printing mean weight for category " << category;
      cache_ << category << " " << REPORT_R_LIST << "\n";

      cache_ << "mean_weights " << REPORT_R_LIST << "\n";
      cache_ << "values: ";

      for (unsigned age = iterator->min_age_; age <= iterator->max_age_; ++age) {
        Double temp = iterator->mean_weight_by_time_step_age_[time_step_index][age];
        cache_ << temp << " ";
      }
      cache_<<"\n";
      LOG_FINEST() << "cached mean weight";
      cache_ << REPORT_R_LIST_END <<"\n";
      cache_ << REPORT_R_LIST_END <<"\n";
    }

    ready_for_writing_ = true;
  }
}

} /* namespace age */
} /* namespace reports */
} /* namespace niwa */
