#include "filemanagment.h"

void set_logging(bool in){
    logging = in;
}

void set_replaying(bool in){
    replaying = in;
}

void sd_logging(void *param) { //what this does is write multiple lines into a file (test.log)
	if (!pros::usd::is_installed()) return; //if there is no SD card, don't try to read/write from/to it.
    FILE* usd_file_write = fopen("/usd/match.log", "w");
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
        pros::delay(100);
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
