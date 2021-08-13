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

	shader->setInt("albedomap", 0);
	
	shader->setMat4("model", glm::mat4(1.0));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectMatrix());

	this->renderImp();
}

void SimpleDrawable::renderDepth(Shader::ptr shader)
{

}