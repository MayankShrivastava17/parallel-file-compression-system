#pragma once
#include <cstdint>
#include <vector>

struct LZ77 {
    std::vector<uint8_t> compress(const std::vector<uint8_t>& input,
                                  size_t window=4096, size_t max_len=18);
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& input);
};
