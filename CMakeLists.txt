cmake_minimum_required(VERSION 4.1)
project(glrenderer)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/libraries/glfw-3.4")
add_subdirectory(${GLFW_DIR})

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/libraries/glad")
add_library(glad STATIC
        ${GLAD_DIR}/src/glad.c
)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

include_directories(.)

add_executable(glrenderer
        main.cpp
        Shader.cpp
        Shader.h)

target_link_libraries(glrenderer
        glfw
        glad
        opengl32
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLFW_DIR}/include ${GLAD_DIR}/include
)
