#include "main.h"


/* === TODO ===
 * /DONE/- [Driving] multiple joystick turning / strafing 
 * /DONE/ - [Debugging] graphic display using custom manager
 * /DONE/ - [Nice to have] RTOS async functionality
 * /DONE/ - [Nice to have] save/replay system
 * /DONE/ - instead of searching for logs on sd, store the locations in a json file that is always present and loaded
 * - [Nice to have] comment all code (incl. logic behind driving math)
 * /DONE/- split into multiple seperate h/cpp files    ^^^ comment as you go
 * 
 * - [Must] all be in design notebook
*/

// to debug segfault errors, use: arm-none-eabi-addr2line -faps -e ./bin/monolith.elf <pc hex code>

void initialize() {
    //Start Async tasks for gui, and logging/replay systems
    Task gui(gui_manager);
	Task sd_log(sd_logging);
    Task replay(sd_replay);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    lb_pnm.set_value(true);
    pros::delay(500);
    lb_pnm.set_value(false);
}

void opcontrol() {
	while (true) {
        static bool toggleX { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
            togglet_drive_mode();
            toggleX = !toggleX;    //Flip the toggle to match piston state
        }

        drive(); //link this to chasis-drive.cpp
        

        static bool toggleA { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
            flip_logging();
            toggleA = !toggleA;    //Flip the toggle to match piston state
        }
        static bool toggleB { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
            flip_replaying();
            toggleB = !toggleB;    //Flip the toggle to match piston state
        }
        static bool toggleY { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            liftlow_pnm.set_value(!toggleY);
            //lb_pnm.set_value(!toggleY);
            //rf_pnm.set_value(!toggleY);
            //rb_pnm.set_value(!toggleY);
            toggleY = !toggleY;    //Flip the toggle to match piston state
        }

        static bool toggleUP { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
            lb_pnm.set_value(!toggleUP);
            //liftlow_pnm.set_value(!toggleUP);
            //liftupp_pnm.set_value(!toggleUP);
            toggleUP = !toggleUP;   //Flip the toggle to match piston state
        }
        static bool toggleD { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)) {
            lf_pnm.set_value(!toggleD);
            //liftl_pnm.set_value(!toggleR);
            //liftr_pnm.set_value(!toggleR);
            toggleD = !toggleD;    //Flip the toggle to match piston state
        }

        if (master.get_digital(E_CONTROLLER_DIGITAL_L2)){
            collector_mtr.move(45);
            lift_mtr.move(75);
        } else if (master.get_digital(E_CONTROLLER_DIGITAL_L1)){
            collector_mtr.move(-45);
            lift_mtr.move(-75);
        } else {
            collector_mtr.move(0);
            lift_mtr.move(0);
        }

        if (master.get_digital(E_CONTROLLER_DIGITAL_R1)){
            topl_mtr.move(75);
        } else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)){
            topl_mtr.move(-75);
        } else {
            topl_mtr.move(0);
        }
        
        pros::delay(50);
	}
}

