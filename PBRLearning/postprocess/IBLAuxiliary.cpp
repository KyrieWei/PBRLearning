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

	unsigned int hdrToCubemapShader = shaderMgr->loadShader("hdrToCubeShader", "shaders/hdrToCubemap_vert.vs", "shaders/hdrToCubemap_frag.fs");

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
	Shader::ptr shader = shaderMgr->getShader(hdrToCubemapShader);
	Mesh::ptr cubeMesh = std::shared_ptr<Mesh>(new Cube(1.0f, 1.0f, 1.0f));

	shader->use();
	shader->setInt("equirectangularMap", 0);
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

	unsigned int skyboxConvShader = shaderMgr->loadShader("skyboxConvShader", "shaders/irradiance_convolution_vert.vs", "shaders/irradiance_convolution_frag.fs");

	//load framebuffer
	FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "irradianceDepth", {}, true));

	// projection matrix and view matrix.
	glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViewMatrix[] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	};

	framebuffer->bind();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint irradianceTexID = textureMgr->getTexture(irradianceTexIndex)->getTextureID();
	Shader::ptr shader = shaderMgr->getShader(skyboxConvShader);
	Mesh::ptr cubeMesh = std::shared_ptr<Mesh>(new Cube(1.0f, 1.0f, 1.0f));

	shader->use();
	shader->setInt("environmentMap", 0);
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

void IBLAuxiliary::convoluteSpecularIntegral(int width, int height, unsigned int cubemapTexIndex, unsigned int prefilteredTexIndex)
{
	//manager
	TextureMgr::ptr texMgr = TextureMgr::getSingleton();
	ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
	MeshMgr::ptr meshMgr = MeshMgr::getSingleton();

	unsigned int skyboxPrefilterShader = shaderMgr->loadShader("skyboxPrefilterShader", "shaders/prefilter_env_map_vert.vs", "shaders/prefilter_env_map_frag.fs");

	glm::mat4 captureProjectMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViewMatrix[] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(+1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,+1.0f, 0.0f), glm::vec3(0.0f,  0.0f,+1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f,  0.0f,-1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,+1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	//begin to filter
	GLuint prefilteredTexId = texMgr->getTexture(prefilteredTexIndex)->getTextureID();
	Shader::ptr shader = shaderMgr->getShader(skyboxPrefilterShader);
	Mesh::ptr cubeMesh = std::shared_ptr<Mesh>(new Cube(1.0f, 1.0f, 1.0f));
	shader->use();
	shader->setInt("environmentMap", 0);
	shader->setMat4("projection", captureProjectMatrix);
	texMgr->bindTexture(cubemapTexIndex, 0);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; mip++)
	{
		unsigned int mipWidth = width * std::pow(0.5, mip);
		unsigned int mipHeight = height * std::pow(0.5, mip);
		std::stringstream ss;
		ss << mip;
		FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(mipWidth, mipHeight, "prefilteredDepth" + ss.str(), {}, true));
		framebuffer->bind();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shader->setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; i++)
		{
			shader->setMat4("view", captureViewMatrix[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilteredTexId, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cubeMesh->draw();
		}
		framebuffer->unBind();
	}

	texMgr->unbindTexture(cubemapTexIndex);
}


void IBLAuxiliary::convoluteSpecularBRDFIntegral(int width, int height, unsigned int brdfLutTexIndex)
{
	TextureMgr::ptr texMgr = TextureMgr::getSingleton();
	ShaderMgr::ptr shaderMgr = ShaderMgr::getSingleton();
	MeshMgr::ptr meshMgr = MeshMgr::getSingleton();

	unsigned int skyboxPrecomputeBRDFShader = shaderMgr->loadShader("skyboxPrecomputeBRDFShader", "shaders/precompute_brdfLut_vert.vs", "shaders/precompute_brdfLut_frag.fs");

	FrameBuffer::ptr framebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "brdfDepth", {}, true));

	Shader::ptr shader = shaderMgr->getShader(skyboxPrecomputeBRDFShader);
	Mesh::ptr quadMesh = std::shared_ptr<Mesh>(new ScreenQuad());

	framebuffer->bind();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint brdfLutTexId = texMgr->getTexture(brdfLutTexIndex)->getTextureID();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLutTexId, 0);

	shader->use();
	quadMesh->draw();

	framebuffer->unBind();
}