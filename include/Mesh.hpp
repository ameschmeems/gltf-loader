#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <tiny_gltf.h>
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

	Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh);

	void draw(Shader &shader);

private:

	void _setupMesh();
	void _processIndices(tinygltf::Model &model, tinygltf::Accessor &accessor);
	void _processVertices(tinygltf::Model &model, tinygltf::Accessor &accessor);

	std::vector<Vertex> _vertices {};
	std::vector<unsigned int> _indices {};
	std::vector<Texture> _textures {};

	unsigned int _elementCount {};
	GLenum _elementComponentType {};

	unsigned int _vao {};
	unsigned int _vbo {};
	unsigned int _ebo {};
};