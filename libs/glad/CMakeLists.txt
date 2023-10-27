cmake_minimum_required(VERSION 3.16...3.28)
project(gl LANGUAGES C)

add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
