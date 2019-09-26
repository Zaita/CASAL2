/**
 * @file Base.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 2/09/2013
 * @section LICENSE
 *
 * Copyright NIWA Science ©2013 - www.niwa.co.nz
 *
 */
#ifdef TESTMODE

// headers
#include "Base.h"

#include "Model/Model.h"
#include "Model/Models/Age.h"
#include "Model/Managers.h"

// namespaces
namespace niwa {
namespace testfixtures {

/**
 *
 */
void Base::SetUp() {
  model_ = new model::Age();
}

/**
 *
 */
void Base::TearDown() {
  delete model_;
}

} /* namespace testfixtures */
} /* namespace niwa */
#endif /* TESTMODE */
