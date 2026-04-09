cmake_minimum_required(VERSION 4.1)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/Lenovo/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
if(WIN32)
	set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
elseif(UNIX)
	set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

project(glrenderer)

set(CMAKE_CXX_STANDARD 20)

set(STB_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libraries/stb_image")
add_library(stb_image STATIC
		${STB_IMAGE_DIR}/src/stb_image.cpp
)
target_include_directories(stb_image PUBLIC ${STB_IMAGE_DIR}/include)

include_directories(.)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(glrenderer
		src/main.cpp
		src/Shader.cpp
		src/Shader.h
		src/Camera.cpp
		src/Camera.h
		src/ModelImport/Mesh.cpp
		src/ModelImport/Mesh.h
		src/ModelImport/Model.cpp
		src/ModelImport/Model.h
		src/primitives/plane.h
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(glrenderer PRIVATE
		stb_image
		glfw
		glad::glad
		opengl32
		glm::glm-header-only
		assimp::assimp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(glrenderer PRIVATE
		stb_image
		glfw
		glad::glad
		GL
		glm::glm-header-only
		assimp::assimp
	)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLFW_DIR}/include ${GLAD_DIR}/include
)
