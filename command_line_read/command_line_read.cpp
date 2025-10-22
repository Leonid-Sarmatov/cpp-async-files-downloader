#include "command_line_read.h"

CONFIGS read_configs(int argc, const char* argv[]) {
    CONFIGS cfg;
    if (argc < 3) {
        return cfg;
    }

    cfg.urls_file_name.assign(argv[1]);
    cfg.target_dir.assign(argv[2]);
    cfg.workers_num = atoi(argv[3]);
    
    return cfg;
}

