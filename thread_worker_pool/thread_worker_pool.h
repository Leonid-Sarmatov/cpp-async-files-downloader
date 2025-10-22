#ifndef _THREAD_EORKER_POOL
#define _THREAD_EORKER_POOL

#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

#include "../url_file_loader/url_file_loader.h"

class ThreadWorkerPool {
    private:
        std::vector<pthread_t> threads;
        std::queue<FileLoader> task_queue;

        pthread_mutex_t mutex;
        sem_t sem;

        int active_tasks;
        int max_threads;
        int completed_tasks;

        static void* handler(void *args);
        void work_function();

    public:
        void add_task_into_queue(FileLoader loader);
        void wait();

        ThreadWorkerPool(int max);
};

#endif