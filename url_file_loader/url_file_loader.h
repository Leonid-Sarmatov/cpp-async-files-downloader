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

class URLInformation {
    public:
        URLInformation(std::string, std::string);

        std::string fullURL;  // http://localhost/pipapupa
        std::string hostname; // localhost
        std::string path;     // /pipapupa
        std::string port;     // 8080
    
    private:
        void remove_substring(std::string &str, std::string &sbstr);
        void remove_http_https(std::string &str);
        void search_file_name_in_url(std::string &str);
};

class FileLoader {
    public:
        FileLoader(URLInformation, std::string);
        void download_file();
    
    private:
        const char* hostname;
        const char* path;
        const char* port;
        const char* filename;
};

#endif