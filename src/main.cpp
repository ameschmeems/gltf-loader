#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include "Window.hpp"
#include "spdlog/spdlog.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	spdlog::debug("Window resized: width={}, height={}", width, height);
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_Q:
			if (action == GLFW_PRESS)
			{
				GLint polygonMode[2] {};
				glGetIntegerv(GL_POLYGON_MODE, polygonMode);
				if (polygonMode[0] == GL_FILL)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
	}	
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
		window.setKeyCallback(keyCallback);

		std::ifstream is { "res/shaders/triangle.vert" };
		std::stringstream ss {};
		ss << is.rdbuf();
		std::string vertShaderStr { ss.str() };
		const char *vertShaderSource { vertShaderStr.c_str() };
		is.close();
		ss.str(std::string());
		
		GLuint vertShader { glCreateShader(GL_VERTEX_SHADER) };
		glShaderSource(vertShader, 1, &vertShaderSource, nullptr);
		glCompileShader(vertShader);

		GLint success {};
		char infoLog[512] {};
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
			spdlog::critical("Vertex Shader compilation failed: {}", infoLog);
			exit(-1);
		}
		spdlog::debug("Compiled Vertex Shader");

		is.open("res/shaders/triangle.frag");
		ss << is.rdbuf();
		std::string fragShaderStr { ss.str() };
		const char *fragShaderSource { fragShaderStr.c_str() };
		is.close();
		ss.str(std::string());
		
		GLuint fragShader { glCreateShader(GL_FRAGMENT_SHADER) };
		glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
			spdlog::critical("Fragment Shader compilation failed: {}", infoLog);
			exit(-1);
		}
		spdlog::debug("Compiled Fragment Shader");

		GLuint shaderProgram { glCreateProgram() };
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			spdlog::critical("Failed to link Shader Program: {}", infoLog);
			exit(-1);
		}
		spdlog::debug("Linked Shader Program");

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		float vertices[] {
			0.5f,  0.5f, 0.0f,  // top right
    		0.5f, -0.5f, 0.0f,  // bottom right
    		-0.5f, -0.5f, 0.0f,  // bottom left
    		-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] {
			0, 1, 3,
			1, 2, 3
		};

		GLuint vao {};
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vbo {};
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		GLuint ebo {};
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			nullptr
		);
		glEnableVertexAttribArray(0);

		while (!window.shouldClose())
		{
			// rendering
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shaderProgram);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

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