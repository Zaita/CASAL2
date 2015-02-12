/**
 * @file Estimable.Test.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 1/09/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifdef TESTMODE

// headers
#include "Estimable.h"

#include "Model/Model.h"
#include "ObjectiveFunction/ObjectiveFunction.h"
#include "TestResources/TestFixtures/InternalEmptyModel.h"

// namespaces
namespace niwa {
namespace asserts {

using niwa::testfixtures::InternalEmptyModel;

const std::string simple_model =
R"(
@model
start_year 1994
final_year 2008
min_age 1
max_age 50
age_plus t
initialisation_phases iphase1 iphase2
time_steps step_one step_two

@categories
format stage.sex
names immature.male mature.male immature.female mature.female

@initialisation_phase iphase1
years 200
exclude_processes Fishing

@initialisation_phase iphase2
years 1
exclude_processes Fishing

@time_step step_one
processes Recruitment maturation halfM Fishing halfM

@time_step step_two
processes my_ageing

@ageing my_ageing
categories *

@Recruitment Recruitment
type constant
categories stage=immature
proportions 0.5 0.5
R0 997386
age 1

@mortality halfM
type constant_rate
categories *
M 0.065 0.065 0.065 0.065
selectivities One One One One

@mortality Fishing
type event
categories *
years           1998         1999         2000         2001         2002         2003         2004          2005          2006          2007
catches  1849.153714 14442.000000 28323.203463 24207.464203 47279.000000 58350.943094 82875.872790 115974.547730 113852.472257 119739.517172
U_max 0.99
selectivities FishingSel FishingSel FishingSel FishingSel
penalty event_mortality_penalty

@maturation maturation
type rate
from stage=immature
to stage=mature
proportions 1.0 1.0
selectivities Maturation Maturation

@selectivity One
type constant
c 1

@selectivity Maturation
type logistic_producing
L 5
H 30
a50 8
ato95 3

@selectivity FishingSel
type logistic
a50 8
ato95 3

@catchability CPUEq
q 0.000153139

@derived_quantity abundance
type abundance
categories *
selectivities Maturation Maturation Maturation Maturation
time_step step_one

@estimate catchability[CPUEq].q
parameter catchability[CPUEq].q
lower_bound 1e-10
upper_bound 1e-1
type uniform

@estimate process[Recruitment].R0
parameter process[Recruitment].R0
lower_bound 1e5
upper_bound 1e10
type uniform_log

@estimate selectivity[FishingSel].a50
parameter selectivity[FishingSel].a50
lower_bound 1
upper_bound 20
type uniform

@estimate selectivity[FishingSel].ato95
parameter selectivity[FishingSel].ato95
lower_bound 0.01
upper_bound 50
type uniform

@penalty event_mortality_penalty
type process
log_scale False
multiplier 10

@assert process[Recruitment].R0
type estimable
time_step step_two
years 1994 1996 1998
values 997386 997386 997386
)";

/**
 *
 */
TEST_F(InternalEmptyModel, Asserts_Estimable) {
  AddConfigurationLine(simple_model, __FILE__, 30);
  LoadConfiguration();

  ModelPtr model = Model::Instance();
  model->Start(RunMode::kBasic);

  ObjectiveFunction& obj_function = ObjectiveFunction::Instance();
  EXPECT_DOUBLE_EQ(13.81289313550076, obj_function.score());
}

/**
 *
 */
const std::string simple_model_fail =
R"(
@model
start_year 1994
final_year 2008
min_age 1
max_age 50
age_plus t
initialisation_phases iphase1 iphase2
time_steps step_one step_two

@categories
format stage.sex
names immature.male mature.male immature.female mature.female

@initialisation_phase iphase1
years 200
exclude_processes Fishing

@initialisation_phase iphase2
years 1
exclude_processes Fishing

@time_step step_one
processes Recruitment maturation halfM Fishing halfM

@time_step step_two
processes my_ageing

@ageing my_ageing
categories *

@Recruitment Recruitment
type constant
categories stage=immature
proportions 0.5 0.5
R0 997386
age 1

@mortality halfM
type constant_rate
categories *
M 0.065 0.065 0.065 0.065
selectivities One One One One

@mortality Fishing
type event
categories *
years           1998         1999         2000         2001         2002         2003         2004          2005          2006          2007
catches  1849.153714 14442.000000 28323.203463 24207.464203 47279.000000 58350.943094 82875.872790 115974.547730 113852.472257 119739.517172
U_max 0.99
selectivities FishingSel FishingSel FishingSel FishingSel
penalty event_mortality_penalty

@maturation maturation
type rate
from stage=immature
to stage=mature
proportions 1.0 1.0
selectivities Maturation Maturation

@selectivity One
type constant
c 1

@selectivity Maturation
type logistic_producing
L 5
H 30
a50 8
ato95 3

@selectivity FishingSel
type logistic
a50 8
ato95 3

@catchability CPUEq
q 0.000153139

@derived_quantity abundance
type abundance
categories *
selectivities Maturation Maturation Maturation Maturation
time_step step_one

@estimate catchability[CPUEq].q
parameter catchability[CPUEq].q
lower_bound 1e-10
upper_bound 1e-1
type uniform

@estimate process[Recruitment].R0
parameter process[Recruitment].R0
lower_bound 1e5
upper_bound 1e10
type uniform_log

@estimate selectivity[FishingSel].a50
parameter selectivity[FishingSel].a50
lower_bound 1
upper_bound 20
type uniform

@estimate selectivity[FishingSel].ato95
parameter selectivity[FishingSel].ato95
lower_bound 0.01
upper_bound 50
type uniform

@penalty event_mortality_penalty
type process
log_scale False
multiplier 10

@assert r0
type estimable
parameter process[Recruitment].R0
time_step step_two
years 1994 1996 1998
values 997386 1 997386
)";

/**
 *
 */
TEST_F(InternalEmptyModel, Asserts_Estimable_Throws_Exception) {
  AddConfigurationLine(simple_model_fail, __FILE__, 175);
  LoadConfiguration();

  ModelPtr model = Model::Instance();
  EXPECT_THROW(model->Start(RunMode::kBasic), std::string);
}

}
}


#endif
