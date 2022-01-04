#include "filemanagment.h"

void set_file_toload(char * strin){
    file_toload.assign(strin);
}

void set_logging(bool in){
    logging = in;
}

void set_replaying(bool in){
    replaying = in;
}

void sd_logging(void *param) { //what this does is write multiple lines into a file (test.log)
	if (!pros::usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.
	std::string filename = "/usd/match.log" + pros::millis(); //using millis to generate a random-ish number to not overwrite other logs
    FILE* usd_file_write = fopen(filename.c_str(), "w");
    int logint = 0;
	while(logging){
        //output the log to a file
        /*std::string log_text = "{[TIME]" + std::to_string(millis()) + "}<\n" + \
                                "{[LBVOL]" + std::to_string(lb_mtr.get_voltage()) + "}\n" + \
                                ">\n\n";*/
        json output = {
            {logint, {
                {"time", pros::millis()},
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
		pros::delay(100);
	}
	fclose(usd_file_write);
}

void sd_load(){
    if (!pros::usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.
    if (file_toload == "") return;
    //reading from file
    FILE* usd_file_read = fopen(file_toload.c_str(), "r");
    char buf[50]; // This just needs to be larger than the contents of the file
    fread(buf, 1, 50, usd_file_read); // passing 1 because a `char` is 1 byte, and 50 b/c it's the length of buf
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
        pros::delay(100);
    }
}

std::string get_files_from_dir(){
    if (!pros::usd::is_installed()) return "Err: No SD Card"; //if there is no SD card, don't try to read/write from/to it.
    //listing contents of dir
    std::string out = "test\n/usd/ files should be written below:\n";
    std::string path = "/usd/";
    for (const auto & entry : fs::recursive_directory_iterator(path)) {
        out = out + (entry.path().u8string()) + '\n';
        printf(entry.path().u8string().c_str());

    }
    return out;
}

const std::string log_registrar_path = "/usd/log-path.json";

void load_logs(){
    if (!pros::usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.
    FILE* log_registrar = fopen(log_registrar_path.c_str(), "r");
    //int buffer_size = 50;
    char buf[50];
    fread(buf, 1, 50, log_registrar);
    fclose(log_registrar);
    if (strlen(buf) == 0) {
        FILE* log_reg = fopen(log_registrar_path.c_str(), "w");
        json output = {
            {
                "/usd/match.log"
            }
        };
        std::string output_txt = output.dump(4);
		fputs(output_txt.c_str(), log_reg);
        fclose(log_reg);
        return;
    }
    log_paths = json::parse(buf);
}
