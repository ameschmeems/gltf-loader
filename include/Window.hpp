#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

/**
 * @class Window Window.hpp "include/Window.hpp"
 * @brief A wrapper around GLFWwindow
 */
class Window
{
public:

	Window() = delete;
	Window(GLFWwindow *window);
	Window(const int width, const int height, std::string title, GLFWmonitor *monitor, GLFWwindow *share);
	Window(Window &rhs) = delete;
	Window(Window &&rhs);
	~Window() = default;

	Window &operator=(Window &rhs) = delete;
	Window &operator=(Window &&rhs);

	GLFWwindow *getWindowPtr() const;

private:

	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> _window;
};