/**
 * @file PiApproximation.cpp
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 20/09/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 */

// headers
#include <Model/Models/PiApproximation.h>

// namespaces
namespace niwa {
namespace model {

PiApproximation::PiApproximation() {
	partition_type_ = PartitionType::kPiApprox;
}

PiApproximation::~PiApproximation() {
}

} /* namespace model */
} /* namespace niwa */
