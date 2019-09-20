/**
 * @file Runner.cpp
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 20/09/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 *
 * @description
 * This class is the class that is spawned from main() and holds pointers to everything. It's the class
 * that creates the models and hands them off to the different objects for running (minimisers, mcmcs etc)
 *
 */

// headers
#include "Runner.h"

#include <string>
#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>

#include "BaseClasses/Object.h"
#include "ConfigurationLoader/Loader.h"
#include "Model/Factory.h"
#include "Reports/Common/StandardHeader.h"
#include "Reports/Manager.h"
#include "Utilities/RandomNumberGenerator.h"

// namespaces
namespace niwa {
using std::string;
using std::cout;
using std::endl;

/**
 * Default constructor
 */
Runner::Runner() {
}

/**
 * Destructor
 */
Runner::~Runner() {
}

/**
 * This is the main entry point for our Casal2 system. This is called from the main() and shared library methods.
 */
int Runner::Go() {

	RunMode::Type run_mode = run_parameters_.run_mode_;

	int return_code = 0;

	/**
	 * Check the run mode and call the handler.
	 */
	switch (run_mode) {
	case RunMode::kInvalid:
	case RunMode::kUnitTest:
		LOG_ERROR() << "Invalid run mode specified.";
		return -1;
		break;

	case RunMode::kVersion:
	case RunMode::kHelp:
	case RunMode::kLicense:
		break;
	case RunMode::kQuery: {
		string lookup = global_configuration_.object_to_query();
		vector<string> parts;
		boost::split(parts, lookup, boost::is_any_of("."));
		if (parts.size() == 1)
			parts.push_back("");
		if (parts.size() == 2) {
			master_model_.set_partition_type(PartitionType::kAge);
			base::Object *object = master_model_.factory().CreateObject(parts[0], parts[1], PartitionType::kModel);
			if (object) {
				cout << "Printing information for " << parts[0] << " with sub-type " << parts[1] << endl;
				object->PrintParameterQueryInfo();
			} else {
				master_model_.set_partition_type(PartitionType::kLength);
				object = master_model_.factory().CreateObject(parts[0], parts[1], PartitionType::kModel);
				if (object) {
					cout << "Printing information for " << parts[0] << " with sub-type " << parts[1] << endl;
					object->PrintParameterQueryInfo();
				} else
					cout << "Object type " << lookup << " is invalid" << endl;
			}
		} else {
			cout << "Please use object_type.sub_type only when querying an object" << endl;
		}
	} // case RunMode::kQuery
		break;

	case RunMode::kBasic:
	case RunMode::kEstimation:
	case RunMode::kMCMC:
	case RunMode::kSimulation:
	case RunMode::kProfiling:
	case RunMode::kProjection:
	case RunMode::kTesting:
		reports::StandardHeader standard_report(&master_model_);
		if (!global_configuration_.debug_mode() && !global_configuration_.disable_standard_report()) {
			standard_report.Prepare();
			master_model_.managers().report()->set_std_header(standard_report.header());
		}

		vector<Model*> model_list;
		model_list.push_back(&master_model_);
		for (unsigned i = 0; i < 5; ++i) {
			cout << "Loading model " << (i+1) << " into model_list on Runner" << endl;
			Model* model = new Model();
			model->set_id(i+1);
			model_list.push_back(model);
			cout << "Appended model with id " << model->id() << endl;
		}

		// load our configuration file
		configuration::Loader config_loader;
		if (!config_loader.LoadConfigFile(global_configuration_)) {
			Logging::Instance().FlushErrors();
			return_code = -1;
			break;
		}

		Logging &logging = Logging::Instance();
		config_loader.ParseFileLines();
		config_loader.Build(model_list);
		if (logging.errors().size() > 0) {
			logging.FlushErrors();
			return_code = -1;
			break;
		}

		// override any config file values from our command line
		master_model_.global_configuration().ParseOptions(&master_model_);
		utilities::RandomNumberGenerator::Instance().Reset(master_model_.global_configuration().random_seed());

		// Thread off the reports
		reports::Manager *report_manager = master_model_.managers().report();
		std::thread report_thread([&report_manager]() {
			report_manager->FlushReports();
		});

		// Run the model
		return_code = master_model_.Start(run_mode);

		// finish report thread
		report_manager->StopThread();
		report_thread.join();

		if (logging.errors().size() > 0) {
			logging.FlushErrors();
			return_code = -1;
		} else
			logging.FlushWarnings();

		if (!master_model_.global_configuration().debug_mode() && !master_model_.global_configuration().disable_standard_report())
			standard_report.Finalise();
		break;

	} // switch(run_mode)

	return return_code;
}

} /* namespace niwa */
