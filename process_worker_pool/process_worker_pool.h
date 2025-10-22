#ifndef _WORKER_POOL
#define _WORKER_POOL

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "../url_file_loader/url_file_loader.h"

class ProcessWorkerPool {
    public:
        std::vector<pid_t> pids;
        std::vector<FileLoader> tasks;

        void add_task(FileLoader loader);
        void create_processes(void);
        void waiting_processes(void);
};

#endif