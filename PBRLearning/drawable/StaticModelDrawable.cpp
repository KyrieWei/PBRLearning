#include "StaticModelDrawable.h"
#include "../shaders/ShaderMgr.h"
#include "../objects/MeshMgr.h"
#include "../textures/TextureMgr.h"

#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

StaticModelDrawable::StaticModelDrawable(unsigned int shaderIndex, const std::string& path, const std::string& name)
{
	this->shaderIndex = shaderIndex;
	this->name = name;
	loadModel(path);
}

StaticModelDrawable::~StaticModelDrawable()
{

}

void StaticModelDrawable::render(Camera::ptr camera)
{
	//render the model
	if (!visible) return;

	Shader::ptr	shader = ShaderMgr::getSingleton()->getShader(shaderIndex);
	shader->use();

	shader->setInt("albedoMap", 0);
	shader->setInt("normalMap", 0);
	shader->setInt("roughMap", 0);
	shader->setInt("metallicMap", 3);
	
	//depth map

	//object matrix
	//shader->setBool("instance", false);
	shader->setMat4("model", glm::mat4(1.0));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectMatrix());

	this->renderImp();
}

void StaticModelDrawable::renderDepth(Shader::ptr shader)
{

}

void StaticModelDrawable::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void StaticModelDrawable::processNode(aiNode* node, const aiScene* scene)
{
	//process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		unsigned int meshIndex;
		PBRMaterial pbrMat;
		processMesh(mesh, scene, meshIndex, pbrMat);
		this->addMesh(meshIndex);
		this->addPbrTexture(pbrMat);
	}
}

void StaticModelDrawable::processMesh(aiMesh* mesh, const aiScene* scene, unsigned int& meshIndex, PBRMaterial& pbrMat)
{
	//process mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//position
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		//normal
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		//texcoord
		if (mesh->mTextureCoords[0])
			vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		//color
		vertex.color = vertex.normal;
		//tangent
		vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		//bitangent
		vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

		vertices.push_back(vertex);

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Mesh* target = new Mesh(vertices, indices);
	meshIndex = MeshMgr::getSingleton()->loadMesh(target, name);

	//process pbr material
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString nameStr;

		//albedo texture
		material->GetTexture(aiTextureType_DIFFUSE, 0, &nameStr);
		std::string tex_name(nameStr.C_Str());
		if (!tex_name.empty())
			pbrMat.albedoTexIndex = textureMgr->loadTexture2D(tex_name, directory + "/" + tex_name);

		//normal texture
		material->GetTexture(aiTextureType_HEIGHT, 0, &nameStr);
		tex_name = nameStr.C_Str();
		if (!tex_name.empty())
			pbrMat.normalTexIndex = textureMgr->loadTexture2D(tex_name, directory + "/" + tex_name);

		//roughness texture
		material->GetTexture(aiTextureType_SHININESS, 0, &nameStr);
		tex_name = nameStr.C_Str();
		if (!tex_name.empty())
			pbrMat.roughTexIndex = textureMgr->loadTexture2D(tex_name, directory + "/" + tex_name);

		//metallic texture
		material->GetTexture(aiTextureType_AMBIENT, 0, &nameStr);
		tex_name = nameStr.C_Str();
		if (!tex_name.empty())
			pbrMat.metallicIndex = textureMgr->loadTexture2D(tex_name, directory + "/" + tex_name);


	}

}