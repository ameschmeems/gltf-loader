#include "Model.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

/**
 * @brief Constructor for a model from a file. If default scene not specified, reads scene at index 0
 * 
 * @param path path to a model file
 * 
 * @throws ModelLoadingException if tinygltf fails to load file
 */
Model::Model(std::string &path)
{
	// _loadModel(path);

	spdlog::debug("Loading model from file: {}", path);

	tinygltf::Model m {};
	tinygltf::TinyGLTF loader {};
	std::string err {};
	std::string warn {};

	bool ret = loader.LoadASCIIFromFile(&m, &err, &warn, path);

	if (!warn.empty()) {
		spdlog::warn("TinyGLTF: {}", warn);
	}

	if (!err.empty()) {
		spdlog::error("TinyGLTF: {}", err);
	}

	if (!ret) {
		throw ModelLoadingException("Failed to load model: " + path);
	}
	
	_processScene(m, m.defaultScene > -1 ? m.defaultScene : 0);

	spdlog::debug("Loaded model file with TinyGLTF");
}

/**
 * @brief Renders a model to the screen
 * 
 * @param shader shader program to be used for rendering
 */
void Model::draw(Shader &shader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i].draw(shader);
	}
}

void Model::_processScene(tinygltf::Model &model, size_t scene_index)
{
	tinygltf::Scene &scene = model.scenes[scene_index];
	for (size_t i = 0; i < scene.nodes.size(); i++)
	{
		spdlog::debug("Processing node: {}", scene.nodes[i]);
		if (model.nodes[i].mesh > -1)
		{
			_meshes.push_back(Mesh { model, model.meshes[model.nodes[i].mesh] });
		}
	}
}