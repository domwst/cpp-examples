cmake_minimum_required(VERSION 3.22)
project(cpp_examples)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

add_executable(lecture-1 lectures/lecture-1.cpp)
