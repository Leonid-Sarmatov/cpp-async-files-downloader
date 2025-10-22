#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sys/stat.h>   // права
#include <unistd.h>     // fork, pipe, dup2, execvp, read, write, close
#include <sys/types.h>  // pid_t, size_t и др. системные типы
#include <sys/wait.h>   // wait, waitpid, WIFEXITED, WEXITSTATUS и т.п.
#include <fcntl.h>      // fcntl, O_NONBLOCK, FD_CLOEXEC
#include <signal.h>     // kill, sigaction, сигналы
#include <errno.h>      // errno

#include "process_worker_pool.h"
#include "../url_file_loader/url_file_loader.h"

// class ProcessWorkerPool {
//     public:
//         std::vector<pid_t> pids;
//         std::vector<FileLoader> tasks;

//         void add_task();
//         void create_processes();
//         void waiting_processes():
// }

void ProcessWorkerPool::add_task(FileLoader fileLoader) {
    pids.push_back(-1);
    tasks.push_back(fileLoader);
}

void ProcessWorkerPool::create_processes() {
    // Итерируемся по указателям на хэндлеры
    for (int i = 0; i < pids.size(); i++) {
        // Стартим процесс
        pid_t pid = fork();
        // В случае ошибки
        if (pid < 0) {
            perror("fork");
        }
        // Если очутились в дочернем процессе,
        if (pid == 0) {
            tasks[i].download_file();        // Cтарт обработчика
            _exit(EXIT_SUCCESS);             // Обработчик отработал, завершаем процесс
        }
        pids[i] = pid;               // Если очутились в родителе, то заполняем массив айдишников
    }
}

void ProcessWorkerPool::waiting_processes() {
    for (int i = 0; i < pids.size(); i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
}

// void create_processes(FileLoader *loaders, int loaders_num)
// {
//     // Итерируемся по указателям на хэндлеры
//     for (int i = 0; i < workers_num; i++)
//     {
//         // Стартим процесс
//         pid_t pid = fork();
//         // В случае ошибки
//         if (pid < 0)
//         {
//             perror("fork");
//         }
//         // Если очутились в дочернем процессе,
//         if (pid == 0)
//         {
//             workers[i].download_file(workers[i].hostname, workers[i].path, workers[i].port, workers[i].filename);        // Cтарт обработчика
//             _exit(EXIT_SUCCESS);                                 // Обработчик отработал, завершаем процесс
//         }
//         workers[i].pid = pid;               // Если очутились в родителе, то заполняем массив айдишников
//     }
// }

// void waiting_processes(FileLoader *loaders, int loaders_num)
// {
//     for (int i = 0; i < workers_num; i++) 
//     {
//         int status;
//         waitpid(workers[i].pid, &status, 0);
//     }
// }