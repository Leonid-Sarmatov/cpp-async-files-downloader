#ifndef _URL_FILE_LOADER
#define _URL_FILE_LOADER

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

// int download_file(std::string hostname, std::string path, std::string port, std::string filename);

// void remove_http_https(std::string &str);

// std::string search_file_name_in_url(std::string &str);

class URLInformation {
    public:
        std::string fullURL;  // http://localhost/pipapupa
        std::string hostname; // localhost
        std::string path;     // /pipapupa
        std::string port;     // 8080

        URLInformation(std::string, std::string);
    
    private:
        void remove_substring(std::string &str, std::string &sbstr);
        void remove_http_https(std::string &str);
        void search_file_name_in_url(std::string &str);
};

class FileLoader {
    public:
        const char* hostname;
        const char* path;
        const char* port;
        const char* filename;

        FileLoader(URLInformation, std::string);
        void download_file();
};

#endif