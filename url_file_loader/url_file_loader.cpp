//#include <functional>
//#include <iostream>
//#include <optional>
//#include <stdexcept>
#include <string>
//#include <unordered_map>
#include <vector>
#include <fstream>

#include <algorithm> 

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <fstream>

#include "../file_rw/file_rw.h"
#include "../logger/logger.h"

#include "url_file_loader.h"

// class HttpFileLoader {
//     public:
//         HttpFileLoader(URLInformation, std::string);
//         void download_file();
    
//     private:
//         URLInformation info;
//         std::string filename;
// };


HttpFileLoader::HttpFileLoader(URLInformation _info, std::string _fn)
    : info(_info), filename(_fn) {
}

void HttpFileLoader::download_file() {
    std::cout << get_current_timestamp_with_ms() << ": hostname='" << info.hostname << "'   path='" << info.path << "'   filename='" << filename << "'\n";
    httplib::Client cli(info.hostname, 8080);

    cli.set_connection_timeout(0, 300000);
    cli.set_read_timeout(5, 0);
    cli.set_write_timeout(5, 0);
    cli.set_max_timeout(5000);

    httplib::Result res = cli.Get(info.path);
    
    if (res) {
        //std::cout << get_current_timestamp_with_ms() << ": error code: " << res.error() << "\n";
        std::cout << get_current_timestamp_with_ms() << ": file text = " << res->body.data() << "\n";
        std::cout << get_current_timestamp_with_ms() << ": " << res->status << "\n";
        std::cout << get_current_timestamp_with_ms() << ": " << res->get_header_value("Content-Type") << "\n";
        std::cout << get_current_timestamp_with_ms() << ": Response body size: " << res->body.size() << " bytes" << "\n"; // ДЛЯ ОТЛАДКИ

        std::ofstream file(filename, std::ios::binary);
        if (file.is_open()) {
            file.write(res->body.data(), res->body.size());
            file.close();
            std::cout << get_current_timestamp_with_ms() << ": file saved successfully: " << filename << " (" << res->body.size() << " bytes)" << "\n";
        } else {
            std::cerr << get_current_timestamp_with_ms() << ": failed to open file for writing: " << filename << "\n";
        }

        return;
    } else {
        std::cout << get_current_timestamp_with_ms() << ": " << "error: " << res.error() << "\n";
    }

    httplib::SSLClient sslcli(info.hostname, 443);
    sslcli.set_max_timeout(5000);
    sslcli.enable_server_certificate_verification(false);

    res = sslcli.Get(info.path);

    if (res) {
        //std::cout << get_current_timestamp_with_ms() << ": error code: " << res.error() << "\n";
        std::cout << get_current_timestamp_with_ms() << ": file text = " << res->body.data() << "\n";
        std::cout << get_current_timestamp_with_ms() << ": " << res->status << "\n";
        std::cout << get_current_timestamp_with_ms() << ": " << res->get_header_value("Content-Type") << "\n";
        std::cout << get_current_timestamp_with_ms() << ": Response body size: " << res->body.size() << " bytes" << "\n"; // ДЛЯ ОТЛАДКИ

        std::ofstream file(filename, std::ios::binary);
        if (file.is_open()) {
            file.write(res->body.data(), res->body.size());
            file.close();
            std::cout << get_current_timestamp_with_ms() << ": file saved successfully: " << filename << " (" << res->body.size() << " bytes)" << "\n";
        } else {
            std::cerr << get_current_timestamp_with_ms() << ": failed to open file for writing: " << filename << "\n";
        }

        return;
    } else {
        std::cout << get_current_timestamp_with_ms() << ": " << "error: " << res.error() << "\n";
    }

}

// class URLInformation {
//     public:
//         URLInformation(std::string);

//         std::string fullURL;  // http://localhost/eny/pipapupa.flmtr
//         std::string hostname; // localhost
//         std::string path;     // /eny/pipapupa.flmtr
//         int port;             // 8080
//         std::string scheme;   // http:// or https://
// };


URLInformation::URLInformation(std::string _url) {
    fullURL = _url;
    port = 8080;

    if (_url.find("https://") == 0) {
        scheme = "https";
        hostname = fullURL.substr(8); // Убираем "https://"
        port = 443;
    } else if (fullURL.find("http://") == 0) {
        scheme = "http";
        hostname = fullURL.substr(7); // Убираем "http://"
        port = 80;
    }

    size_t path_start = hostname.find('/');
    if (path_start != std::string::npos) {
        path = hostname.substr(path_start);
        hostname = hostname.substr(0, path_start);
    } else {
        path = "/";
    }
}