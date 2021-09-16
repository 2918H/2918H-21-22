#ifndef _GUI_H_
#define _GUI_H_

#include "pros/apix.h" //lvgl library
#include "api.h"
#include "dev.h"
#include "filemanagment.h"
#include "chasis-drive.h"
#include "json.hpp"
using json = nlohmann::json;

static int auton_type = 0; //0 is blue left, 1 is blue right, 3 is red left, 4 is red right

static lv_res_t btn_event_handler(lv_obj_t * btn);
void gui_manager(void * param);

#endif
