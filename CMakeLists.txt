cmake_minimum_required(VERSION 3.18)
project(mo_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(third_party/googletest)
add_subdirectory(src)

