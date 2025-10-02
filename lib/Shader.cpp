#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

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


/**
 * @brief Sets shader program as active using glUseProgram
 */
void Shader::useProgram()
{
	glUseProgram(_id);
}

/**
 * @brief Sets GLint uniform
 * 
 * @param name Uniform name
 * @param val The GLint value the uniform should be set to
 */
void Shader::setUniform(std::string name, GLint val)
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), val);
}

/**
 * @brief Sets mat4 uniform
 * 
 * @param name Uniform name
 * @param val The mat4 the uniform should be set to
 */
void Shader::setUniform(std::string name, glm::mat4 &val)
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}
