/**
 * @file Model.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 16/11/2012
 * @section LICENSE
 *
 * Copyright NIWA Science �2012 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * This class is the primary representation of our model and it's states
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef MODEL_H_
#define MODEL_H_

// Headers
#include <boost/shared_ptr.hpp>

#include "BaseClasses/Object.h"
#include "Catchabilities/Manager.h"

// Namespaces
namespace isam {

using boost::shared_ptr;
using isam::catchabilities::CatchabilityManagerPtr;

// Enumerated Types
namespace RunMode {
enum Type {
  kInvalid      = 1,
  kLicense      = 2,
  kVersion      = 3,
  kHelp         = 4,
  kBasic        = 8,
  kEstimation   = 16,
  kMCMC         = 32,
  kSimulation   = 64,
  kProfiling    = 128,
  kTesting      = 256
};}

namespace State {
enum Type {
  kInitialise,
  kValidate,
  kBuild,
  kVerify,
  kPreExecute,
  kExecute,
  kPostExecute,
  kIterationComplete,
  kFinalise,
  kReset
};
}

/**
 * Class definition
 */
class Model : public base::Object {
public:
  // Methods
  static shared_ptr<Model>    Instance(bool force_new = false);
  virtual                     ~Model() = default;
  void                        Start();
  void                        FullIteration();

  // Accessors
  void                        set_run_mode(RunMode::Type new_mode) { run_mode_ = new_mode; }
  RunMode::Type               run_mode() { return run_mode_; }
  State::Type                 state() { return state_; }
  unsigned                    start_year() { return start_year_; }
  unsigned                    final_year() { return final_year_; }
  unsigned                    current_year() { return current_year_; }
  virtual unsigned            min_age() const { return min_age_; }
  virtual unsigned            max_age() const { return max_age_; }
  virtual unsigned            age_spread() const { return (max_age_ - min_age_) + 1; }
  bool                        age_plus() const { return age_plus_; }
  const vector<string>&       time_steps() const { return time_steps_; }
  const vector<string>&       initialisation_phases() const { return initialisation_phases_; }

protected:
  // Methods
  Model();
  void                        Initialise();
  void                        Validate();
  void                        Build();
  void                        Verify();
  void                        Iterate();
  void                        Reset();
  void                        RunBasic();
  void                        RunEstimation();
  void                        RunMCMC();

  // Members
  RunMode::Type               run_mode_ = RunMode::kInvalid;
  State::Type                 state_    = State::kInitialise;
  unsigned                    start_year_ = 0;
  unsigned                    final_year_ = 0;
  unsigned                    current_year_ = 0;
  unsigned                    min_age_ = 0;
  unsigned                    max_age_ = 0;
  bool                        age_plus_ = true;
  vector<string>              initialisation_phases_;
  vector<string>              time_steps_;
  CatchabilityManagerPtr      catchability_manager_;
};

/**
 * Typedef
 */
typedef boost::shared_ptr<Model> ModelPtr;

} /* namespace isam */
#endif /* MODEL_H_ */























