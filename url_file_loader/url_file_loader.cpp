#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include <algorithm> 

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include "../file_rw/file_rw.h"
#include "../logger/logger.h"

#define BUFFER_SIZE 1024

#include "url_file_loader.h"

// class URLInformation {
//     public:
//         std::string fullURL;  // http://localhost/pipapupa
//         std::string hostname; // localhost
//         std::string path;     // /pipapupa
//         std::string port;     // 8080

//         const char* getCharHostName();
//         const char* getCharPath();
//         const char* getCharHostNam();
    
//     private:
//         void remove_substring(std::string &str, std::string &sbstr);
//         void remove_http_https(std::string &str);
//         std::string search_file_name_in_url(std::string &str);
// }


URLInformation::URLInformation(std::string url, std::string p) {
    port = p;
    remove_http_https(url);
    search_file_name_in_url(url);
    hostname = url;
}

void URLInformation::remove_substring(std::string &str, std::string &sbstr) {
    std::size_t found = str.find(sbstr);
    if (found != std::string::npos) {
        str.erase(str.begin(), str.begin()+sbstr.size());
    }
}

void URLInformation::remove_http_https(std::string &str) {
    std::string https_str("https://");
    std::string http_str("http://");
    remove_substring(str, https_str);
    remove_substring(str, http_str);
}

void URLInformation::search_file_name_in_url(std::string &str) {
    while (str.size() > 0 && str.back() != '/') {
        path.push_back(str.back());
        str.pop_back();
    }
    if (str.size() > 0) {
        path.push_back(str.back());
        str.pop_back();
        reverse(path.begin(), path.end());
    }
}

// class FileLoader {
//     public:
//         const char* hostname;
//         const char* path;
//         const char* port;
//         const char* filename;
//         void download_file();
// }

FileLoader::FileLoader(URLInformation info, std::string fn) {
    hostname = strdup(info.hostname.c_str());
    path = strdup(info.path.c_str());
    port = strdup(info.port.c_str());
    filename = strdup(fn.c_str());

    //std::cout << hostname << ", " << path << ", " << port << ", " << filename << "\n";
}

void FileLoader::download_file() {
    char request[256];
    char buffer[BUFFER_SIZE];

    //std::cout << get_current_timestamp_with_ms() << ": hostname='" << hostname << "'   path='" << path << "'   filename='" << filename << "'\n";

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    size_t bytes_received;
    FILE *fp;

    snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, hostname);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to connect\n");
        return;
    }

    freeaddrinfo(servinfo);

    send(sockfd, request, strlen(request), 0);

    fp = fopen(filename, "wb");

    if (!fp) {
        perror("fopen");
        close(sockfd);
        return;
    }

    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, fp);
    }

    fclose(fp);
    close(sockfd);

    return;
}

// void remove_substring(std::string &str, std::string &sbstr)
// {
//     std::size_t found = str.find(sbstr);
//     if (found != std::string::npos)
//     {
//         str.erase(str.begin(), str.begin()+sbstr.size());
//     }
// }

// void remove_http_https(std::string &str)
// {
//     std::string https_str("https://");
//     std::string http_str("http://");

//     remove_substring(str, https_str);
//     remove_substring(str, http_str);
// }

// std::string search_file_name_in_url(std::string &str)
// {
//     std::string filename;
//     while (str.size() > 0 && str.back() != '/')
//     {
//         filename.push_back(str.back());
//         str.pop_back();
//     }
//     if (str.size() > 0)
//     {
//         filename.push_back(str.back());
//         str.pop_back();
//         reverse(filename.begin(), filename.end());
//     }
//     return filename;
// }

// int download_file(std::string cpphostname, std::string cpppath, std::string cppport, std::string cppfilename)
// {
//     std::cout << "download func: " << " hostname = " << cpphostname << ". path = " << cpppath << ", filename = " << cppfilename << "\n";
//     const char* hostname = string_to_char_pointer(cpphostname);
//     const char* path = string_to_char_pointer(cpppath);
//     const char* port = string_to_char_pointer(cppport);
//     const char* filename = string_to_char_pointer(cppfilename);
//     std::cout << "download func C str: " << " hostname = " << hostname << ". path = " << path << ", filename = " << filename << ", port = " << port <<"\n";

//     char request[256];
//     char buffer[BUFFER_SIZE];

//     int sockfd;
//     struct addrinfo hints, *servinfo, *p;
//     int rv;
//     size_t bytes_received;
//     FILE *fp;

//     snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, hostname);

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_STREAM;

//     if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//         return 1;
//     }

//     for (p = servinfo; p != NULL; p = p->ai_next) {
//         if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
//             perror("socket");
//             continue;
//         }

//         if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
//             close(sockfd);
//             perror("connect");
//             continue;
//         }
//         break;
//     }

//     if (p == NULL) {
//         fprintf(stderr, "Failed to connect\n");
//         return 2;
//     }

//     freeaddrinfo(servinfo);

//     send(sockfd, request, strlen(request), 0);

//     fp = fopen(filename, "wb");

//     if (!fp) {
//         perror("fopen");
//         close(sockfd);
//         return 3;
//     }

//     while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
//         fwrite(buffer, 1, bytes_received, fp);
//     }

//     fclose(fp);
//     close(sockfd);

//     return 0;
// }