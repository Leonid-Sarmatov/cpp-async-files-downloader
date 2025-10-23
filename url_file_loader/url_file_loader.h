#ifndef _URL_FILE_LOADER
#define _URL_FILE_LOADER

#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

class URLInformation {
    public:
        URLInformation(std::string);

        std::string fullURL;  // http://localhost/eny/pipapupa.flmtr
        std::string hostname; // localhost
        std::string path;     // /eny/pipapupa.flmtr
        int port;             // 8080
        std::string scheme;   // http:// or https://
};

class HttpFileLoader {
    public:
        HttpFileLoader(URLInformation, std::string);
        void download_file();
    
    private:
        URLInformation info;
        std::string filename;
};

#endif