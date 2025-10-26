#ifndef _FILE_RW
#define _FILE_RW

#include <vector>
#include <map>
#include <pthread.h>

std::vector<std::string> get_file_lines(std::string file_name);

void print_file_lines(std::vector<std::string> lines);

class FileSaver {
    public:
        FileSaver(std::string);
        void save_file(std::string, const char*, int);

    private:
        std::map<std::string, int> file_names_counter;
        pthread_mutex_t mutex;
        std::string save_directory;
};

#endif