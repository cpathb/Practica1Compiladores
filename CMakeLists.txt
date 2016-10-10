cmake_minimum_required(VERSION 2.8.4)
project(Practica1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c")

set(SOURCE_FILES main.c)
add_executable(dbsg ${SOURCE_FILES})