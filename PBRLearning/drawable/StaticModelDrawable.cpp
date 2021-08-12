#include "StaticModelDrawable.h"
#include "../shaders/ShaderMgr.h"

StaticModelDrawable::StaticModelDrawable(unsigned int shaderIndex, const std::string& path)
{
	this->shaderIndex = shaderIndex;
	loadModel(path);
}

StaticModelDrawable::~StaticModelDrawable()
{

}

void StaticModelDrawable::render(Camera::ptr camera, Shader::ptr shader)
{
	//render the model
	if (!visible) return;
	if (shader == nullptr)
	{
		shader = ShaderMgr::getSingleton()->getShader(shaderIndex);
		shader->use();
	}

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