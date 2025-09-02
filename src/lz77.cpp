#include "lz77.hpp"
#include <stdexcept>

std::vector<uint8_t> LZ77::compress(const std::vector<uint8_t>& in, size_t window, size_t max_len) {
    std::vector<uint8_t> out;
    out.insert(out.end(), {'L','Z','7','7'});
    size_t i=0;
    while (i < in.size()) {
        size_t best_off=0, best_len=0;
        size_t start = (i > window) ? i - window : 0;
        for (size_t j=start; j<i; ++j) {
            size_t k=0;
            while (i+k < in.size() && j+k < i && in[j+k]==in[i+k] && k<max_len) ++k;
            if (k > best_len) { best_len = k; best_off = i - j; }
        }
        uint8_t literal = in[i + best_len < in.size() ? i + best_len : i];
        // write triple: offset(2 bytes), length(1), literal(1)
        out.push_back((uint8_t)(best_off & 0xFF));
        out.push_back((uint8_t)((best_off >> 8) & 0xFF));
        out.push_back((uint8_t)best_len);
        out.push_back(literal);
        i += (best_len ? best_len + 1 : 1);
    }
    return out;
}

std::vector<uint8_t> LZ77::decompress(const std::vector<uint8_t>& in) {
    if (in.size() < 4) throw std::runtime_error("bad lz77 stream");
    if (!(in[0]=='L' && in[1]=='Z' && in[2]=='7' && in[3]=='7'))
        throw std::runtime_error("bad magic");
    std::vector<uint8_t> out;
    size_t i=4;
    while (i + 4 <= in.size()) {
        uint16_t off = (uint16_t)in[i] | ((uint16_t)in[i+1] << 8);
        uint8_t len = in[i+2];
        uint8_t lit = in[i+3];
        i += 4;
        if (off>0 && len>0) {
            if (off > out.size()) throw std::runtime_error("invalid offset");
            size_t start = out.size() - off;
            for (size_t k=0; k<len; ++k) out.push_back(out[start+k]);
        }
        out.push_back(lit);
    }
    return out;
}
