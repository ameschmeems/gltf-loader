#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

/**
 * @class Texture Texture.hpp "include/Texture.hpp"
 * @brief A class encapsulating an OpenGL 2D texture
 */
class Texture
{
public:

	Texture() = delete;
	Texture(const Texture &rhs) = default;
	Texture(Texture &&rhs) = default;
	Texture(const std::string &path, GLenum target, GLint internalFormat=GL_RGBA, GLenum format=GL_RGBA, GLenum type=GL_UNSIGNED_BYTE, bool flipVertically=true);
	~Texture() = default;

	Texture &operator=(const Texture &rhs) = default;
	Texture &operator=(Texture &&rhs) = default;

	void bind();

	class TextureLoadingException : public std::exception
	{
	public:
		TextureLoadingException(const std::string &message) throw();
		const char *what() const throw();

	private:
		const std::string _message;
	};

private:

	GLuint _id {};
	GLenum _target {};
	int _width {};
	int _height {};
	int _nrChannels{};
};