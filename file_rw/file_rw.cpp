#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "file_rw.h"

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
    for (int i = 0; i < lines.size(); i++)
    {
        std::cout << i << ". " << lines[i] << "\n";
    }
    std::cout << "\n";
}

std::string char_pointer_to_string(const char* str)
{
    std::string res_string;
    for (int i = 0; str[i] != '\0'; i++) {
        res_string.push_back(str[i]);
    }
    return res_string;
}

const char* string_to_char_pointer(std::string &str)
{
    //std::cout << "\nstrong to char func = " << str << "\n";
    //std::cout << "strong to char func C res = " << str.c_str() << "\n\n";
    // int i;
    // for (i = 0; i < str.size(); i++)
    // {
    //     result[i] = str.at(i);
    // }
    // i++;
    // result[i] = str.at('\0');
    return str.c_str();
}
