/**
 * @file PiApproximation.h
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 20/09/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 */
#ifndef SOURCE_MODEL_MODELS_PIAPPROXIMATION_H_
#define SOURCE_MODEL_MODELS_PIAPPROXIMATION_H_

// headers
#include "Model/Model.h"

// namespaces
namespace niwa {
namespace model {

// class declaration
class PiApproximation : public Model {
public:
	PiApproximation();
	virtual ~PiApproximation();
};

} /* namespace model */
} /* namespace niwa */

#endif /* SOURCE_MODEL_MODELS_PIAPPROXIMATION_H_ */
