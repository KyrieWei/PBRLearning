#include "Drawable.h"
#include "../objects/MeshMgr.h"
#include "../textures/TextureMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../system/Window.h"

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

PointLightDrawable::PointLightDrawable()
{
	particleVBO = 0; 
	particleRadius = 1.0f;
	lightColor = glm::vec3(1.0, 1.0, 1.0);
	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);

	//load shader and texture

	shaderMgr = ShaderMgr::getSingleton();
	shaderIndex = shaderMgr->loadShader("lightSource", "shaders/lightSource_vert.vs", "shaders/lightSource_frag.fs");
}

PointLightDrawable::~PointLightDrawable()
{
	glDeleteBuffers(1, &particleVBO);
	glDeleteVertexArrays(1, &particleVAO);
}

void PointLightDrawable::setPointLightRadius(float radius)
{
	particleRadius = radius;
}

void PointLightDrawable::setPointLightPositions(const std::vector<PointLight::ptr>& lights)
{
	particleNum = lights.size();
	std::vector<LightSource> pos;

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		LightSource target;
		target.pos = lights[i]->getPosition();
		target.color = glm::normalize(lights[i]->getRadiance());

		pos.push_back(target);
	}

	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(LightSource), &pos[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LightSource), reinterpret_cast<void*>(offsetof(LightSource, pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LightSource), reinterpret_cast<void*>(offsetof(LightSource, color)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PointLightDrawable::render(Camera::ptr camera)
{
	int scrWidth = Window::getSingleton()->getWindowWidth();
	float aspect = camera->getAspect();
	float fovy = camera->getFovy();
	float pointScale = 1.0f * scrWidth / aspect * (1.0f / tanf(glm::radians(fovy) * 0.5f));

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	Shader::ptr shader = shaderMgr->getShader("lightSource");
	shader->use();
	shader->setFloat("pointScale", pointScale);
	shader->setFloat("pointSize", particleRadius);
	shader->setMat4("model", glm::mat4(1.0f));
	shader->setMat4("view", camera->GetViewMatrix());
	shader->setMat4("projection", camera->GetProjectMatrix());

	glBindVertexArray(particleVAO);
	glDrawArrays(GL_POINTS, 0, particleNum);
	glBindVertexArray(0);

	glDisable(GL_PROGRAM_POINT_SIZE);
}

void PointLightDrawable::renderDepth(Shader::ptr shader)
{
	return;
}