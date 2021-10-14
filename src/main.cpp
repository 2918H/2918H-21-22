#include "main.h"


/* === TODO ===
 * /DONE/- [Driving] multiple joystick turning / strafing 
 * /DONE/ - [Debugging] graphic display using custom manager
 * /DONE/ - [Nice to have] RTOS async functionality
 * /DONE/ - [Nice to have] save/replay system
 * - [Nice to have] comment all code (incl. logic behind driving math)
 * /DONE/- split into multiple seperate h/cpp files    ^^^ comment as you go
 * 
 * - [Must] all be in design notebook
*/

// to debug segfault errors, use: arm-none-eabi-addr2line -faps -e ./bin/monolith.elf <pc hex code>

void initialize() {
    //Start Async tasks for gui, and logging/replay systems
    //Task gui(gui_manager);
	//Task sd_log(sd_logging);
    //Task replay(sd_replay);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	while (true) {
        if (master.get_digital(E_CONTROLLER_DIGITAL_UP)) { set_drive_mode(get_drive_mode() + 1); pros::delay(75); }
        if (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) { set_drive_mode(get_drive_mode() - 1); pros::delay(75); }
        
        if(master.get_digital(E_CONTROLLER_DIGITAL_A)) { set_logging(true); }
        if(master.get_digital(E_CONTROLLER_DIGITAL_B)) { set_logging(false); }
        if(master.get_digital(E_CONTROLLER_DIGITAL_X)) { set_replaying(true); }
        if(master.get_digital(E_CONTROLLER_DIGITAL_Y)) { set_replaying(false); }

        drive(); //link this to chasis-drive.cpp
        
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

