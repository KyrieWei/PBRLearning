#pragma once
#include <string>
#include "Drawable.h"
#include "../tools/Camera.h"

class aiMesh;
class aiNode;
class aiScene;

class StaticModelDrawable : public Drawable
{
private:
	std::string directory;
public:
	typedef std::shared_ptr<StaticModelDrawable> ptr;

	StaticModelDrawable(unsigned int shaderIndex, const std::string& path, const std::string& name);
	~StaticModelDrawable();

	virtual void render(Camera::ptr camera);
	virtual void renderDepth(Shader::ptr shader);

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene, unsigned int& meshIndex, PBRMaterial& pbrMat);
};

