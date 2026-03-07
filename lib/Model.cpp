#include "Model.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

/**
 * @brief Constructor for a model from a file. If default scene not specified, reads scene at index 0
 * 
 * @param path path to a model file
 * 
 * @throws ModelLoadingException if tinygltf fails to load file
 */
Model::Model(std::string &path)
{
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

void Model::_processScene(tinygltf::Model &model, size_t sceneIndex)
{
	spdlog::debug("Processing Scene: {}", sceneIndex);

	tinygltf::Scene &scene = model.scenes[sceneIndex];

	for (auto i : scene.nodes)
	{
		_processNode(model, model.nodes[i]);
	}
}

void Model::_processNode(tinygltf::Model &model, tinygltf::Node &node)
{
	if (node.mesh > -1)
	{
		spdlog::debug("Processing Mesh: {}", node.mesh);
		_meshes.push_back(Mesh { model, model.meshes[node.mesh] });
	}

	for (auto child : node.children)
	{
		_processNode(model, model.nodes[child]);
	}
}