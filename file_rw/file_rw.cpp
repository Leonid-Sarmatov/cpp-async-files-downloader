#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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
