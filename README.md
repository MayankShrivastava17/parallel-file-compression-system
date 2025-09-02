# Parallel File Compression System

This project was developed as a **capstone for EE 669**.
It implements a **parallel file compression tool** in C++ using **Huffman coding** and **LZ77**, with a Python CLI and visualization utilities.

---

## Build & Run

### 1. Build the C++ Project

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
```

### 2. Compress Files

```bash
# Huffman
./parallel_compressor compress huffman ../out ../data/sample.txt

# LZ77
./parallel_compressor compress lz77 ../out ../data/sample.txt
```

### 3. Decompress Files

```bash
./parallel_compressor decompress ../out ../out/sample.huf
./parallel_compressor decompress ../out ../out/sample.lz77
```

### 4. Python CLI Wrapper

```bash
cd python
python3 cli.py compress --algo huffman --out-dir ../out ../data/sample.txt
python3 cli.py compress --algo lz77 --out-dir ../out ../data/sample.txt
```

### 5. Visualization

```bash
python3 visualize.py --csv ../out/metrics_<timestamp>.csv
```

---

## Notes

* All compressed files are saved in the `out/` folder.
* Decompressed files are suffixed with `.out`.
* Use the Python visualization script to compare **compression ratios** and **runtime** across workloads.

---
