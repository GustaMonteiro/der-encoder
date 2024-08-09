cmake_minimum_required(VERSION 3.15)
project("der_encoder")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(der_encoder der_encoder.cpp make_buffer_string.cpp main.cpp)