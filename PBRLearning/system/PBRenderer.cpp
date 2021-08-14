#include "PBRenderer.h"
#include "../postprocess/IBLAuxiliary.h"
#include "../objects/Geometry.h"

void PBRenderer::initialzie(int _width, int _height)
{
	width = _width;
	height = _height;

	camera = std::make_shared<Camera>(Camera(glm::vec3(0.0f, 0.0f, 3.0f)));

	meshMgr = MeshMgr::getSingleton();
	shaderMgr = ShaderMgr::getSingleton();
	textureMgr = TextureMgr::getSingleton();

	drawableList = std::make_shared<DrawableList>();

	deferredShading = std::shared_ptr<DeferredShading>(new DeferredShading(width, height));

	camera->setInitialStatus(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(-0.2f, 1.0f, -0.4f), -480.0f, -24.0f);
	camera->setMovementSpeed(30.0f);
}

void PBRenderer::setSkyDomeHDR(const std::string& path)
{
	if (skyDome != nullptr)
		return;

	unsigned int skyboxShader = shaderMgr->loadShader("skybox", "shaders/skybox_vert.vs", "shaders/skybox_frag.fs");
	unsigned int hdrTexIndex = textureMgr->loadTexture2DHDR("hdrTex", path);
	unsigned int cubeTexIndex = textureMgr->loadTextureCubeHDR("skyboxCubemap", nullptr, 1024, 1024);

	//convert hdrmap to cubemap
	unsigned int hdrToCubemapShader = shaderMgr->loadShader("hdrToCubeShader", "shaders/hdrToCubemap_vert.vs", "shaders/hdrToCubemap_frag.fs");
	IBLAuxiliary::convertToCubemap(1024, 1024, hdrTexIndex, cubeTexIndex);

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

	Mesh::ptr mesh = std::shared_ptr<Mesh>(new Sphere(1, 25, 25));

	//draw pbr balls
	glm::mat4 model;
	for (int row = 0; row < 7; row++)
	{
		shader->setFloat("metallic", (float)row / 7.0f);
		
		for (int col = 0; col < 7; col++)
		{
			shader->setFloat("roughness", glm::clamp((float)col / 7.0f, 0.05f, 1.0f));
			
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

	//render to g-buffers
	deferredShading->bindDeferredFramebuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawableList->render(camera);
	

	//deferred shading
	deferredShading->renderDeferredShading(camera);

	skyDome->render(camera);
}
