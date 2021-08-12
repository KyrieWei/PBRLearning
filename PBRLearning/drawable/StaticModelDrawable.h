#pragma once
#include <string>
#include "Drawable.h"

class aiMesh;
class aiNode;
class aiScene;

class StaticModelDrawable : public Drawable
{
private:
	std::string directory;

public:
	typedef std::shared_ptr<StaticModelDrawable> ptr;

	StaticModelDrawable(unsigned int shaderIndex, const std::string& path);
	~StaticModelDrawable();

	virtual void render(Shader::ptr shader = nullptr);
	virtual void renderDepth(Shader::ptr shader);

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene, unsigned int meshIndex, PBRMaterial& pbrMat);
};

