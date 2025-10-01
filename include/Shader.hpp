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

	class VertexShaderCompilationException : public std::exception
	{
	public:
		VertexShaderCompilationException(const std::string &message) throw();
		const char *what() const throw();
	
	private:
		const std::string _message;
	};

	class FragmentShaderCompilationException : public std::exception
	{
	public:
		FragmentShaderCompilationException(const std::string &message) throw();
		const char *what() const throw();
	
	private:
		const std::string _message;
	};

	class ShaderProgramLinkingException : public std::exception
	{
	public:
		ShaderProgramLinkingException(const std::string &message) throw();
		const char *what() const throw();
	
	private:
		const std::string _message;
	};

private:

	GLuint _id;
};