add_executable(app)

target_sources(app
	PRIVATE
		main.cpp
		# stb_image.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(app PRIVATE core glad glfw OpenGL::GL glm)

# add_custom_command(TARGET app PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                        ${PROJECT_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:app>/res)