/**
 * @file Manager.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 14/05/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 */

// headers
#include "Manager.h"

#include <algorithm>
#include <thread>

// namespaces
namespace niwa {
namespace minimisers {

using std::scoped_lock;

std::mutex Manager::lock_;

/**
 * Default constructor
 */
Manager::Manager() {
}

/**
 * Destructor
 */
Manager::~Manager() noexcept(true) {

}

/**
 * Validate any loaded minimisers we have.
 */
void Manager::Validate() {
  LOG_TRACE();
  LOG_CODE_ERROR() << "This method is not supported";
}

/**
 *
 */
void Manager::Validate(shared_ptr<Model> model) {
	std::scoped_lock l(lock_);
	if (model->partition_type() == PartitionType::kPiApprox)
		return;
  if (objects_.size() == 0 || has_validated_)
		return;

  LOG_TRACE();
  for (auto minimiser : objects_)
    minimiser->Validate();

  if (objects_.size() == 1) {
    objects_[0]->set_active(true);
    active_minimiser_ = objects_[0];

  } else {
    unsigned active_count = 0;
    for (auto minimiser : objects_) {
      if (minimiser->active()) {
        active_count++;
        active_minimiser_ = minimiser;
      }
    }

    RunMode::Type must_have_minimiser[] = { RunMode::kProfiling, RunMode::kEstimation, RunMode::kMCMC };
    if (std::find(std::begin(must_have_minimiser), std::end(must_have_minimiser), model->run_mode()) != std::end(must_have_minimiser) && active_count != 1) {
      LOG_FATAL() << "You must have only one active minimiser per run, you have specified " << active_count << " of " << size() << " please fix this.";
    }

    RunMode::Type one_active_only[] = { RunMode::kTesting };
    if (std::find(std::begin(one_active_only), std::end(one_active_only), model->run_mode()) != std::end(one_active_only) && active_count > 1) {
      LOG_FATAL() << "You must have only one active minimiser per run, you have specified " << active_count << " of " << size() << " please fix this.";
    }
  }

  has_validated_ = true;
}

/**
 *
 */
void Manager::Build() {
	LOG_TRACE();

	std::scoped_lock l(lock_);
  if (objects_.size() == 0 || has_built_ || !has_validated_)
		return;

  niwa::base::Manager<minimisers::Manager, niwa::Minimiser>::Build();
	has_built_ = true;
}

} /* namespace minimisers */
} /* namespace niwa */
