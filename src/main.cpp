#include "main.h"
#include "json.hpp"
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

//#define LCD_OLD //define if not using default lcd manager
#define LCD_NEW

using json = nlohmann::json;

/* === TODO ===
 * /DONE/- [Driving] multiple joystick turning / strafing 
 * /DONE/ - [Debugging] graphic display using custom manager
 * /DONE/ - [Nice to have] RTOS async functionality
 * /DONE/ - [Nice to have] save/replay system
 * - [Nice to have] comment all code (incl. logic behind driving math)
 * 
 * 
 * - [Must] all be in design notebook
*/

int drive_mode = 0; //goes between 0, 1, and 2
int power_multiplier = 1000; //overvolt. is 16000, so 12000 just for now
int auton_type = 0; //0 is blue left, 1 is blue right, 3 is red left, 4 is red right 

char *file_toload;
json input_ins;

Controller master(E_CONTROLLER_MASTER);

//port 10 is broken
Motor lf_mtr(9, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_ROTATIONS);
Motor lb_mtr(8, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_ROTATIONS);
Motor rf_mtr(3, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_ROTATIONS);
Motor rb_mtr(2, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_ROTATIONS);
Motor stick(11, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_ROTATIONS);

void on_center_button() { //for default lcd manager (ideally not use)
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		lcd::set_text(2, "I was pressed!");
	} else {
		lcd::clear_line(2);
	}
}

bool logging = true;
bool replaying = false;

void sd_logging(void *param) { //what this does is write multiple lines into a file (test.log)
	if (!usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.
    FILE* usd_file_write = fopen("/usd/test.log", "w");
    int logint = 0;
	while(logging){
        //output the log to a file
        /*std::string log_text = "{[TIME]" + std::to_string(millis()) + "}<\n" + \
                                "{[LBVOL]" + std::to_string(lb_mtr.get_voltage()) + "}\n" + \
                                ">\n\n";*/
        json output = {
            {logint, {
                {"time", millis()},
                {"lf", {
                    {"voltage", lf_mtr.get_voltage()},
                    {"velocity", lf_mtr.get_actual_velocity()},
                    {"position", lf_mtr.get_position()},
                    {"torque", lf_mtr.get_torque()},
                    {"direction", lf_mtr.get_direction()},
                    {"stopped", lf_mtr.is_stopped()}
                }},
                {"lb", {
                    {"voltage", lb_mtr.get_voltage()},
                    {"velocity", lb_mtr.get_actual_velocity()},
                    {"position", lb_mtr.get_position()},
                    {"torque", lb_mtr.get_torque()},
                    {"direction", lb_mtr.get_direction()},
                    {"stopped", lb_mtr.is_stopped()}
                }},
                {"rf", {
                    {"voltage", rf_mtr.get_voltage()},
                    {"velocity", rf_mtr.get_actual_velocity()},
                    {"position", rf_mtr.get_position()},
                    {"torque", rf_mtr.get_torque()},
                    {"direction", rf_mtr.get_direction()},
                    {"stopped", rf_mtr.is_stopped()}
                }},
                {"rb", {
                    {"voltage", rb_mtr.get_voltage()},
                    {"velocity", rb_mtr.get_actual_velocity()},
                    {"position", rb_mtr.get_position()},
                    {"torque", rb_mtr.get_torque()},
                    {"direction", rb_mtr.get_direction()},
                    {"stopped", rb_mtr.is_stopped()}
                }}
            }}
        };
        //std::string log_text = std::to_string(logint) + ": {\n\t\"time\": " + std::to_string(millis()) + ", \n\t\"lf\": {\n\t\t\"vol\": " + std::to_string(lf_mtr.get_voltage()) + ",\n\t\t\"vel\": " + std::to_string(lf_mtr.get_actual_velocity()) + ",\n\t\t\"pos\": " + std::to_string(lf_mtr.get_position()) + ",\n\t\t\"tor\": " + std::to_string(lf_mtr.get_torque()) + ",\n\t\t\"dir\": " + std::to_string(lf_mtr.get_direction()) + ",\n\t\t\"stopped\": " + std::to_string(lf_mtr.is_stopped()) + "\n\t},\n\t\"lb\": {\n\t\t\"vol\": " + std::to_string(lb_mtr.get_voltage()) + ",\n\t\t\"vel\": " + std::to_string(lb_mtr.get_actual_velocity()) + ",\n\t\t\"pos\": " + std::to_string(lb_mtr.get_position()) + ",\n\t\t\"tor\": " + std::to_string(lb_mtr.get_torque()) + ",\n\t\t\"dir\": " + std::to_string(lb_mtr.get_direction()) + ",\n\t\t\"stopped\": " + std::to_string(lb_mtr.is_stopped()) + "\n\t},\n\t\"rf\": {\n\t\t\"vol\": " + std::to_string(rf_mtr.get_voltage()) + ",\n\t\t\"vel\": " + std::to_string(rf_mtr.get_actual_velocity()) + ",\n\t\t\"pos\": " + std::to_string(rf_mtr.get_position()) + ",\n\t\t\"tor\": " + std::to_string(rf_mtr.get_torque()) + ",\n\t\t\"dir\": " + std::to_string(rf_mtr.get_direction()) + ",\n\t\t\"stopped\": " + std::to_string(rf_mtr.is_stopped()) + "\n\t},\n\t\"rb\": {\n\t\t\"vol\": " + std::to_string(rb_mtr.get_voltage()) + ",\n\t\t\"vel\": " + std::to_string(rb_mtr.get_actual_velocity()) + ",\n\t\t\"pos\": " + std::to_string(rb_mtr.get_position()) + ",\n\t\t\"tor\": " + std::to_string(rb_mtr.get_torque()) + ",\n\t\t\"dir\": " + std::to_string(rb_mtr.get_direction()) + ",\n\t\t\"stopped\": " + std::to_string(rb_mtr.is_stopped()) + "\n\t},\n},\n";
        std::string log_text = output.dump(4);
		fputs(log_text.c_str(), usd_file_write);
        logint++;
		delay(100);
	}
	fclose(usd_file_write);
}

void sd_load(){
    if (!usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.

    //reading from file
    FILE* usd_file_read = fopen(file_toload, "r");
    char buf[50]; // This just needs to be larger than the contents of the file
    fread(buf, 1, 50, usd_file_read); // passing 1 because a `char` is 1 byte, and 50 b/c it's the length of buf
    printf("%s\n", buf); // print the string read from the file
    // Should print "Example text" to the terminal
    fclose(usd_file_read); // always close files when you're done with them

    input_ins = json::parse(buf);
}

void sd_replay(void *param){
    int replayint = 0;
    while(replaying){
        if(true){
            lb_mtr.move(input_ins[replayint]["lb"]["voltage"]);
            lf_mtr.move(input_ins[replayint]["lf"]["voltage"]);
            rb_mtr.move(input_ins[replayint]["rb"]["voltage"]);
            rf_mtr.move(input_ins[replayint]["rf"]["voltage"]);
        } // i am VERY aware that we are using almost none of the data that we are storing BUT! it's funny.
        replayint++;
        delay(100);
    }
}

std::string get_files_from_dir(){
    //listing contents of dir
    std::string out;
    std::string path = "/usr/";
    for (const auto & entry : fs::directory_iterator(path))
        out += (std::string(entry.path()).c_str()) + '\n';
    return out;
}

#ifdef LCD_NEW
lv_obj_t * tabview;
lv_obj_t * details_label;
lv_obj_t * drive_label;
lv_obj_t * debug_label;
lv_obj_t * autonbutton1;
lv_obj_t * autonbutton1_label;
lv_obj_t * files_label;
lv_obj_t * files_selector;

static lv_res_t btn_event_handler(lv_obj_t * btn) {
    uint8_t id = lv_obj_get_free_num(btn);
    if(id == 0) { //for autonbutton1
        if (auton_type == 3) {auton_type = 0;}
        else {auton_type++;}
        
    }

    return LV_RES_OK;
}

void gui_manager(void * param){
    tabview = lv_tabview_create(lv_scr_act(), NULL);
    lv_obj_t *details_tab = lv_tabview_add_tab(tabview, "DETAILS");
    lv_obj_t *auton_tab = lv_tabview_add_tab(tabview, "AUTON");
    lv_obj_t *drive_tab = lv_tabview_add_tab(tabview, "DRIVE");
    lv_obj_t *debug_tab = lv_tabview_add_tab(tabview, "DEBUG");
    lv_obj_t *files_tab = lv_tabview_add_tab(tabview, "FILES");
    
    details_label = lv_label_create(details_tab, NULL);
    autonbutton1 = lv_btn_create(auton_tab, NULL);
    drive_label = lv_label_create(drive_tab, NULL);
    debug_label = lv_label_create(debug_tab, NULL);
    files_label = lv_label_create(files_tab, NULL);
    files_selector = lv_ddlist_create(files_tab, NULL);
    
    lv_label_set_text(details_label, " --- Drive One --- \n \
									Left Joystick -> straightwards and strafing\n \
									Right Joystick -> turning\n\n \
									 --- Drive Two --- \n \
									Effectively tank controls, but x axis controls strafing\n\n \
									 --- Drive Three --- \n \
									Left Joystick -> straightwards and turning\n\
									Right Joystick -> strafing\n\n \
									 --- Others --- \n \
									Up -> Shift up a drive mode \n \
									Down -> Shift down a drive mode \n \
									A -> Activates Logging\n \
									B -> Deactivates Logging\n \
									");
    //lv_obj_set_event_cb(autonbutton1, auton_event_handler);
    autonbutton1_label = lv_label_create(autonbutton1, NULL);
    lv_label_set_text(autonbutton1_label, "Auton");
    lv_btn_set_action(autonbutton1, LV_BTN_ACTION_CLICK, btn_event_handler);
    lv_label_set_text(files_label, get_files_from_dir().c_str());
    lv_ddlist_set_options(files_selector, get_files_from_dir().c_str());

    lv_obj_align(debug_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(drive_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    while(true){
        lv_ddlist_get_selected_str(files_selector, file_toload);

        //update labels here
        lv_label_set_text(drive_label, ("Drive Type: " + std::to_string(drive_mode)).c_str());
        lv_label_set_text(debug_label, ("LB Motor Voltage: " + std::to_string(lb_mtr.get_voltage()) + \
                                "\nLF Motor Voltage: " + std::to_string(lf_mtr.get_voltage()) + \
                                "\nRB Motor Voltage: " + std::to_string(rb_mtr.get_voltage()) + \
                                "\nRF Motor Voltage: " + std::to_string(rf_mtr.get_voltage()) + \
                                "\n" + \
                                "\nSD Card Connected: " + std::to_string(usd::is_installed()) + \
                                "\nBattery Current: " + std::to_string(battery::get_current()) + \
                                "\nBattery Voltage: " + std::to_string(battery::get_voltage()) + \
                                "\n"\
                                       ).c_str()\
                                );
        switch(auton_type){
//            case default:
//                lv_label_set_text(autonbutton1_label, "||||||||||||||");
//                break;
            case 0: //blue left
                lv_label_set_text(autonbutton1_label, "Blue, Left");
                break;
            case 1: //blue right
                lv_label_set_text(autonbutton1_label, "Blue, Right");
                break;
            case 2: //red left
                lv_label_set_text(autonbutton1_label, "Red, Left");
                break;
            case 3: //red right
                lv_label_set_text(autonbutton1_label, "Red, Right");
                break;
        }
        delay(250); //update every half a second
    }
}
#endif

void initialize() {
    //stick.set_brake_mode(E_MOTOR_BRAKE_HOLD); // do not; burns out motors
#ifdef LCD_OLD
	lcd::initialize();
	lcd::set_text(1, "Hello PROS User!");

	lcd::register_btn1_cb(on_center_button);
#endif
#ifdef LCD_NEW
    Task gui(gui_manager);
#endif
	Task sd_log(sd_logging);
    Task replay(sd_replay);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void drive_one(){
    float axis_4 = master.get_analog(ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(ANALOG_RIGHT_X); //right side
    
    float f_lf = (axis_3 + (-axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_lb = (axis_3 + (axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_rf = (axis_3 + (-axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rb = (axis_3 + (axis_4) - axis_1)/(3*127)*power_multiplier;
    
    lf_mtr.move(f_lf);
    lb_mtr.move(f_lb);
    rf_mtr.move(f_rf);
    rb_mtr.move(f_rb);
}

void drive_two(){
    float axis_4 = master.get_analog(ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(ANALOG_RIGHT_X); //right side
    
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
    float axis_4 = master.get_analog(ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(ANALOG_RIGHT_X); //right side
    
    float f_lf = (axis_3 + (axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_lb = (axis_3 + (axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rf = (axis_3 + (-axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rb = (axis_3 + (-axis_4) + axis_1)/(3*127)*power_multiplier;
    
    lf_mtr.move(f_lf);
    lb_mtr.move(f_lb);
    rf_mtr.move(f_rf);
    rb_mtr.move(f_rb);
}

void opcontrol() {
	while (true) {
#ifdef LCD_OLD
		lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_OLD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_OLD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_OLD_BTN_RIGHT) >> 0);
        lcd::print(4, std::to_string(drive_mode).c_str());
#endif        
        if (master.get_digital(E_CONTROLLER_DIGITAL_UP)) { drive_mode++; delay(75); }
        if (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) { drive_mode--; delay(75); }
        
        if(master.get_digital(E_CONTROLLER_DIGITAL_A)) { logging = true; }
        if(master.get_digital(E_CONTROLLER_DIGITAL_B)) { logging = false; }
        if(master.get_digital(E_CONTROLLER_DIGITAL_X)) { replaying = true; }
        if(master.get_digital(E_CONTROLLER_DIGITAL_Y)) { replaying = false; }

        if (drive_mode == 0){
            drive_one();
        } else if (drive_mode == 1){
            drive_two();
        } else if (drive_mode == 2){
            drive_three();
        } else if (drive_mode >= 3){
            drive_mode = 2;
        } else if (drive_mode <= -1){
            drive_mode = 0;
        }
        
        if (master.get_digital(E_CONTROLLER_DIGITAL_L2)){
            stick.move(45);
        } else if (master.get_digital(E_CONTROLLER_DIGITAL_L1)){
            stick.move(-45);
        } else {
            stick.move(0);
        }
        
        delay(20);
	}
}

