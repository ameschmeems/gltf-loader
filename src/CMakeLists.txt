add_executable(app)

target_sources(app
	PRIVATE
		main.cpp
)

find_package(OpenGL REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(app PUBLIC DEBUG)
endif()

target_link_libraries(app PRIVATE core glad glfw OpenGL::GL glm spdlog stb_image)

add_custom_command(TARGET app PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${PROJECT_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:app>/res)