#include <stb_image.h>
#include <sstream>
#include <spdlog/spdlog.h>
#include "Texture.hpp"

/**
 * @brief Constructor for a Texture from a path to an image file
 * 
 * @param path The path to an image file
 * @param typeName The type of texture ("texture_diffuse", "texture_specular")
 * @param type The type that the image is stored as. Defaults to GL_UNSIGNED_BYTE
 * @param flipVertically Decides whether the image should be flipped vertically or not. Defaults to true
 * 
 * @throws TextureLoadingException if texture failed to load
 */
Texture::Texture(const std::string &path, const std::string &typeName, GLenum type, bool flipVertically)
{
	spdlog::debug(
		"Loading texture from file: {}, typeName: {}, type: {}, flipVertically: {}",
		path,
		typeName,
		type,
		flipVertically
	);

	glGenTextures(1, &_id);
	
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flipVertically);
	
	unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &_nrChannels, 0);
	if (data)
	{
		GLenum format {};
		if (_nrChannels == 1)
		format = GL_RED;
		else if (_nrChannels == 3)
		format = GL_RGB;
		else if (_nrChannels == 4)
		format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, type, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::stringstream error {};
		error << "Failed to load texture: " << path << std::endl;
		throw TextureLoadingException(error.str());
	}
	stbi_image_free(data);

	spdlog::debug("Texture loaded successfully from file");
}

void Texture::setType(std::string &type)
{
	_type = type;
}

const std::string &Texture::getType()
{
	return _type;
}

GLuint Texture::getId()
{
	return _id;
}