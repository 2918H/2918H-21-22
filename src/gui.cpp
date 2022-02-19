#include "gui.h"

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
									A -> Toggles Logging\n \
									B -> Toggles Replaying\n \
									X -> Toggles Drive Mode\n \
									UP -> Toggles Can Opener <Goal Spin>\n \
									DOWN -> Toggles Goal Lift\n \
									");
    //lv_obj_set_event_cb(autonbutton1, auton_event_handler);
    autonbutton1_label = lv_label_create(autonbutton1, NULL);
    lv_label_set_text(autonbutton1_label, "Auton");
    lv_btn_set_action(autonbutton1, LV_BTN_ACTION_CLICK, btn_event_handler);
    lv_label_set_text(files_label, /*get_files_from_dir().c_str()*/ "test. test. test.");

    std::string filepaths = log_paths.dump(1);
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), '{'), filepaths.end());
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), '}'), filepaths.end());
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), ','), filepaths.end());
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), '"'), filepaths.end());
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), '\''), filepaths.end());
    filepaths.erase(remove(filepaths.begin(), filepaths.end(), ' '), filepaths.end());
    lv_ddlist_set_options(files_selector, /*filepaths.c_str()*/ "test. \ntest. \ntest.");

    lv_obj_align(debug_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(drive_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(files_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(files_selector, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

    char * file_toload_buf; //using this varible breaks the memory
    uint16_t file_toload_size;
    while(true){
        //lv_ddlist_get_selected_str(files_selector, file_toload_buf);
        //set_file_toload(file_toload_buf, 7);

        //update labels here
        lv_label_set_text(drive_label, ("Drive Type: " + std::to_string(get_drive_mode())).c_str());
        lv_label_set_text(debug_label, ("LB Motor Voltage: " + std::to_string(lb_mtr.get_voltage()) + \
                                "\nLF Motor Voltage: " + std::to_string(lf_mtr.get_voltage()) + \
                                "\nRB Motor Voltage: " + std::to_string(rb_mtr.get_voltage()) + \
                                "\nRF Motor Voltage: " + std::to_string(rf_mtr.get_voltage()) + \
                                "\n" + \
                                "\nSD Card Connected: " + std::to_string(pros::usd::is_installed()) + \
                                "\nBattery Current: " + std::to_string(pros::battery::get_current()) + \
                                "\nBattery Voltage: " + std::to_string(pros::battery::get_voltage()) + \
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
        delay(500); //update every half a second
    }
}
