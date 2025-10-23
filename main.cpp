#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#include "command_line_read/command_line_read.h"
#include "file_rw/file_rw.h"
#include "url_file_loader/url_file_loader.h"
#include "process_worker_pool/process_worker_pool.h"
#include "thread_worker_pool/thread_worker_pool.h"


int main(int argc, const char* argv[])
{
    CONFIGS cfg = read_configs(argc, argv);

    std::cout << "имя файла с ссылками на файлы:\n" << cfg.urls_file_name 
        << "\nдиректория для сохранения файлов:\n" << cfg.target_dir 
        << "\nколичество воркеров (потоков):\n" << cfg.workers_num << "\n\n";

    std::vector<std::string> lines = get_file_lines(cfg.urls_file_name);

    std::cout << "urls:" << "\n";
    print_file_lines(lines);

    mkdir(strdup(cfg.target_dir.c_str()), 0777);

    /*
    ProcessWorkerPool wp;

    for (int i = 0; i < lines.size(); i++) {
        URLInformation info(lines[i], char_pointer_to_string("8080"));
        char fn[64];
        snprintf(fn, sizeof(fn), "./%s/file_%d", argv[2], i);
        FileLoader loader(info, char_pointer_to_string(fn));
        wp.add_task(loader);
    }

    wp.create_processes();

    wp.waiting_processes();
    */

    ThreadWorkerPool wp(cfg.workers_num);

    for (int i = 0; i < lines.size()-1; i++) {
        //std::string port_string;
        //port_string.assign("8080");
        URLInformation info(lines[i]);

        char fn[64];
        snprintf(fn, sizeof(fn), "./%s/file_%d", argv[2], i);
        std::string _filename;
        _filename.assign(fn);
        HttpFileLoader loader(info, _filename);

        wp.add_task_into_queue(loader);
    }

    wp.wait();

    return 0;
}