#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include "api.h"
#include "dev.h"
#include "json.hpp"
using json = nlohmann::json;
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

static std::string file_toload = "";
void set_file_toload(char* strin, uint16_t strin_size);
static json input_ins;
static json log_paths;
const std::string log_registrar_path = "/usd/log-path.json";

static bool logging = true;
void set_logging(bool in);
void flip_logging();

static bool replaying = false;
void set_replaying(bool in);
void flip_replaying();

void sd_logging(void *param);
void sd_load();
void sd_replay(void *param);
std::string get_files_from_dir();
void load_logs();

#endif
