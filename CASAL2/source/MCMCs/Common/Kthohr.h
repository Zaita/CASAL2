/**
 * @file Kthohr.h
 * @author  Scott Rasmussen (scott@zaita.com)
 * @date 14/01/2020
 * @section LICENSE
 *
 * Copyright Scott Rasmussen (c)2020 - https://www.zaita.com
 *
 * @description
 * This class is an implementation of the Kthohr MCMC algorithms provided at:
 * https://github.com/kthohr/mcmc
 * https://www.kthohr.com/mcmclib.html
 *
 */
#ifndef SOURCE_MCMCS_COMMON_KTHOHR_H_
#define SOURCE_MCMCS_COMMON_KTHOHR_H_

// headers
#include "../MCMC.h"

// namespaces
namespace niwa { class Estimate; }
namespace niwa::mcmcs {

// class declaration
class Kthohr : public niwa::MCMC {
public:
	Kthohr() = delete;
	virtual ~Kthohr() = default;
	explicit Kthohr(shared_ptr<Model> model);
  void                        DoExecute() final;

protected:
  // methods
  void                        DoValidate() final;
  void                        DoBuild() final;
  void												RWMH();

  // members
  vector<Estimate*> 					estimates_;
  string											algorithm_ = "";
  unsigned										burn_in_ = 0;
  unsigned										length_ = 0;
  unsigned										keep_ = 0;
  unsigned                    estimate_count_ = 0;
};

} /* namespace niwa::mcmcs */

#endif /* SOURCE_MCMCS_COMMON_KTHOHR_H_ */
