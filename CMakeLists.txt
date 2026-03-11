cmake_minimum_required(VERSION 4.1)
project(glrenderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(glrenderer
        glad.c
        main.cpp
        Shader.cpp
        Shader.h)
