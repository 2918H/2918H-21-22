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

void autonomous() {}

void opcontrol() {
	while (true) {
        if (master.get_digital(E_CONTROLLER_DIGITAL_UP)) { set_drive_mode(get_drive_mode() + 1); pros::delay(75); }
        if (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) { set_drive_mode(get_drive_mode() - 1); pros::delay(75); }
        
        //if(master.get_digital(E_CONTROLLER_DIGITAL_A)) { set_logging(true); }
        //if(master.get_digital(E_CONTROLLER_DIGITAL_B)) { set_logging(false); }
        //if(master.get_digital(E_CONTROLLER_DIGITAL_X)) { set_replaying(true); }
        //if(master.get_digital(E_CONTROLLER_DIGITAL_Y)) { set_replaying(false); }

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
        /*static bool toggleX { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
            piston.set_value(!toggleX);    //When false go to true and in reverse
            toggleX = !toggleX;    //Flip the toggle to match piston state
        }*/
        static bool toggleY { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
            lf_pnm.set_value(!toggleY);
            lb_pnm.set_value(!toggleY);
            rf_pnm.set_value(!toggleY);
            rb_pnm.set_value(!toggleY);
            toggleY = !toggleY;    //Flip the toggle to match piston state
        }

        static bool toggleUP { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
            liftlow_pnm.set_value(!toggleUP);
            liftupp_pnm.set_value(!toggleUP);
            toggleUP = !toggleUP;   //Flip the toggle to match piston state
        }
        static bool toggleR { false };    //This static variable will keep state between loops or function calls
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)) {
            liftl_pnm.set_value(!toggleR);
            liftr_pnm.set_value(!toggleR);
            toggleR = !toggleR;    //Flip the toggle to match piston state
        }

        /*if (master.get_digital(E_CONTROLLER_DIGITAL_L2)){
            stick.move(45);
        } else if (master.get_digital(E_CONTROLLER_DIGITAL_L1)){
            stick.move(-45);
        } else {
            stick.move(0);
        }*/
        
        pros::delay(50);
	}
}

