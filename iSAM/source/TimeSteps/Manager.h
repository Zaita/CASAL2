/*
 * Manager.h
 *
 *  Created on: 13/12/2012
 *      Author: Admin
 */

#ifndef TIMESTEPS_MANAGER_H_
#define TIMESTEPS_MANAGER_H_

// Headers
#include "BaseClasses/Manager.h"
#include "DerivedQuantities/Manager.h"
#include "TimeSteps/TimeStep.h"

// Namespaces
namespace isam {
namespace timesteps {

/**
 * Class Definition
 */
class Manager : public isam::base::Manager<isam::timesteps::Manager, isam::TimeStep> {
public:
  // Methods
  Manager();
  virtual                     ~Manager() noexcept(true);
  void                        Validate() override final;
  void                        Build() override final;
  void                        Execute(unsigned year);
  TimeStepPtr                 GetTimeStep(const string& label) const;
  void                        RemoveAllObjects() override final { objects_.clear(); ordered_time_steps_.clear(); }

private:
  // Members
  vector<TimeStepPtr>         ordered_time_steps_;
  vector<DerivedQuantityPtr>  derived_quantities_;

};

} /* namespace timesteps */
} /* namespace isam */
#endif /* TIMESTEPS_MANAGER_H_ */