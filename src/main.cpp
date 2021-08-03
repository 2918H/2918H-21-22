 #include "main.h"
//#define LCD_OLD //define if not using default lcd manager
#define LCD_NEW

/* === TODO ===
 * /DONE/- [Driving] multiple joystick turning / strafing 
 * - [Debugging] graphic display using custom manager
 * - [Nice to have] RTOS async functionality
 * - [Nice to have] comment all code (incl. logic behind driving math)
 * 
 * 
 * - [Must] all be in design notebook
*/

int drive_mode = 1; //goes between 0, 1, and 2
int power_multiplier = 12000; //overvolt. is 16000, so 12000 just for now
int auton_type = 0; //0 is blue left, 1 is blue right, 3 is red left, 4 is red right 

/* --- motor telemetry ideas ---
 *  - .get_actual_velocity()
 *  - .get_direction()
 *  - .get_position()
 *  - .get_voltage()
 *  - .get_torque()
 *  - .is_stopped()
 *  |||
 *  vvv
 *  all written to a file, maybe there's and inbuilt data storage system <- can turn into easy autonomous
 *  \-> write json parser / ctrl+c, ctrl+v s  -> see https://kishoreganesh.com/post/writing-a-json-parser-in-cplusplus/ 
 * 
*/

/* --- RTOS (multitasking) ideas ---
 *  - GUI manager task
 *  - Seperate tasks for drive (4 mtrs) and others (arms, hands, etc) (4 mtrs)
 *  - File manager task <-> read / write logs from motors
 *  - 
 * MAKE SURE NO STACK OVERFLOWS OR SYSTEM TASK`FAILURES
*/

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

#ifdef LCD_NEW
lv_obj_t * tabview;
lv_obj_t * drive_label;
lv_obj_t * debug_label;
lv_obj_t * autonbutton1;
lv_obj_t * autonbutton1_label;

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
    lv_obj_t *auton_tab = lv_tabview_add_tab(tabview, "AUTON");
    lv_obj_t *drive_tab = lv_tabview_add_tab(tabview, "DRIVE");
    lv_obj_t *debug_tab = lv_tabview_add_tab(tabview, "DEBUG");
    
    autonbutton1 = lv_btn_create(auton_tab, NULL);
    drive_label = lv_label_create(drive_tab, NULL);
    debug_label = lv_label_create(debug_tab, NULL);
    
    //lv_obj_set_event_cb(autonbutton1, auton_event_handler);
    autonbutton1_label = lv_label_create(autonbutton1, NULL);
    lv_label_set_text(autonbutton1_label, "Auton");
    lv_btn_set_action(autonbutton1, LV_BTN_ACTION_CLICK, btn_event_handler);
    
    lv_obj_align(debug_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(drive_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    while(true){
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
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

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

void drive_one(){
    float axis_4 = master.get_analog(ANALOG_LEFT_X); //left side
    float axis_3 = master.get_analog(ANALOG_LEFT_Y); //left up
	float axis_2 = master.get_analog(ANALOG_RIGHT_Y); //right up
    float axis_1 = master.get_analog(ANALOG_RIGHT_X); //right side
    
    float f_lf = (axis_3 + (axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_lb = (axis_3 + (-axis_4) + axis_1)/(3*127)*power_multiplier;
    float f_rf = (axis_3 + (-axis_4) - axis_1)/(3*127)*power_multiplier;
    float f_rb = (axis_3 + (axis_4) - axis_1)/(3*127)*power_multiplier;
    
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
        if (master.get_digital(E_CONTROLLER_DIGITAL_UP)) drive_mode++;
        if (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) drive_mode--;
        
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

