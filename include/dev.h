#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "api.h"
static pros::Motor lf_mtr(9, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor lb_mtr(8, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor rf_mtr(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor rb_mtr(2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);

static pros::Motor goalx_mtr(11, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor goaly_mtr(12, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);

static pros::Motor topl_mtr(18, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS);
static pros::Motor lift_mtr(20, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS);

static pros::Motor collector_mtr(19, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);

static pros::Controller master(pros::E_CONTROLLER_MASTER);
static pros::Controller partner(pros::E_CONTROLLER_PARTNER);

static pros::ADIDigitalOut lf_pnm ('A');
static pros::ADIDigitalOut lb_pnm ('B');
static pros::ADIDigitalOut rf_pnm ('C');
static pros::ADIDigitalOut rb_pnm ('D');

static pros::ADIDigitalOut liftl_pnm ('E');
static pros::ADIDigitalOut liftr_pnm ('F');
static pros::ADIDigitalOut liftlow_pnm ('G');
static pros::ADIDigitalOut liftupp_pnm ('H');

static pros::ADIDigitalIn lift_bmp ({{5, 'A'}});
static pros::ADIDigitalIn rot_lmt ({{5, 'B'}});


#endif
