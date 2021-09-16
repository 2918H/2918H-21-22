#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "api.h"
static pros::Motor lf_mtr(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor lb_mtr(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor rf_mtr(3, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor rb_mtr(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor stick(11, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS);

static pros::Controller master(pros::E_CONTROLLER_MASTER);

#endif
