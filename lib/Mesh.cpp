#include "Mesh.hpp"
#include <spdlog/spdlog.h>

/**
 * @brief Constructor for a Mesh, from vectors of vertices, indices, and textures
 * 
 * @param vertices vector of vertices of the mesh
 * @param indices vector of indices of the mesh
 * @param textures vector of textures of the mesh
 */
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: _vertices { vertices }, _indices { indices }, _textures { textures }
{
	spdlog::debug(
		"Loading mesh with {} vertices, {} indices, {} textures.",
		_vertices.size(),
		_indices.size(),
		_textures.size()
	);

	_setupMesh();
}

/**
 * @brief Renders the mesh to the screen
 * 
 * @param shader shader program to be used for rendering
 */
void Mesh::draw(Shader &shader)
{
	unsigned int diffuseNr { 1 };
	unsigned int specularNr { 1 };

	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number {};
		std::string name = _textures[i].getType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setUniform(name + number, i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].getId());
	}

	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::_setupMesh()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		nullptr
	);
	glEnableVertexAttribArray(0);
	// vertex normals
    glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, Normal)
	);
	glEnableVertexAttribArray(1);
    // vertex texture coords
    glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, TexCoords)
	);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}