/**
 * @file RasmussenDunn.h
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 3/10/2019
 * @section LICENSE
 *
 * Copyright NIWA Science ©2019 - www.niwa.co.nz
 */
#ifndef SOURCE_MCMCS_COMMON_RASMUSSENDUNN_H_
#define SOURCE_MCMCS_COMMON_RASMUSSENDUNN_H_

// headers
#include "../../MCMCs/MCMC.h"
#include "../../Estimates/Estimate.h"

// namespaces
namespace niwa {
namespace mcmcs {

// class declaration
class RasmussenDunn : public MCMC {
public:
	RasmussenDunn() = delete;
	explicit RasmussenDunn(shared_ptr<Model> model);
	virtual ~RasmussenDunn() = default;
  void                        DoExecute() override final;

protected:
  // methods
  void                        DoValidate() override final;
  void                        DoBuild() override final;

//  unsigned										length_ = 0;
};

} /* namespace mcmcs */
} /* namespace niwa */

#endif /* SOURCE_MCMCS_COMMON_RASMUSSENDUNN_H_ */
