#include "IBLAuxiliary.h"
#include "../textures/TextureMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../objects/Geometry.h"
#include "../objects/FrameBuffer.h"
#include "../objects/MeshMgr.h"

void IBLAuxiliary::convertToCubemap(int width, int height, unsigned int hdrTexIndex, unsigned int cubemapTexIndex)
{
	//manager
	ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
	MeshMgr::ptr meshMgr = MeshMgr::getSingleton();

	//load framebuffer
	FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "convertDepth", {}, true));

	//projection and view matrix
	glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViewMatrix[] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,+1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f,-1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	};

	//convert
	framebuffer->bind();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint cubemapId = textureMgr->getTexture(cubemapTexIndex)->getTextureID();
	Shader::ptr shader = shaderMgr->getShader("hdrToCubeShader");
	Mesh::ptr cubeMesh = meshMgr->getMesh(1);

	shader->use();
	shader->setMat4("projection", captureProjectMatrix);
	textureMgr->bindTexture(hdrTexIndex, 0);

	for (unsigned int i = 0; i < 6; i++)
	{
		shader->setMat4("view", captureViewMatrix[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapId, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeMesh->draw();
	}

	textureMgr->unbindTexture(hdrTexIndex);
	framebuffer->unBind();
}

void IBLAuxiliary::convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex, unsigned int irradianceTexIndex)
{
	//manager
	ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
	MeshMgr::ptr meshMgr = MeshMgr::getSingleton();

	//load framebuffer
	FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "irradianceDepth", {}, true));

	// projection matrix and view matrix.
	glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViewMatrix[] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,+1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f,-1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	};

	framebuffer->bind();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint irradianceTexID = textureMgr->getTexture(irradianceTexIndex)->getTextureID();
	Shader::ptr shader = shaderMgr->getShader("skyboxConvShader");
	Mesh::ptr cubeMesh = meshMgr->getMesh(1);

	shader->use();
	shader->setMat4("projection", captureProjectMatrix);
	textureMgr->bindTexture(cubemapTexIndex, 0);

	for (unsigned int i = 0; i < 6; i++)
	{
		shader->setMat4("view", captureViewMatrix[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceTexID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeMesh->draw();
	}

	textureMgr->unbindTexture(cubemapTexIndex);
	framebuffer->unBind();
}