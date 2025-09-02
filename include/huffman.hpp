#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

struct HuffmanCodec {
    std::vector<uint8_t> compress(const std::vector<uint8_t>& input);
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& input);
private:
    struct Node {
        uint32_t freq;
        int16_t left, right; 
        uint16_t symbol;    
    };
    void build_tree(const std::array<uint32_t,256>& freq,
                    std::vector<Node>& nodes, int& root);
    void build_codes(int idx, const std::vector<Node>& nodes,
                     std::vector<std::vector<bool>>& codes,
                     std::vector<bool>& cur);
};
