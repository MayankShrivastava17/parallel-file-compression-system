#include "huffman.hpp"
#include <array>
#include <queue>
#include <stdexcept>
#include <bitset>

struct PQItem { uint32_t freq; int idx; bool operator>(const PQItem& o) const { return freq > o.freq; } };

void HuffmanCodec::build_tree(const std::array<uint32_t,256>& freq,
                              std::vector<Node>& nodes, int& root) {
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
    nodes.clear();
    for (int i=0;i<256;++i) if (freq[i]) {
        nodes.push_back({freq[i], -1, -1, (uint16_t)i});
        pq.push({freq[i], (int)nodes.size()-1});
    }
    if (pq.empty()) { root = -1; return; }
    if (pq.size()==1) {
        // duplicate leaf to avoid edge cases
        auto single = pq.top(); pq.pop();
        nodes.push_back({nodes[single.idx].freq, -1, -1, nodes[single.idx].symbol});
        int a = single.idx, b = (int)nodes.size()-1;
        nodes.push_back({nodes[a].freq + nodes[b].freq, a, b, 0});
        root = (int)nodes.size()-1;
        return;
    }
    while (pq.size()>1) {
        auto a=pq.top(); pq.pop();
        auto b=pq.top(); pq.pop();
        nodes.push_back({a.freq + b.freq, a.idx, b.idx, 0});
        pq.push({a.freq + b.freq, (int)nodes.size()-1});
    }
    root = pq.top().idx;
}

void HuffmanCodec::build_codes(int idx, const std::vector<Node>& nodes,
                               std::vector<std::vector<bool>>& codes,
                               std::vector<bool>& cur) {
    const auto& n = nodes[idx];
    if (n.left==-1 && n.right==-1) {
        codes[n.symbol] = cur;
        return;
    }
    cur.push_back(false);
    build_codes(n.left, nodes, codes, cur);
    cur.back() = true;
    build_codes(n.right, nodes, codes, cur);
    cur.pop_back();
}

// file format (very simple, not optimal):
// [magic 'HUF1'][256x uint32 freq][bitlen uint32][packed bits...]
std::vector<uint8_t> HuffmanCodec::compress(const std::vector<uint8_t>& input) {
    std::array<uint32_t,256> freq{}; for (auto b: input) freq[b]++;
    std::vector<Node> nodes; int root=-1;
    build_tree(freq, nodes, root);

    std::vector<std::vector<bool>> codes(256);
    std::vector<bool> cur;
    if (root != -1) build_codes(root, nodes, codes, cur);

    std::vector<uint8_t> out;
    out.insert(out.end(), {'H','U','F','1'});
    // write freq table
    for (int i=0;i<256;++i) {
        uint32_t f = freq[i];
        for (int k=0;k<4;++k) out.push_back((f >> (8*k)) & 0xFF);
    }

    // pack bits
    std::vector<bool> bits;
    bits.reserve(input.size()*2);
    for (auto b: input) {
        auto& code = codes[b];
        bits.insert(bits.end(), code.begin(), code.end());
    }
    uint32_t bitlen = (uint32_t)bits.size();
    for (int k=0;k<4;++k) out.push_back((bitlen >> (8*k)) & 0xFF);

    uint8_t acc=0; int nb=0;
    for (bool bit: bits) {
        acc |= (bit?1:0) << (nb++);
        if (nb==8) { out.push_back(acc); acc=0; nb=0; }
    }
    if (nb) out.push_back(acc);
    return out;
}

std::vector<uint8_t> HuffmanCodec::decompress(const std::vector<uint8_t>& input) {
    if (input.size() < 4 + 256*4 + 4) throw std::runtime_error("bad huffman stream");
    if (!(input[0]=='H' && input[1]=='U' && input[2]=='F' && input[3]=='1'))
        throw std::runtime_error("bad magic");
    size_t off=4;
    std::array<uint32_t,256> freq{};
    for (int i=0;i<256;++i) {
        uint32_t f=0; for (int k=0;k<4;++k) f |= (uint32_t)input[off++] << (8*k);
        freq[i]=f;
    }
    uint32_t bitlen=0; for (int k=0;k<4;++k) bitlen |= (uint32_t)input[off++] << (8*k);

    std::vector<Node> nodes; int root=-1;
    build_tree(freq, nodes, root);
    if (root==-1) return {};

    std::vector<uint8_t> out;
    int idx = root;
    uint32_t read_bits=0;
    for (; off < input.size() && read_bits<bitlen; ++off) {
        uint8_t byte = input[off];
        for (int i=0; i<8 && read_bits<bitlen; ++i, ++read_bits) {
            bool bit = ((byte >> i) & 1);
            idx = bit ? nodes[idx].right : nodes[idx].left;
            if (nodes[idx].left==-1 && nodes[idx].right==-1) {
                out.push_back((uint8_t)nodes[idx].symbol);
                idx = root;
            }
        }
    }
    return out;
}
