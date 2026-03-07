#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <tiny_gltf.h>
#include "Texture.hpp"
#include "Shader.hpp"

#define VERTEX_ATTRIB_POSITION 0
#define VERTEX_ATTRIB_NORMAL 1
#define VERTEX_ATTRIB_TEX_COORD_0 2

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
	void _processNormals(tinygltf::Model &model, tinygltf::Accessor &accessor);

	unsigned int _elementCount {};
	unsigned int _elementOffset {};
	GLenum _elementComponentType {};

	unsigned int _vao {};
	unsigned int _positionVbo {};
	unsigned int _normalVbo {};
	unsigned int _ebo {};
};