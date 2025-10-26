#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <pthread.h>

#include "file_rw.h"
#include "../logger/logger.h"

std::vector<std::string> get_file_lines(std::string file_name)
{
    std::ifstream file;
    std::vector<std::string> lines;
    file.open(file_name);
    if (file.is_open())
    {
        while (file)
        {
            std::string line;
            std::getline(file, line);
            lines.push_back(line);
        }
    }
    return lines;
}

void print_file_lines(std::vector<std::string> lines)
{
    for (int i = 0; i < lines.size()-1; i++)
    {
        std::cout << i << ". " << lines[i] << "\n";
    }
    std::cout << "\n";
}

// class FileSaver {
//     public:
//         FileSaver(std::string);
//         void save_file(std::string, const char*, int);

//     private:
//         std::map<std::string, int> file_names_counter;
//         pthread_mutex_t mutex;
//         std::string save_directory;
// };

FileSaver::FileSaver(std::string sd) 
    : save_directory(sd) {
    pthread_mutex_init(&mutex, nullptr);
}

void FileSaver::save_file(std::string filename, const char *body, int body_size) {
    std::string final_filename;

    pthread_mutex_lock(&mutex);

    if (file_names_counter.count(filename) > 0) {
        int name_counter = ++file_names_counter[filename];
        final_filename = std::to_string(name_counter) + "_" + filename;
    } else {
        file_names_counter[filename] = 1;
        final_filename = filename;
    }

    pthread_mutex_unlock(&mutex);

    std::string save_path = "./" + save_directory + "/" + final_filename;

    std::ofstream file(save_path, std::ios::binary);
    if (file.is_open()) {
        file.write(body, body_size);
        file.close();
        std::cout << get_current_timestamp_with_ms() << ": file saved successfully: " << final_filename << " (" << body_size << " bytes)" << "\n";
    } else {
        std::cerr << get_current_timestamp_with_ms() << ": failed to open file for writing: " << final_filename << "\n";
    }
}