#include "PBRenderer.h"
#include "../postprocess/IBLAuxiliary.h"
#include "../objects/Geometry.h"

void PBRenderer::initialzie(int _width, int _height)
{
	width = _width;
	height = _height;

	camera = std::make_shared<Camera>(Camera(glm::vec3(0.0f, 0.0f, 3.0f)));

	setSunLight(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f));

	gaussianBlur = std::shared_ptr<GaussianBlur>(new GaussianBlur(width, height));

	meshMgr = MeshMgr::getSingleton();
	shaderMgr = ShaderMgr::getSingleton();
	textureMgr = TextureMgr::getSingleton();

	drawableList = std::make_shared<DrawableList>();

	deferredShading = std::shared_ptr<DeferredShading>(new DeferredShading(width, height));

	camera->setInitialStatus(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-0.2f, 1.0f, -0.4f), -480.0f, -24.0f);
	camera->setMovementSpeed(30.0f);
}

void PBRenderer::addPointLight(glm::vec3 pos, glm::vec3 radiance)
{
	if (pointLights.size() > 128)
		return;

	PointLight::ptr pointLight = std::shared_ptr<PointLight>(new PointLight());
	pointLight->setPosition(pos, pointLights.size());
	pointLight->setLightColor(radiance);
	pointLights.push_back(pointLight);

}

void PBRenderer::addPointLightDrawble()
{
	if (pointLightDrawable == nullptr && pointLights.size() > 0)
	{
		pointLightDrawable = std::shared_ptr<PointLightDrawable>(new PointLightDrawable());
		pointLightDrawable->setPointLightRadius(0.7f);
		pointLightDrawable->setPointLightPositions(pointLights);
	}
}

void PBRenderer::setSunLight(glm::vec3 dir, glm::vec3 radiance)
{
	DirectionalLight* light = new DirectionalLight();
	light->setDirection(dir);
	light->setLightColor(radiance);
	sunLight = std::shared_ptr<DirectionalLight>(light);
}

void PBRenderer::setSkyDomeHDR(const std::string& path)
{
	if (skyDome != nullptr)
		return;

	unsigned int skyboxShader = shaderMgr->loadShader("skybox", "shaders/skybox_vert.vs", "shaders/skybox_frag.fs");
	unsigned int hdrTexIndex = textureMgr->loadTexture2DHDR("hdrTex", path);
	unsigned int cubeTexIndex = textureMgr->loadTextureCubeHDR("skyboxCubemap", nullptr, 1024, 1024);

	//convert hdrmap to cubemap
	IBLAuxiliary::convertToCubemap(1024, 1024, hdrTexIndex, cubeTexIndex);

	//precompute the irradiance map
	unsigned int irradianceTexIndex = textureMgr->loadTextureCubeHDR("irradianceMap", nullptr, 512, 512);
	IBLAuxiliary::convoluteDiffuseIntegral(512, 512, cubeTexIndex, irradianceTexIndex);

	//prefilter the environment map for specular lighting
	unsigned int prefilteredTexIndex = textureMgr->loadTextureCubeHDR("prefilteredMap", nullptr, 256, 256, true);
	IBLAuxiliary::convoluteSpecularIntegral(256, 256, cubeTexIndex, prefilteredTexIndex);

	//generate brdf lookup texture
	unsigned int brdfLutTexIndex = textureMgr->loadTexture2DHDRRaw("brdfLutMap", nullptr, 512, 512);
	IBLAuxiliary::convoluteSpecularBRDFIntegral(512, 512, brdfLutTexIndex);

	unsigned int mesh = meshMgr->loadMesh(new Sphere(1, 25, 25));
	skyDome = std::make_shared<SkyDome>(skyboxShader);
	PBRMaterial mat;
	mat.albedoTexIndex = cubeTexIndex;
	skyDome->addMesh(mesh);
	skyDome->addPbrTexture(mat);
}

void PBRenderer::render_simplescene()
{
	glm::vec3 lightPositions[4] =
	{
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	glm::vec3 lightColors[4] = 
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	Shader::ptr shader = shaderMgr->getShader("pbrShader"); 
	shader->use();
	
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera->Zoom, (float)width / (float)height, 0.1f, 100.0f);

	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	shader->setVec3("viewPos", camera->Position);

	shader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
	shader->setFloat("ao", 1.0f);

	textureMgr->bindTexture("skyboxConvCubemap", 0);
	textureMgr->bindTexture("skyboxPrefilterMap", 1);
	textureMgr->bindTexture("brdfLutMap", 2);
	textureMgr->bindTexture("sphere3_basecolor", 3);
	textureMgr->bindTexture("sphere3_metalness", 4);
	textureMgr->bindTexture("sphere3_normal", 5);
	textureMgr->bindTexture("sphere3_roughness", 6);

	Mesh::ptr mesh = std::shared_ptr<Mesh>(new Sphere(1, 25, 25));

	//draw pbr balls
	glm::mat4 model;
	for (int row = 0; row < 7; row++)
	{
		//shader->setFloat("metallic", (float)row / 7.0f);
		
		for (int col = 0; col < 7; col++)
		{
			//shader->setFloat("roughness", glm::clamp((float)col / 7.0f, 0.05f, 1.0f));
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((col - 7 / 2) * 2.5, (row - 7 / 2) * 2.5, 0.0f));
			shader->setMat4("model", model);
			mesh->draw();
		}
	}

	//draw four light ball
	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec3 newPos = lightPositions[i];
		shader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		shader->setMat4("model", model);
		mesh->draw();
	}

	//render skybox
	shader = shaderMgr->getShader("skyboxShader");
	shader->use();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	
	textureMgr->bindTexture("skyboxCubemap", 0);
	mesh->draw();

	//mesh = meshMgr->getMesh("Quad");
	//shader = shaderMgr->getShader("debug");

	//shader->use();
	//textureMgr->bindTexture("brdfLutMap", 0);
	//mesh->draw();

}

void PBRenderer::render()
{
	if (drawableList == nullptr)
		return;

	//set camera
	camera->setPerspectiveProject((float)width / (float)height, 0.1f, 1000.0f);

	updatePointLightPosition();

	//point light objects generation

	//render to g-buffers
	deferredShading->bindDeferredFramebuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	
	drawableList->render(camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//deferred shading and forward shading
	//gaussianBlur->bindGaussianFramebuffer();

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	deferredShading->renderDeferredShading(camera, sunLight, pointLights);

	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, deferredShading->getFrameBufferId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//deferredShading->ssaoFactorGeneration(camera);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	skyDome->render(camera);

	//point light
	if (pointLightDrawable != nullptr)
		pointLightDrawable->render(camera);

	//gaussianBlur->renderGaussianBlurEffect();
}


void PBRenderer::updatePointLightPosition()
{
	if (rndVelForPointLights.empty())
	{
		rndVelForPointLights.resize(pointLights.size());
		srand(time(nullptr));
		for (int i = 0; i < pointLights.size(); i++)
		{
			glm::vec3 vel;
			vel.x = (((double)rand()) / RAND_MAX) * 2.0f - 1.0f;
			vel.y = 0.0f;
			vel.z = (((double)rand()) / RAND_MAX) * 2.0f - 1.0f;
			rndVelForPointLights[i] = vel;
		}
	}

	const float speed = 2.0f;

	for (int i = 0; i < pointLights.size(); i++)
	{
		glm::vec3 pos = pointLights[i]->getPosition();
		pos += rndVelForPointLights[i] * speed;

		if (pos.x > 200.0f)
		{
			pos.x = 200.0f;
			rndVelForPointLights[i].x *= (-1.0f);
		}
		if (pos.x < -200.0f)
		{
			pos.x = -200.0f;
			rndVelForPointLights[i].x *= (-1.0f);
		}
		if (pos.z > 200.0f)
		{
			pos.z = 200.0f;
			rndVelForPointLights[i].z *= (-1.0f);
		}
		if (pos.z < -200.0f)
		{
			pos.z = -200.0f;
			rndVelForPointLights[i].z *= (-1.0f);

		}
		pointLights[i]->setPosition(pos, i);
	}
	 
	pointLightDrawable->setPointLightPositions(pointLights);
}