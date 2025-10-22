#ifndef _COMMAND_LINE_READ
#define _COMMAND_LINE_READ

#include <string>

struct CONFIGS {
    std::string urls_file_name;
    std::string target_dir;
    int workers_num;
};

CONFIGS read_configs(int argc, const char* argv[]);


#endif