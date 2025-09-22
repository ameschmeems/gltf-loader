#include "Window.hpp"
#include <iostream>

/**
 * @brief Constructor for Window from existing GLFWwindow pointer
 * 
 * @param window Pointer to GLFWwindow
 */
Window::Window(GLFWwindow *window) : _window { window, glfwDestroyWindow }
{
}

/**
 * @brief Constructor for Window using glfwCreateWindow
 * 
 * @param width Window width
 * @param height Window height
 * @param title Window title
 * @param monitor The monitor to use for full screen mode, or NULL for windowed mode.
 * @param share The window whose context to share resources with, or NULL to not share resources.
 */
Window::Window(const int width, const int height, const std::string title, GLFWmonitor *monitor, GLFWwindow *share) : 
	_window { glfwCreateWindow(width, height, title.c_str(), monitor, share), glfwDestroyWindow }
{
	if (!_window.get())
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
}

/**
 * @brief Move constructor for Window
 */
Window::Window(Window &&rhs) : _window { std::move(rhs._window) }
{
}

/**
 * @brief Move assignment operator for Window
 */
Window &Window::operator=(Window &&rhs)
{
	_window = std::move(rhs._window);

	return *this;
}

/**
 * @brief Get raw underlying GLFWwindow
 * 
 * @returns raw pointer to GLFWwindow
 */
GLFWwindow *Window::getWindowPtr() const
{
	return _window.get();
}
