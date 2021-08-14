#include "Drawable.h"
#include "../objects/MeshMgr.h"
#include "../textures/TextureMgr.h"
#include "../shaders/ShaderMgr.h"

void Drawable::renderImp()
{
	MeshMgr::ptr meshMgr = MeshMgr::getSingleton();
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();

	for (int i = 0; i < meshIndex.size(); i++)
	{
		if (i < texIndex.size())
		{
			textureMgr->bindTexture(texIndex[i].albedoTexIndex, 0);
			textureMgr->bindTexture(texIndex[i].normalTexIndex, 1);
			textureMgr->bindTexture(texIndex[i].roughTexIndex, 2);
			textureMgr->bindTexture(texIndex[i].metallicIndex, 3);
		}

		meshMgr->drawMesh(meshIndex[i]);

		if (i < texIndex.size())
		{
			textureMgr->unbindTexture(texIndex[i].albedoTexIndex);
			textureMgr->unbindTexture(texIndex[i].normalTexIndex);
			textureMgr->unbindTexture(texIndex[i].roughTexIndex);
			textureMgr->unbindTexture(texIndex[i].metallicIndex);
		}
	}
}

void SimpleDrawable::render(Camera::ptr camera)
{
	if (!visible) return;

	Shader::ptr shader = ShaderMgr::getSingleton()->getShader(shaderIndex);
	shader->use();

	shader->setInt("albedoMap", 0);
	shader->setInt("normalMap", 1);
	shader->setInt("roughnessMap", 2);
	shader->setInt("metallicMap", 3); 
	
	shader->setMat4("model", transformation.getWorldMatrix());
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectMatrix());

	this->renderImp();
}

void SimpleDrawable::renderDepth(Shader::ptr shader)
{

}

void SkyDome::render(Camera::ptr camera)
{
	if (!visible) return;

	Shader::ptr shader = ShaderMgr::getSingleton()->getShader(shaderIndex);
	shader->use();

	shader->setInt("environmentMap", 0);
	shader->setMat4("view", glm::mat4(glm::mat3(camera->GetViewMatrix())));
	shader->setMat4("projection", camera->GetProjectMatrix());
	this->renderImp();
}

void SkyDome::renderDepth(Shader::ptr shader)
{

}