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

		if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
		{
			_processNormals(model, model.accessors[primitive.attributes.at("NORMAL")]);
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
		(void*)_elementOffset
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
		buffer.data.data() + bufferView.byteOffset,
		GL_STATIC_DRAW
	);

	_elementCount = accessor.count;
	_elementComponentType = accessor.componentType;
	_elementOffset = accessor.byteOffset;
}

void Mesh::_processVertices(tinygltf::Model &model, tinygltf::Accessor &accessor)
{
	tinygltf::BufferView &bufferView { model.bufferViews[accessor.bufferView] };
	tinygltf::Buffer &buffer { model.buffers[bufferView.buffer] };
	GLenum target { bufferView.target > -1 ? static_cast<GLenum>(bufferView.target) : GL_ARRAY_BUFFER };

	glGenBuffers(1, &_positionVbo);
	glBindBuffer(target, _positionVbo);
	glBufferData(
		target,
		bufferView.byteLength,
		buffer.data.data() + bufferView.byteOffset,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		VERTEX_ATTRIB_POSITION,
		3,
		accessor.componentType,
		GL_FALSE,
		bufferView.byteStride,
		(void*)(accessor.byteOffset)
	);
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
}

void Mesh::_processNormals(tinygltf::Model &model, tinygltf::Accessor &accessor)
{
	tinygltf::BufferView &bufferView { model.bufferViews[accessor.bufferView] };
	tinygltf::Buffer &buffer { model.buffers[bufferView.buffer] };
	GLenum target { bufferView.target > -1 ? static_cast<GLenum>(bufferView.target) : GL_ARRAY_BUFFER };

	glGenBuffers(1, &_normalVbo);
	glBindBuffer(target, _normalVbo);
	glBufferData(
		target,
		bufferView.byteLength,
		buffer.data.data() + bufferView.byteOffset,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		VERTEX_ATTRIB_NORMAL,
		3,
		accessor.componentType,
		GL_FALSE,
		bufferView.byteStride,
		(void*)(accessor.byteOffset)
	);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
}