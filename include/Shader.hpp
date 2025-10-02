#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

/**
 * @class Shader Shader.hpp "include/Shader.hpp"
 * @brief A class encapsulating an openGL shader program
 */
class Shader
{
public:

	Shader() = delete;
	Shader(const Shader &rhs) = default;
	Shader(Shader &&rhs) = default;
	Shader(const std::string &vertPath, const std::string &fragPath);
	~Shader() = default;

	Shader &operator=(const Shader &rhs) = default;
	Shader &operator=(Shader &&rhs) = default;

	void useProgram();

	void setUniform(std::string name, int val);

	class VertexShaderCompilationException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class FragmentShaderCompilationException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class ShaderProgramLinkingException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

private:

	GLuint _id;
};