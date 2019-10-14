/**
 * @file RasmussenDunn.cpp
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 3/10/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 */

// headers
#include "RasmussenDunn.h"

#include "../../Model/Model.h"

// namespaces
namespace niwa {
namespace mcmcs {

RasmussenDunn::RasmussenDunn(shared_ptr<Model> model) : MCMC(model) {
//	parameters_.Bind<unsigned>(PARAM_LENGTH, &length_, "Length of chain", "");

}

void RasmussenDunn::DoExecute() {
	LOG_TRACE();

	for (unsigned i = 0; i < length_; ++i)
		model_->FullIteration();
}

void RasmussenDunn::DoValidate() {

}

void RasmussenDunn::DoBuild() {

}

} /* namespace mcmcs */
} /* namespace niwa */
