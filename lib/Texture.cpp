#include <stb_image.h>
#include <sstream>
#include "Texture.hpp"

/**
 * @brief Constructor for a Texture from a path to an image file
 * 
 * @param path The path to an image file
 * @param target The desired texture unit
 * @param internalFormat The format that openGL should store the texture as. Defaults to GL_RGBA
 * @param format The format that the image is stored as. Defaults to GL_RGBA
 * @param type The type that the image is stored as. Defaults to GL_UNSIGNED_BYTE
 * @param flipVertically Decides whether the image should be flipped vertically or not. Defaults to true
 * 
 * @throws TextureLoadingException if texture failed to load
 */
Texture::Texture(const std::string &path, GLenum target, GLint internalFormat, GLenum format, GLenum type, bool flipVertically)
	: _target { target }
{
	glGenTextures(1, &_id);
	
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flipVertically);
	
	unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &_nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, type, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::stringstream error {};
		error << "Failed to load texture: " << path << std::endl;
		throw TextureLoadingException(error.str());
	}
	stbi_image_free(data);
}

/**
 * @brief Binds the texture as active
 */
void Texture::bind()
{
	glActiveTexture(_target);
	glBindTexture(GL_TEXTURE_2D, _id);
}