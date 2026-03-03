#include "Model.hpp"
#include <iostream>
#include <tiny_gltf.h>
#include <spdlog/spdlog.h>

/**
 * @brief Constructor for a model from a file
 * 
 * @param path path to a model file
 * 
 * @throws ModelLoadingException if assimp fails to load file
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

void Model::_loadModel(std::string path)
{
	Assimp::Importer importer {};
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw ModelLoadingException(importer.GetErrorString());
	}
	_directory = path.substr(0, path.find_last_of('/'));

	_processNode(scene->mRootNode, scene);
}

void Model::_processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		_processNode(node->mChildren[i], scene);
	}
}

Mesh Model::_processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices {};
	std::vector<unsigned int> indices {};
	std::vector<Texture> textures {};

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex {};
		
		glm::vec3 pos {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};
		vertex.Position = pos;
		
		glm:: vec3 normal {
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		};
		vertex.Normal = normal;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec {
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			};
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = _loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = _loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::_loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures {};
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str {};
		mat->GetTexture(type, i, &str);
		std::string texPath = _directory + "/" + std::string(str.C_Str());
		
		if (std::find(_textures_loaded.begin(), _textures_loaded.end(), texPath) != _textures_loaded.end())
			continue;
		Texture texture { texPath, typeName };
		_textures_loaded.push_back(texPath);
		textures.push_back(texture);
	}
	return textures;
}