#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <unistd.h> 
#include <semaphore.h>

#include "../logger/logger.h"
#include "thread_worker_pool.h"
#include "../url_file_loader/url_file_loader.h"

// class ThreadWorkerPool {
//     public:
//         std::vector<pthread_t> threads;
//         std::queue<FileLoader> task_queue;

//         pthread_mutex_t mutex;
//         pthread_cond_t start_signal_cond;
//         pthread_cond_t finished_cond;
//         int active_tasks;
//         int max_threads;
//         sem_t sem;
//         int completed_tasks;

//         void add_task_into_queue(FileLoader loader);
//         void start();
//         void wait();
//         void handler();
// };

ThreadWorkerPool::ThreadWorkerPool(int max) {
    max_threads = max;
    completed_tasks = 0;
    active_tasks = 0;
    pthread_mutex_init(&mutex, nullptr);
    sem_init(&sem, 0, 0);

    for (int i = 0; i < max_threads; i++) {
        int thread_index = i;
        threads.push_back(0);
        pthread_create(&threads[i], nullptr, handler, this);
    }
    pthread_mutex_lock(&mutex);
    std::cout << get_current_timestamp_with_ms() << ": потоки открыты " << "\n";
    pthread_mutex_unlock(&mutex);
}

void ThreadWorkerPool::add_task_into_queue(FileLoader loader) {
    pthread_mutex_lock(&mutex);
    task_queue.push(loader);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem);
}

void* ThreadWorkerPool::handler(void *args) {
    //pthread_mutex_lock(&mutex);
    //std::cout << get_current_timestamp_with_ms() << "# ---->\n";
    //pthread_mutex_unlock(&mutex);
    static_cast<ThreadWorkerPool*>(args)->work_function();
    return nullptr;
}

void ThreadWorkerPool::work_function() {
    while (true) {
        sem_wait(&sem);

        pthread_mutex_lock(&mutex);

        if (task_queue.empty() && active_tasks == 0) {
            std::cout << get_current_timestamp_with_ms() << " 333" << "\n";
            pthread_mutex_unlock(&mutex);
            std::cout << get_current_timestamp_with_ms() << " 444" << "\n";
            break;
        }

        FileLoader task = task_queue.front();
        task_queue.pop();
        std::cout << get_current_timestamp_with_ms() << ": поток " << " получил задачу" << "\n";
        //std::cout << get_current_timestamp_with_ms() << "  queue size " << task_queue.size() << "\n";
        active_tasks++;

        pthread_mutex_unlock(&mutex);

        task.download_file();

        pthread_mutex_lock(&mutex);

        active_tasks--;
        completed_tasks++;
        std::cout << get_current_timestamp_with_ms() << ": поток " << " выполнил задачу, в очереди осталось " << task_queue.size() << " задач, выполнено " << completed_tasks << " задач" << "\n";

        pthread_mutex_unlock(&mutex);
    }
    return;
}

void ThreadWorkerPool::wait() {

    while (true) {
        pthread_mutex_lock(&mutex);
        if (task_queue.empty() && active_tasks == 0) {
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
}