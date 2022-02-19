#ifndef _CHASSIS_DRIVE_H_
#define _CHASSIS_DRIVE_H_

#include "dev.h"
#include "api.h"
using namespace pros;

int get_drive_mode();
void set_drive_mode(int in_mode);
void togglet_drive_mode();
void drive();
void drive_one();
void drive_two();
void drive_three();

extern int drive_mode;
static int power_multiplier = 1000;

#endif
