#ifndef _FILE_RW
#define _FILE_RW

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

std::vector<std::string> get_file_lines(std::string file_name);

void print_file_lines(std::vector<std::string> lines);

std::string char_pointer_to_string(const char* str);

const char* string_to_char_pointer(std::string &str);

#endif