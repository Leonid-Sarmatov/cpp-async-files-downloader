#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "command_line_read.h"
#include "../file_rw/file_rw.h"

CONFIGS read_configs(int argc, const char* argv[]) {
    CONFIGS cfg;
    if (argc < 2) {
        return cfg;
    }
    cfg.urls_file_name = char_pointer_to_string(argv[1]);
    cfg.target_dir = char_pointer_to_string(argv[2]);
    
    return cfg;
}

