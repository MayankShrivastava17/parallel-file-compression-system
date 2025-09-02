#include "utils.hpp"
#include <fstream>
#include <stdexcept>

std::vector<uint8_t> read_file_bytes(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) throw std::runtime_error("Failed to open: " + path);
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<uint8_t> buf(size);
    ifs.read(reinterpret_cast<char*>(buf.data()), size);
    return buf;
}

void write_file_bytes(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open for write: " + path);
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::string basename_no_ext(const std::string& path) {
    auto slash = path.find_last_of("/\\");
    auto dot = path.find_last_of('.');
    size_t start = (slash == std::string::npos) ? 0 : slash + 1;
    if (dot == std::string::npos || dot < start) dot = path.size();
    return path.substr(start, dot - start);
}
