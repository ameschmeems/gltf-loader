#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "Window.hpp"
#include "spdlog/spdlog.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	spdlog::debug("Window resized: width={}, height={}", width, height);
	glViewport(0, 0, width, height);
}

void processInput(Window &window)
{
	if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		window.setWindowShouldClose(true);
}

int main() {

	#ifdef DEBUG
	spdlog::set_level(spdlog::level::debug);
	#endif

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	spdlog::debug("Initialized glfw");

	try {
		Window window { WINDOW_WIDTH, WINDOW_HEIGHT, "gltf-loader", NULL, NULL };

		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			spdlog::critical("Failed to initialize GLAD");
			exit(-1);
		}

		spdlog::debug("Initialized glad");

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		window.setFramebufferSizeCallback(framebufferSizeCallback);

		while (!window.shouldClose())
		{
			// input
			processInput(window);

			// rendering
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// check and call events and swap buffers
			window.swapBuffers();
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
	catch (Window::CreateWindowFailedException e)
	{
		spdlog::critical(e.what());
		exit(-1);
	}
}