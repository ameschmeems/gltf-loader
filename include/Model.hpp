#pragma once
#include <stdexcept>
#include <tiny_gltf.h>
#include "Shader.hpp"
#include "Mesh.hpp"

/**
 * @class Model Model.hpp "include/Model.hpp"
 * @brief A class encapsulating a model, composed of one or more meshes
 */
class Model
{
public:

	Model(std::string &path);

	void draw(Shader &shader);

	class ModelLoadingException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

private:

	void _processScene(tinygltf::Model &model, size_t sceneIndex);
	void _processNode(tinygltf::Model &model, tinygltf::Node &node);

	std::vector<Mesh> _meshes {};
	std::string _directory {};
	std::vector<std::string> _textures_loaded {};
};