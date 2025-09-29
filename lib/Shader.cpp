#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

/**
 * @brief Constructor for a Shader from a path to vertex and fragment shader
 * 
 * @param vertPath The path to a vertex shader
 * @param fragPath The path to a fragment shader
 * 
 * @throws VertexShaderCompilationException if failed to compile vertex shader
 * @throws FragmentShaderCompilationException if failed to compile fragment shader
 * @throws ShaderProgramLinkingException if failed to link shader program
 */
Shader::Shader(const std::string &vertPath, const std::string &fragPath) : _id { glCreateProgram() }
{
	std::ifstream is { vertPath };
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
		std::stringstream error {};
		error << "Vertex Shader compilation failed: " << infoLog << std::endl;
		throw VertexShaderCompilationException(error.str());
	}
	spdlog::debug("Compiled Vertex Shader");

	is.open(fragPath);
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
		std::stringstream error {};
		error << "Fragment Shader compilation failed: " << infoLog << std::endl;
		throw FragmentShaderCompilationException(error.str());
	}
	spdlog::debug("Compiled Fragment Shader");

	glAttachShader(_id, vertShader);
	glAttachShader(_id, fragShader);
	glLinkProgram(_id);

	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, nullptr, infoLog);
		std::stringstream error {};
		error << "Failed to link Shader Program: " << infoLog << std::endl;
		throw ShaderProgramLinkingException(error.str());
	}
	spdlog::debug("Linked Shader Program");

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void Shader::useProgram()
{
	glUseProgram(_id);
}

Shader::VertexShaderCompilationException::VertexShaderCompilationException(const std::string &message) : _message { message } {}

const char *Shader::VertexShaderCompilationException::what() const throw()
{
	return _message.c_str();
}

Shader::FragmentShaderCompilationException::FragmentShaderCompilationException(const std::string &message) : _message { message } {}

const char *Shader::FragmentShaderCompilationException::what() const throw()
{
	return _message.c_str();
}

Shader::ShaderProgramLinkingException::ShaderProgramLinkingException(const std::string &message) : _message { message } {}

const char *Shader::ShaderProgramLinkingException::what() const throw()
{
	return _message.c_str();
}