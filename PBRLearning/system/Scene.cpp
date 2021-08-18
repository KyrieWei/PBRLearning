#include "Scene.h"
#include "../objects/Geometry.h"
#include "../postprocess/IBLAuxiliary.h"
#include "../drawable/StaticModelDrawable.h"

void simpleScene::initializeScene(PBRenderer::ptr pbrrenderer)
{
	MeshMgr::ptr meshMgr = pbrrenderer->getMeshMgr();
	ShaderMgr::ptr shaderMgr = pbrrenderer->getShaderMgr();
	TextureMgr::ptr textureMgr = pbrrenderer->getTextureMgr();

	unsigned int sphereMeshIndex = meshMgr->loadMesh(new Sphere(1, 64, 64));
	unsigned int cubeMeshIndex = meshMgr->loadMesh(new Cube(1, 1, 1));
	unsigned int quadMeshIndex = meshMgr->loadMesh(new ScreenQuad());

	//shaders
	unsigned int debugShader = shaderMgr->loadShader("debug", "shaders/debug_vert.vs", "shaders/debug_frag.fs");
	unsigned int pbrShader = shaderMgr->loadShader("pbrShader", "shaders/pbr_vert.vs", "shaders/pbr_frag.fs");
	unsigned int skyboxShader = shaderMgr->loadShader("skyboxShader", "shaders/skybox_vert.vs", "shaders/skybox_frag.fs");

	//pbr shader
	Shader::ptr shader = shaderMgr->getShader("pbrShader");
	shader->use();
	shader->setInt("irradianceMap", 0);
	shader->setInt("prefilterMap", 1);
	shader->setInt("brdfLUT", 2);

	//sky box shader
	shader = shaderMgr->getShader("skyboxShader");
	shader->use();
	shader->setInt("environmentMap", 0);

	//debug shader
	shader = shaderMgr->getShader("debug");
	shader->use();
	shader->setInt("debug_texture", 0);

	//textures
	unsigned int equirectangularMap = textureMgr->loadTexture2DHDR("equirectangularMap", "assets/HDR/newport_loft.hdr");
	unsigned int cube_tex_index = textureMgr->loadTextureCubeHDR("skyboxCubemap", nullptr, 512, 512);
	unsigned int cube_conv_tex_index = textureMgr->loadTextureCubeHDR("skyboxConvCubemap", nullptr, 512, 512);
	unsigned int cube_prefilter_tex_index = textureMgr->loadTextureCubeHDR("skyboxPrefilterMap", nullptr, 256, 256, true);
	unsigned int brdfLut_tex_index = textureMgr->loadTexture2DHDRRaw("brdfLutMap", nullptr, 512, 512);

	//convert hdrmap to cube map
	IBLAuxiliary::convertToCubemap(512, 512, equirectangularMap, cube_tex_index);
	//convolute hdrmap
	IBLAuxiliary::convoluteDiffuseIntegral(512, 512, cube_tex_index, cube_conv_tex_index);
	//prefilter environment map 
	IBLAuxiliary::convoluteSpecularIntegral(256, 256, cube_tex_index, cube_prefilter_tex_index);
	//precompute brdfLut
	IBLAuxiliary::convoluteSpecularBRDFIntegral(512, 512, brdfLut_tex_index);
}


void FelLordScene::initializeScene(PBRenderer::ptr pbrrenderer)
{
	MeshMgr::ptr meshMgr = pbrrenderer->getMeshMgr();
	ShaderMgr::ptr shaderMgr = pbrrenderer->getShaderMgr();
	TextureMgr::ptr textureMgr = pbrrenderer->getTextureMgr();

	//shaders 
	unsigned int simpleShader = shaderMgr->loadShader("simpleShader", "shaders/diffuse_shader_vert.vs", "shaders/diffuse_shader_frag.fs");
	unsigned int pbrShader = shaderMgr->loadShader("pbrShader", "shaders/pbr_vert.vs", "shaders/pbr_frag.fs");

	//models
	StaticModelDrawable* lamp = new StaticModelDrawable(simpleShader, "assets/lamp/lamp.obj", "lamp");
	StaticModelDrawable* nanosuit = new StaticModelDrawable(simpleShader, "assets/nanosuit/nanosuit.obj", "nanosuit");

	unsigned int planeMeshIndex = meshMgr->loadMesh(new Plane(900, 900));
	unsigned int sphereMeshIndex = meshMgr->loadMesh(new Sphere(1, 25, 25));

	PBRMaterial sphereMat1;
	sphereMat1.albedoTexIndex = textureMgr->loadTexture2D("sphere1_basecolor", "assets/greasy-pan-2/greasy-pan-2-albedo.png");
	sphereMat1.metallicIndex = textureMgr->loadTexture2D("sphere1_metalness", "assets/greasy-pan-2/greasy-pan-2-metal.png");
	sphereMat1.normalTexIndex = textureMgr->loadTexture2D("sphere1_normal", "assets/greasy-pan-2/greasy-pan-2-normal.png");
	sphereMat1.roughTexIndex = textureMgr->loadTexture2D("sphere1_roughness", "assets/greasy-pan-2/greasy-pan-2-roughness.png");

	PBRMaterial sphereMat2;
	sphereMat2.albedoTexIndex = textureMgr->loadTexture2D("sphere2_basecolor", "assets/harshbricks/harshbricks-albedo.png");
	sphereMat2.metallicIndex = textureMgr->loadTexture2D("sphere2_metalness", "assets/harshbricks/harshbricks-metalness.png");
	sphereMat2.normalTexIndex = textureMgr->loadTexture2D("sphere2_normal", "assets/harshbricks/harshbricks-normal.png");
	sphereMat2.roughTexIndex = textureMgr->loadTexture2D("sphere2_roughness", "assets/harshbricks/harshbricks-roughness.png");

	PBRMaterial floorMat;
	floorMat.albedoTexIndex = textureMgr->loadTexture2D("floor_basecolor", "assets/floorMat/bathroomtile2-basecolor.png");
	floorMat.metallicIndex = textureMgr->loadTexture2D("floor_metalness", "assets/floorMat/bathroomtile2-metalness.png");
	floorMat.normalTexIndex = textureMgr->loadTexture2D("floor_normal", "assets/floorMat/bathroomtile2-normal-dx.png");
	floorMat.roughTexIndex = textureMgr->loadTexture2D("floor_roughness", "assets/floorMat/bathroomtile2-roughness.png");

	SimpleDrawable* sphere1 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere2 = new SimpleDrawable(simpleShader);

	SimpleDrawable* floor = new SimpleDrawable(simpleShader);

	sphere1->addMesh(sphereMeshIndex);
	sphere2->addMesh(sphereMeshIndex);
	floor->addMesh(planeMeshIndex);
	
	sphere1->addPbrTexture(sphereMat1);
	sphere2->addPbrTexture(sphereMat2);
	floor->addPbrTexture(floorMat);

	lamp->getTransformation()->setScale(glm::vec3(1.0f));
	lamp->getTransformation()->setTranslation(glm::vec3(-30.0f, 0.0f, 30.0f));

	nanosuit->getTransformation()->setScale(glm::vec3(5.0f));

	sphere1->getTransformation()->setScale(glm::vec3(7.0f));
	sphere1->getTransformation()->setTranslation(glm::vec3(30.0f, 10.0f, 40.0f));

	sphere2->getTransformation()->setScale(glm::vec3(7.0f));
	sphere2->getTransformation()->setTranslation(glm::vec3(10.0f, 10.0f, 40.0f));

	pbrrenderer->addDrawable(lamp);
	pbrrenderer->addDrawable(nanosuit);
	pbrrenderer->addDrawable(sphere1);
	pbrrenderer->addDrawable(sphere2);
	pbrrenderer->addDrawable(floor);


	srand(time(nullptr));
	for (unsigned int i = 0; i < 128; i++)
	{
		glm::vec3 pos;
		glm::vec3 radiance;

		pos.x = -200.0f + (((double)rand()) / RAND_MAX) * 400.0f;
		pos.y = 3.0f;
		pos.z = -200.0f + (((double)rand()) / RAND_MAX) * 400.0f;
		radiance.x = (((double)rand()) / RAND_MAX) * 80;
		radiance.y = (((double)rand()) / RAND_MAX) * 80;
		radiance.z = (((double)rand()) / RAND_MAX) * 80;

		pbrrenderer->addPointLight(pos, radiance);
	}

	pbrrenderer->setSunLight(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f));
}
