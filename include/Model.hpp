#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
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

	void _loadModel(std::string path);
	void _processNode(aiNode *node, const aiScene *scene);
	Mesh _processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> _loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	std::vector<Mesh> _meshes {};
	std::string _directory {};
	std::vector<std::string> _textures_loaded {};
};