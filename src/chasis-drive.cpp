#include "chasis-drive.h"

int drive_mode = 1; //goes between 1, 2, and 3
//power_multiplier = 1000; //overvolt. is 16000, so 12000 just for now

void drive_one(){
    float axis_4 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X); //right side

    //left joystick controls straightwards and strafing
    //right joystick controls turning
    float f_lf = (axis_3 + (--axis_4) + axis_1)/(3*127)*power_multiplier; // //reversed left side of drive for strafing _only_
    float f_lb = (axis_3 + (-axis_4) + axis_1)/(3*127)*power_multiplier; //reversed forward/backward for back motors
    float f_rf = (axis_3 + (-axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rb = (axis_3 + (axis_4) - axis_1)/(3*127)*power_multiplier;

    lf_mtr.move(f_lf);
    lb_mtr.move(f_lb);
    rf_mtr.move(f_rf);
    rb_mtr.move(f_rb);
}

void drive_two(){
    float axis_4 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X); //right side

    //tank controls
    //x axis controls strafing
    float f_lb = (axis_3 + (axis_4))/(2*127)*power_multiplier;
    float f_lf = (axis_3 + (-axis_4))/(2*127)*power_multiplier;
    float f_rb = (axis_2 + (-axis_1))/(2*127)*power_multiplier;
    float f_rf = (axis_2 + axis_1)/(2*127)*power_multiplier;

    lf_mtr.move(f_lf);
    lb_mtr.move(f_lb);
    rf_mtr.move(f_rf);
    rb_mtr.move(f_rb);
}

void drive_three(){
    float axis_4 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X); //right side

    //left joystick controls straightwards and turning
    //right joystick controls strafing
    float f_lf = (axis_3 + (axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_lb = (axis_3 + (axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rf = (axis_3 + (-axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rb = (axis_3 + (-axis_4) + axis_1)/(3*127)*power_multiplier;

    lf_mtr.move(f_lf);
    lb_mtr.move(f_lb);
    rf_mtr.move(f_rf);
    rb_mtr.move(f_rb);
}

//standard get/set methods for drive_mode
int get_drive_mode(){
    return drive_mode;
}

void set_drive_mode(int in_mode){
    drive_mode = in_mode;
}

void togglet_drive_mode(){
    drive_mode++;
}

//main drive method
void drive(){
    //cap drive_mode at min 1 and max 3
    if (drive_mode == 1){
            drive_one();
    } else if (drive_mode == 2){
            drive_two();
    } else if (drive_mode == 3){
            drive_three();
    } else if (drive_mode >= 4){
            drive_mode = 3;
    } else if (drive_mode <= 0){
            drive_mode = 1;
    }
}
