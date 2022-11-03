include (cmake/list_extension.cmake)

cmake_minimum_required(VERSION 3.18)
project(mo_test LANGUAGES CXX)

# cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
# 윈도우에서는 빌드툴 생성할 때 32비트 설정을 해줘야함

# https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(third_party/googletest)
add_subdirectory(src)

