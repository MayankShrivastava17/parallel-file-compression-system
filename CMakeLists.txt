cmake_minimum_required(VERSION 3.14)
project(parallel_compressor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

add_library(pcompress
    src/huffman.cpp
    src/lz77.cpp
    src/thread_pool.cpp
    src/utils.cpp
    src/compressor.cpp
)

target_include_directories(pcompress PUBLIC include)

add_executable(parallel_compressor src/main.cpp)
target_link_libraries(parallel_compressor PRIVATE pcompress)
