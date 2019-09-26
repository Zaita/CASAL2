/**
 * @file Runner.h
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 20/09/2019
 * @section LICENSE
 *
 * Copyright NIWA Science �2019 - www.niwa.co.nz
 */
#ifndef SOURCE_RUNNER_H_
#define SOURCE_RUNNER_H_

// headers
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Model/Models/Age.h"

// namespaces
namespace niwa {

// class declaration
class Runner {
public:
	// methods
	Runner();
	virtual 										~Runner();
	int													Go();
	void                  			set_run_parameters(utilities::RunParameters& options) { run_parameters_ = options; }
	GlobalConfiguration&        global_configuration() { return global_configuration_; }
	Model&											model() { return master_model_; }


private:
	// members
	GlobalConfiguration					global_configuration_;
	model::Age									master_model_;
	utilities::RunParameters		run_parameters_;
};

} /* namespace niwa */

#endif /* SOURCE_RUNNER_H_ */