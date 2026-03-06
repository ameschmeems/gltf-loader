#include "Mesh.hpp"
#include <spdlog/spdlog.h>

/**
 * @brief Constructor for a Mesh, from tinygltf mesh object
 * 
 * @param model Model loaded from tinygltf
 * @param mesh Mesh loaded from tinygltf
 */
Mesh::Mesh(tinygltf::Model &model, tinygltf::Mesh &mesh)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	for (auto primitive : mesh.primitives)
	{
		if (primitive.indices > -1)
		{
			_processIndices(model, model.accessors[primitive.indices]);
		}

		if (primitive.attributes.find("POSITION") != primitive.attributes.end())
		{
			_processVertices(model, model.accessors[primitive.attributes.at("POSITION")]);
		}
	}

	glBindVertexArray(0);
}

/**
 * @brief Renders the mesh to the screen
 * 
 * @param shader shader program to be used for rendering
 */
void Mesh::draw(Shader &shader)
{
	glBindVertexArray(_vao);
	glDrawElements(
		GL_TRIANGLES,
		_elementCount,
		_elementComponentType,
		0
	);
	glBindVertexArray(0);
}

void Mesh::_processIndices(tinygltf::Model &model, tinygltf::Accessor &accessor)
{
	tinygltf::BufferView &bufferView { model.bufferViews[accessor.bufferView] };
	tinygltf::Buffer &buffer { model.buffers[bufferView.buffer] };
	GLenum target { bufferView.target > -1 ? static_cast<GLenum>(bufferView.target) : GL_ELEMENT_ARRAY_BUFFER };

	glGenBuffers(1, &_ebo);
	glBindBuffer(target, _ebo);
	glBufferData(
		target,
		bufferView.byteLength,
		&buffer.data[bufferView.byteOffset],
		GL_STATIC_DRAW
	);

	_elementCount = accessor.count;
	_elementComponentType = accessor.componentType;
}

void Mesh::_processVertices(tinygltf::Model &model, tinygltf::Accessor &accessor)
{
	tinygltf::BufferView &bufferView { model.bufferViews[accessor.bufferView] };
	tinygltf::Buffer &buffer { model.buffers[bufferView.buffer] };
	GLenum target { bufferView.target > -1 ? static_cast<GLenum>(bufferView.target) : GL_ARRAY_BUFFER };

	glGenBuffers(1, &_vbo);
	glBindBuffer(target, _vbo);
	glBufferData(
		target,
		bufferView.byteLength,
		&buffer.data[bufferView.byteOffset],
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		accessor.componentType,
		GL_FALSE,
		accessor.byteOffset,
		0
	);
	glEnableVertexAttribArray(0);
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