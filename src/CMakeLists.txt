add_executable(gltf-loader)

target_sources(gltf-loader
	PRIVATE
		main.cpp
)

find_package(OpenGL REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(gltf-loader PUBLIC DEBUG)
endif()

target_link_libraries(gltf-loader PRIVATE core glad glfw OpenGL::GL glm spdlog stb_image)

add_custom_command(TARGET gltf-loader PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${PROJECT_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:gltf-loader>/res)