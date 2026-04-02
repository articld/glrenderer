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

set(STB_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libraries/stb_image")
add_library(stb_image STATIC
		${STB_IMAGE_DIR}/src/stb_image.cpp
)
target_include_directories(stb_image PUBLIC ${STB_IMAGE_DIR}/include)

include_directories(.)

add_executable(glrenderer
		src/main.cpp
		src/Shader.cpp
		src/Shader.h
		src/Camera.cpp
		src/Camera.h
		src/Mesh.cpp
		src/Mesh.h
)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(main PRIVATE assimp::assimp)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(glrenderer
		stb_image
		glfw
		glad
		opengl32
		glm::glm-header-only
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(glrenderer
		stb_image
		glfw
		glad
		GL
		glm::glm-header-only
	)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLFW_DIR}/include ${GLAD_DIR}/include
)
