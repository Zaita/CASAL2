/**
 * @file Callback.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 14/03/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef USE_AUTODIFF
#ifndef _MSC_VER
#ifndef CALLBACK_H_
#define CALLBACK_H_

// headers
#include <dlib/optimization.h>

#include "../../../Model/Model.h"

// namespaces
namespace niwa {
namespace minimisers {
namespace dlib {

/**
 *
 */
class Callback {
public:
  // methods
  Callback(shared_ptr<Model> model);
  virtual                     ~Callback() = default;
  Double                      operator()(const ::dlib::matrix<double, 0, 1>& Parameters) const;

private:
  // members
  shared_ptr<Model>                    model_;
};

} /* namespace dlib */
} /* namespace minimisers */
} /* namespace niwa */
#endif /* CALLBACK_H_ */
#endif
#endif /* USE_AUTODIFF */
