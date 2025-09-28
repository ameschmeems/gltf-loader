#include "Window.hpp"
#include "spdlog/spdlog.h"

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
 * 
 * @throws CreateWindowFailedException if glfwCreateWindow fails
 */
Window::Window(const int width, const int height, const std::string title, GLFWmonitor *monitor, GLFWwindow *share) : 
	_window { glfwCreateWindow(width, height, title.c_str(), monitor, share), glfwDestroyWindow }
{
	if (!_window.get())
	{
		throw CreateWindowFailedException();
	}
	spdlog::debug("Created new window: width={}, height={}, title={}", width, height, title);
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
GLFWwindow *Window::getWindowPtr()
{
	return _window.get();
}

/**
 * @brief Makes the context of the specified window current for the calling thread, using glfwMakeContextCurrent.
 */
void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(_window.get());
}

/**
 * @brief Sets framebuffer size callback for window using glfwSetFramebufferSizeCallback.
 * 
 * @param callback Function pointer to desired callback method
 * 
 * @returns The previously set callback, or NULL if no callback was set or glfw had not been initialized.
 */
GLFWframebuffersizefun Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback)
{
	return glfwSetFramebufferSizeCallback(_window.get(), callback);
}

/**
 * @brief Sets key callback for window using glfwSetKeyCallback
 * 
 * @param callback Function pointer to desired callback method
 * 
 * @returns The previously set callback, or NULL if not callback was set or glfw had not been initialized.
 */
GLFWkeyfun Window::setKeyCallback(GLFWkeyfun callback)
{
	return glfwSetKeyCallback(_window.get(), callback);
}

/**
 * @brief Checks the close flag of the window
 * 
 * @returns true if window should close, otherwise false
 */
bool Window::shouldClose()
{
	return glfwWindowShouldClose(_window.get());
}

/**
 * @brief Sets the close flag of the window
 * 
 * @param value The desired value of the close flag
 */
void Window::setWindowShouldClose(bool value)
{
	glfwSetWindowShouldClose(_window.get(), value);
}

/**
 * @brief Checks the last reported state of a key for the window.
 * 
 * @param key The desired key for which state should be checked
 * 
 * @returns GLFW_PRESS or GLFW_RELEASE
 */
int Window::getKey(int key)
{
	return glfwGetKey(_window.get(), key);
}

/**
 * @brief Swaps the front and back buffers of the window.
 */
void Window::swapBuffers()
{
	glfwSwapBuffers(_window.get());
}

const char *Window::CreateWindowFailedException::what() const throw()
{
	return "Failed to create GLFW window";
}
