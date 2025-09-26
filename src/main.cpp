#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "Window.hpp"

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(Window &window)
{
	if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		window.setWindowShouldClose(true);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	try {
		Window window { 800, 600, "glfw-loader", NULL, NULL };

		window.makeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(-1);
		}

		glViewport(0, 0, 800, 600);

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
		std::cerr << e.what() << std::endl;
		exit(-1);
	}
}