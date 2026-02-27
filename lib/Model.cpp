#include "Model.hpp"
#include <iostream>

/**
 * @brief Constructor for a model from a file
 * 
 * @param path path to a model file
 * 
 * @throws ModelLoadingException if assimp fails to load file
 */
Model::Model(char *path)
{
	_loadModel(path);
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
		// TODO: process material
	}

	return Mesh(vertices, indices, textures);
}