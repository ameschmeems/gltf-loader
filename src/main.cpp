#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float fov { 45.0f };

glm::vec3 cameraPos { 0.0f, 0.0f, 3.0f };
glm::vec3 cameraFront { 0.0f, 0.0f, -1.0f };
glm::vec3 cameraUp { 0.0f, 1.0f, 0.0f };

float deltaTime { 0.0f };
float lastFrame { 0.0f };

bool firstMouse { true };
float lastMouseX { WINDOW_WIDTH / 2.0f };
float lastMouseY { WINDOW_HEIGHT / 2.0f };

float yaw { -90.0f };
float pitch { 0.0f };

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
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS)
			{
				if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				else
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			break;
	}
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
	{
		firstMouse = true;
		return;
	}
	
	if (firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;
	}
	float xOffset { static_cast<float>(xPos - lastMouseX) };
	float yOffset { static_cast<float>(yPos - lastMouseY) };
	lastMouseX = xPos;
	lastMouseY = yPos;

	const float sensitivity { 0.1f };
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch -= yOffset;
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction {
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	};

	cameraFront = glm::normalize(direction);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	fov -= static_cast<float>(yOffset);
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f);
		fov = 45.0f;
}

// Doing this in keyCallback makes the movement feel laggy
void processMovementInput(Window &window)
{
	float cameraSpeed { 2.5f * deltaTime };
	if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
		window.setCursorPosCallback(mouseCallback);
		window.setScrollCallback(scrollCallback);

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

		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
			5 * sizeof(float),
			nullptr
		);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(float),
			(void*)(3 * sizeof(float))
		);
		glEnableVertexAttribArray(1);

		shader.useProgram();
		shader.setUniform("woodTexture", 0);
		shader.setUniform("faceTexture", 1);

		while (!window.shouldClose())
		{
			// input
			float currentFrame { static_cast<float>(glfwGetTime()) };
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processMovementInput(window);

			// rendering
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader.useProgram();
			woodTexture.bind();
			faceTexture.bind();
			glBindVertexArray(vao);
			glm::mat4 model { 1.0f };
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glm::mat4 view {
				glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)
			};
			glm::mat4 projection { glm::perspective(glm::radians(fov), static_cast<float>(WINDOW_WIDTH)/static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f) };
			shader.setUniform("model", model);
			shader.setUniform("view", view);
			shader.setUniform("projection", projection);
			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			// check and call events and swap buffers
			window.swapBuffers();
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
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