#include "compressor.hpp"
#include "huffman.hpp"
#include "lz77.hpp"
#include "utils.hpp"
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

static bool is_huffman(const std::vector<uint8_t>& v) {
    return v.size()>=4 && v[0]=='H' && v[1]=='U' && v[2]=='F' && v[3]=='1';
}
static bool is_lz77(const std::vector<uint8_t>& v) {
    return v.size()>=4 && v[0]=='L' && v[1]=='Z' && v[2]=='7' && v[3]=='7';
}

Result Compressor::compress_file(const std::string& in_path, const std::string& out_dir, Algo algo) {
    auto data = read_file_bytes(in_path);
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<uint8_t> out_data;
    if (algo == Algo::HUFFMAN) {
        HuffmanCodec h; out_data = h.compress(data);
    } else {
        LZ77 l; out_data = l.compress(data);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double secs = std::chrono::duration<double>(end - start).count();
    fs::create_directories(out_dir);

    std::string base = basename_no_ext(in_path);
    std::string ext = (algo==Algo::HUFFMAN) ? ".huf" : ".lz77";
    std::string out_path = (fs::path(out_dir) / (base + ext)).string();
    write_file_bytes(out_path, out_data);

    return {in_path, out_path, data.size(), out_data.size(), secs};
}

Result Compressor::decompress_file(const std::string& in_path, const std::string& out_dir) {
    auto data = read_file_bytes(in_path);
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<uint8_t> out_data;
    if (is_huffman(data)) {
        HuffmanCodec h; out_data = h.decompress(data);
    } else if (is_lz77(data)) {
        LZ77 l; out_data = l.decompress(data);
    } else {
        throw std::runtime_error("unknown format: " + in_path);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double secs = std::chrono::duration<double>(end - start).count();
    fs::create_directories(out_dir);

    std::string base = basename_no_ext(in_path);
    std::string out_path = (fs::path(out_dir) / (base + ".out")).string();
    write_file_bytes(out_path, out_data);

    return {in_path, out_path, data.size(), out_data.size(), secs};
}
