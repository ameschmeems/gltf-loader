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
	Texture(const std::string &path, const std::string &typeName, GLenum type=GL_UNSIGNED_BYTE, bool flipVertically=true);

	~Texture() = default;

	Texture &operator=(const Texture &rhs) = default;
	Texture &operator=(Texture &&rhs) = default;

	void setType(std::string &type);
	const std::string &getType();
	GLuint getId();

	class TextureLoadingException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

private:

	GLuint _id {};
	std::string _type {};
	int _width {};
	int _height {};
	int _nrChannels{};
};