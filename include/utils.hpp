#pragma once
#include <string>
#include <vector>

std::vector<uint8_t> read_file_bytes(const std::string& path);
void write_file_bytes(const std::string& path, const std::vector<uint8_t>& data);
std::string basename_no_ext(const std::string& path);
