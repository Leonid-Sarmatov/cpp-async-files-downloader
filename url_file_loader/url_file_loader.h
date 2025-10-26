#ifndef _URL_FILE_LOADER
#define _URL_FILE_LOADER

#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "../file_rw/file_rw.h"

class URLInformation {
    public:
        URLInformation(std::string);

        std::string fullURL;  // http://localhost/eny/pipapupa.flmtr
        std::string hostname; // localhost
        std::string path;     // /eny/pipapupa.flmtr
        int port;             // 8080
        std::string scheme;   // http:// or https://
        std::string filename; // pipapupa.flmtr
};

class HttpFileLoader {
    public:
        HttpFileLoader(URLInformation, FileSaver&);
        void download_file();
    
    private:
        URLInformation info;
        FileSaver &saver;
        std::string filename;

        template<typename ClientType>
        void write_file(ClientType &client);
};

#endif