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

static char *file_toload;
static json input_ins;

static bool logging = true;
void set_logging(bool in);

static bool replaying = false;
void set_replaying(bool in);

void sd_logging(void *param);
void sd_load();
void sd_replay(void *param);
std::string get_files_from_dir();

#endif
