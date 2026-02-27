#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Texture.hpp"
#include "Shader.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

/**
 * @class Mesh Mesh.hpp "include/Mesh.hpp"
 * @brief A class encapsulating a single Mesh of a Model
 */
class Mesh
{
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void draw(Shader &shader);

private:

	void _setupMesh();

	std::vector<Vertex> _vertices {};
	std::vector<unsigned int> _indices {};
	std::vector<Texture> _textures {};

	unsigned int _vao {};
	unsigned int _vbo {};
	unsigned int _ebo {};
};