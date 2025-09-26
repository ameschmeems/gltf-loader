cmake_minimum_required(VERSION 3.23)
project(
	gltf-loader
	VERSION 0.1
	DESCRIPTION ".gltf model loader"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
	message("Build type not set, defaulting to debug")
	set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()
include(GoogleTest)

add_subdirectory(extern/spdlog)
add_subdirectory(extern/glm)
add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(extern/stb_image)
add_subdirectory(test)

find_package(Doxygen)

if (${DOXYGEN_FOUND})
	add_custom_target(gltf-loader-docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
endif()