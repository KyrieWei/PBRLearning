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
	shader->setInt("albedoMap", 3);
	shader->setInt("normalMap", 4);
	shader->setInt("metallicMap", 5);
	shader->setInt("roughtnessMap", 6);


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

	unsigned int albedoTexIndex = textureMgr->loadTexture2D("sphere3_basecolor", "assets/streaked-metal1/streaked-metal1-albedo.png");
	unsigned int metallicIndex = textureMgr->loadTexture2D("sphere3_metalness", "assets/streaked-metal1/streaked-metal1-metalness.png");
	unsigned int normalTexIndex = textureMgr->loadTexture2D("sphere3_normal", "assets/streaked-metal1/streaked-metal1-normal-dx.png");
	unsigned int roughTexIndex = textureMgr->loadTexture2D("sphere3_roughness", "assets/streaked-metal1/streaked-metal1-rough.png");

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

	PBRMaterial sphereMat3;
	sphereMat3.albedoTexIndex = textureMgr->loadTexture2D("sphere3_basecolor", "assets/streaked-metal1/streaked-metal1-albedo.png");
	sphereMat3.metallicIndex = textureMgr->loadTexture2D("sphere3_metalness", "assets/streaked-metal1/streaked-metal1-metalness.png");
	sphereMat3.normalTexIndex = textureMgr->loadTexture2D("sphere3_normal", "assets/streaked-metal1/streaked-metal1-normal-dx.png");
	sphereMat3.roughTexIndex = textureMgr->loadTexture2D("sphere3_roughness", "assets/streaked-metal1/streaked-metal1-rough.png");

	PBRMaterial sphereMat4;
	sphereMat4.albedoTexIndex = textureMgr->loadTexture2D("sphere4_basecolor", "assets/copper_scuffed/Copper-scuffed_basecolor.png");
	sphereMat4.metallicIndex = textureMgr->loadTexture2D("sphere4_metalness", "assets/copper_scuffed/Copper-scuffed_metallic.png");
	sphereMat4.normalTexIndex = textureMgr->loadTexture2D("sphere4_normal", "assets/copper_scuffed/Copper-scuffed_normal.png");
	sphereMat4.roughTexIndex = textureMgr->loadTexture2D("sphere4_roughness", "assets/copper_scuffed/Copper-scuffed_roughness.png");

	PBRMaterial sphereMat5;
	sphereMat5.albedoTexIndex = textureMgr->loadTexture2D("sphere5_basecolor", "assets/gold/albedo.png");
	sphereMat5.metallicIndex = textureMgr->loadTexture2D("sphere5_metalness", "assets/gold/metallic.png");
	sphereMat5.normalTexIndex = textureMgr->loadTexture2D("sphere5_normal", "assets/gold/normal.png");
	sphereMat5.roughTexIndex = textureMgr->loadTexture2D("sphere5_roughness", "assets/gold/roughness.png");

	PBRMaterial sphereMat6;
	sphereMat6.albedoTexIndex = textureMgr->loadTexture2D("sphere6_basecolor", "assets/grass/albedo.png");
	sphereMat6.metallicIndex = textureMgr->loadTexture2D("sphere6_metalness", "assets/grass/metallic.png");
	sphereMat6.normalTexIndex = textureMgr->loadTexture2D("sphere6_normal", "assets/grass/normal.png");
	sphereMat6.roughTexIndex = textureMgr->loadTexture2D("sphere6_roughness", "assets/grass/roughness.png");

	PBRMaterial sphereMat7;
	sphereMat7.albedoTexIndex = textureMgr->loadTexture2D("sphere7_basecolor", "assets/plastic/albedo.png");
	sphereMat7.metallicIndex = textureMgr->loadTexture2D("sphere7_metalness", "assets/plastic/metallic.png");
	sphereMat7.normalTexIndex = textureMgr->loadTexture2D("sphere7_normal", "assets/plastic/normal.png");
	sphereMat7.roughTexIndex = textureMgr->loadTexture2D("sphere7_roughness", "assets/plastic/roughness.png");

	PBRMaterial sphereMat8;
	sphereMat8.albedoTexIndex = textureMgr->loadTexture2D("sphere8_basecolor", "assets/rusted_iron/albedo.png");
	sphereMat8.metallicIndex = textureMgr->loadTexture2D("sphere8_metalness", "assets/rusted_iron/metallic.png");
	sphereMat8.normalTexIndex = textureMgr->loadTexture2D("sphere8_normal", "assets/rusted_iron/normal.png");
	sphereMat8.roughTexIndex = textureMgr->loadTexture2D("sphere8_roughness", "assets/rusted_iron/roughness.png");


	PBRMaterial floorMat;
	floorMat.albedoTexIndex = textureMgr->loadTexture2D("floor_basecolor", "assets/floorMat/bathroomtile2-basecolor.png");
	floorMat.metallicIndex = textureMgr->loadTexture2D("floor_metalness", "assets/floorMat/bathroomtile2-metalness.png");
	floorMat.normalTexIndex = textureMgr->loadTexture2D("floor_normal", "assets/floorMat/bathroomtile2-normal-dx.png");
	floorMat.roughTexIndex = textureMgr->loadTexture2D("floor_roughness", "assets/floorMat/bathroomtile2-roughness.png");

	SimpleDrawable* sphere1 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere2 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere3 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere4 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere5 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere6 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere7 = new SimpleDrawable(simpleShader);
	SimpleDrawable* sphere8 = new SimpleDrawable(simpleShader);

	SimpleDrawable* floor = new SimpleDrawable(simpleShader);

	sphere1->addMesh(sphereMeshIndex);
	sphere2->addMesh(sphereMeshIndex);
	sphere3->addMesh(sphereMeshIndex);
	sphere4->addMesh(sphereMeshIndex);
	sphere5->addMesh(sphereMeshIndex);
	sphere6->addMesh(sphereMeshIndex);
	sphere7->addMesh(sphereMeshIndex);
	sphere8->addMesh(sphereMeshIndex);

	floor->addMesh(planeMeshIndex);
	
	sphere1->addPbrTexture(sphereMat1);
	sphere2->addPbrTexture(sphereMat2);
	sphere3->addPbrTexture(sphereMat3);
	sphere4->addPbrTexture(sphereMat4);
	sphere5->addPbrTexture(sphereMat5);
	sphere6->addPbrTexture(sphereMat6);
	sphere7->addPbrTexture(sphereMat7);
	sphere8->addPbrTexture(sphereMat8);

	floor->addPbrTexture(floorMat);

	lamp->getTransformation()->setScale(glm::vec3(1.0f));
	lamp->getTransformation()->setTranslation(glm::vec3(-30.0f, 0.0f, 30.0f));

	nanosuit->getTransformation()->setScale(glm::vec3(5.0f));
	nanosuit->getTransformation()->setTranslation(glm::vec3(30.0f, 0.0f, 0.0f));

	sphere1->getTransformation()->setScale(glm::vec3(7.0f));
	sphere1->getTransformation()->setTranslation(glm::vec3(10.0f, 10.0f, 40.0f));

	sphere2->getTransformation()->setScale(glm::vec3(7.0f));
	sphere2->getTransformation()->setTranslation(glm::vec3(30.0f, 10.0f, 40.0f));

	sphere3->getTransformation()->setScale(glm::vec3(7.0f));
	sphere3->getTransformation()->setTranslation(glm::vec3(50.0f, 10.0f, 40.0f));

	sphere4->getTransformation()->setScale(glm::vec3(7.0f));
	sphere4->getTransformation()->setTranslation(glm::vec3(70.0f, 10.0f, 40.0f));

	sphere5->getTransformation()->setScale(glm::vec3(7.0f));
	sphere5->getTransformation()->setTranslation(glm::vec3(10.0f, 30.0f, 40.0f));

	sphere6->getTransformation()->setScale(glm::vec3(7.0f));
	sphere6->getTransformation()->setTranslation(glm::vec3(30.0f, 30.0f, 40.0f));

	sphere7->getTransformation()->setScale(glm::vec3(7.0f));
	sphere7->getTransformation()->setTranslation(glm::vec3(50.0f, 30.0f, 40.0f));

	sphere8->getTransformation()->setScale(glm::vec3(7.0f));
	sphere8->getTransformation()->setTranslation(glm::vec3(70.0f, 30.0f, 40.0f));

	pbrrenderer->addDrawable(lamp);
	pbrrenderer->addDrawable(nanosuit);
	pbrrenderer->addDrawable(sphere1);
	pbrrenderer->addDrawable(sphere2);
	pbrrenderer->addDrawable(sphere3);
	pbrrenderer->addDrawable(sphere4);
	pbrrenderer->addDrawable(sphere5);
	pbrrenderer->addDrawable(sphere6);
	pbrrenderer->addDrawable(sphere7);
	pbrrenderer->addDrawable(sphere8);
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

	pbrrenderer->addPointLightDrawble();

	pbrrenderer->setSunLight(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f));
}
