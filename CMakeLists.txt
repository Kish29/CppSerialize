cmake_minimum_required(VERSION 3.23)
project(CppSerialize)

include_directories(serialize)
set(CMAKE_CXX_STANDARD 14)

add_executable(CppSerialize main.cpp serialize/DataStream.h serialize/Serialize.h)
