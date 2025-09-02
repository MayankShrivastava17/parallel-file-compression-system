#include "compressor.hpp"
#include "thread_pool.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage:\n"
                  << "  parallel_compressor compress <huffman|lz77> <out_dir> <file1> [file2...]\n"
                  << "  parallel_compressor decompress <out_dir> <file1> [file2...]\n";
        return 1;
    }

    std::string mode = argv[1];
    ThreadPool pool(std::thread::hardware_concurrency());

    if (mode == "compress") {
        std::string alg_s = argv[2];
        Algo algo = (alg_s == "huffman") ? Algo::HUFFMAN : Algo::LZ77;
        std::string out_dir = argv[3];
        std::vector<std::future<Result>> futs;
        for (int i=4; i<argc; ++i) {
            std::string path = argv[i];
            futs.push_back(pool.enqueue([=]{ return Compressor::compress_file(path, out_dir, algo); }));
        }
        std::cout << "input_path,output_path,in_bytes,out_bytes,seconds\n";
        for (auto& f : futs) {
            auto r = f.get();
            std::cout << r.input_path << "," << r.output_path << ","
                      << r.in_bytes << "," << r.out_bytes << "," << r.seconds << "\n";
        }
    } else if (mode == "decompress") {
        std::string out_dir = argv[2];
        std::vector<std::future<Result>> futs;
        for (int i=3; i<argc; ++i) {
            std::string path = argv[i];
            futs.push_back(pool.enqueue([=]{ return Compressor::decompress_file(path, out_dir); }));
        }
        std::cout << "input_path,output_path,in_bytes,out_bytes,seconds\n";
        for (auto& f : futs) {
            auto r = f.get();
            std::cout << r.input_path << "," << r.output_path << ","
                      << r.in_bytes << "," << r.out_bytes << "," << r.seconds << "\n";
        }
    } else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }
    return 0;
}
