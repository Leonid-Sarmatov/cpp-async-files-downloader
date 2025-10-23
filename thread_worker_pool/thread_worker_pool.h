#ifndef _THREAD_EORKER_POOL
#define _THREAD_EORKER_POOL

#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

#include "../url_file_loader/url_file_loader.h"

class ThreadWorkerPool {
    public:
        ThreadWorkerPool(int max);
        
        void add_task_into_queue(HttpFileLoader loader);
        void wait();

    private:
        std::vector<pthread_t> threads;
        std::queue<HttpFileLoader> task_queue;

        pthread_mutex_t mutex;
        sem_t sem;

        int active_tasks;
        int max_threads;
        int completed_tasks;

        static void* handler(void *args);
        void work_function();
};

#endif