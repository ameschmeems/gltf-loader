#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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

int main()
{
	try
	{
		#ifdef DEBUG
		spdlog::set_level(spdlog::level::debug);
		#endif

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		spdlog::debug("Initialized glfw");

		Window window { WINDOW_WIDTH, WINDOW_HEIGHT, "gltf-loader", NULL, NULL };

		window.makeContextCurrent();
		window.setFramebufferSizeCallback(framebufferSizeCallback);
		window.setKeyCallback(keyCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			spdlog::critical("Failed to initialize GLAD");
			exit(-1);
		}
		spdlog::debug("Initialized glad");

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		
		Shader shader { "res/shaders/triangle.vert", "res/shaders/triangle.frag" };
		
		Texture woodTexture { "res/textures/container.jpg", GL_TEXTURE0, GL_RGBA, GL_RGB };
		
		Texture faceTexture { "res/textures/awesomeface.png", GL_TEXTURE1 };

		float vertices[] {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
			8 * sizeof(float),
			nullptr
		);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)(3 * sizeof(float))
		);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)(6 * sizeof(float))
		);
		glEnableVertexAttribArray(2);

		shader.useProgram();
		shader.setUniform("woodTexture", 0);
		shader.setUniform("faceTexture", 1);

		while (!window.shouldClose())
		{
			// rendering
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			shader.useProgram();
			woodTexture.bind();
			faceTexture.bind();
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
	catch (Window::CreateWindowFailedException &e)
	{
		spdlog::critical("Exception thrown: {}", e.what());
		exit(-1);
	}
	catch (Shader::VertexShaderCompilationException &e)
	{
		spdlog::critical("Exception thrown: {}", e.what());
		exit(-1);
	}
	catch (Shader::FragmentShaderCompilationException &e)
	{
		spdlog::critical("Exception thrown: {}", e.what());
		exit(-1);
	}
	catch (Shader::ShaderProgramLinkingException &e)
	{
		spdlog::critical("Exception thrown: {}", e.what());
		exit(-1);
	}
	catch (Texture::TextureLoadingException &e)
	{
		spdlog::critical("Exception thrown: {}", e.what());
		exit(-1);
	}
}