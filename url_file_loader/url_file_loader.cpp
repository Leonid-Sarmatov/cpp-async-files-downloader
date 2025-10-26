#include <string>
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
#include <regex>

#include "../file_rw/file_rw.h"
#include "../logger/logger.h"

#include "url_file_loader.h"

// class HttpFileLoader {
//     public:
//         HttpFileLoader(URLInformation, FileSaver&);
//         void download_file();
    
//     private:
//         URLInformation info;
//         FileSaver &saver;
//         std::string filename;

//         template<typename ClientType>
//         void write_file(ClientType &client);
// };


HttpFileLoader::HttpFileLoader(URLInformation _info, FileSaver &_saver)
    : info(_info), saver(_saver) {
}

void HttpFileLoader::download_file() {
    std::cout << get_current_timestamp_with_ms() << ": hostname='" << info.hostname << "'   path='" << info.path << "'   filename='" << filename << "'\n";

    if (info.scheme == "https") {
        httplib::SSLClient sslcli(info.hostname, 443);
        sslcli.enable_server_certificate_verification(false);
        write_file(sslcli);
    } else {
        httplib::Client cli(info.hostname, 8080);
        write_file(cli);
    }
}

template<typename ClientType>
void HttpFileLoader::write_file(ClientType &client) {
    // Таймауты соединения
    client.set_connection_timeout(0, 300000);
    client.set_read_timeout(5, 0);
    client.set_write_timeout(5, 0);
    client.set_max_timeout(5000);

    httplib::Result res = client.Get(info.path);
    
    if (res) {
        std::cout << get_current_timestamp_with_ms() << ": response code: " << res->status << "\n";

        // Если задан заголовок Content-Disposition с типом attachment, то извлекаем имя файла из него
        std::string name_header = res->get_header_value("Content-Disposition");
        if (name_header.find("attachment") != std::string::npos) {
            std::regex pattern("filename\\s*=\\s*\"?([^\"]*)\"?");
            std::smatch matches;
            if (std::regex_search(name_header, matches, pattern) && matches.size() > 0) {
                info.filename = matches[1].str();
            }
        }

        // Создание и запись в файл тела сообщения
        saver.save_file(info.filename, res->body.data(), res->body.size());

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
//         std::string filename; // pipapupa.flmtr
// };


URLInformation::URLInformation(std::string _url) {
    fullURL = _url;
    port = 8080;

    // Обрезка http:// или https://, сохранение протокла и порта в поля класса
    if (_url.find("https://") == 0) {
        scheme = "https";
        hostname = fullURL.substr(8); // Убираем "https://"
        port = 443;
    } else if (fullURL.find("http://") == 0) {
        scheme = "http";
        hostname = fullURL.substr(7); // Убираем "http://"
        port = 80;
    }

    // Извлечение из URL и сохранение полного пути к файлу
    size_t path_start = hostname.find('/');
    if (path_start != std::string::npos) {
        path = hostname.substr(path_start);
        hostname = hostname.substr(0, path_start);
    } else {
        path = "/";
    }

    // Извлечение из пути и сохрание имени файла
    size_t last_slash = fullURL.find_last_of('/');
    if (last_slash != std::string::npos) {
        filename = fullURL.substr(last_slash + 1);
    } else {
        filename = "";
    }
}