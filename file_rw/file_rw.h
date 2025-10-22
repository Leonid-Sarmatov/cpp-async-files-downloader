#ifndef _FILE_RW
#define _FILE_RW

#include <vector>

std::vector<std::string> get_file_lines(std::string file_name);

void print_file_lines(std::vector<std::string> lines);


#endif