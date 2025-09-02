#pragma once
#include <string>
#include <vector>

enum class Algo { HUFFMAN, LZ77 };

struct Result {
    std::string input_path;
    std::string output_path;
    size_t in_bytes{0};
    size_t out_bytes{0};
    double seconds{0.0};
};

class Compressor {
public:
    static Result compress_file(const std::string& in_path, const std::string& out_dir, Algo algo);
    static Result decompress_file(const std::string& in_path, const std::string& out_dir);
};
