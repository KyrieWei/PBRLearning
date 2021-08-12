#include "Scene.h"
#include "../objects/Geometry.h"
#include "../postprocess/IBLAuxiliary.h"
#include "../drawable/StaticModelDrawable.h"

void simpleScene::initializeScene(PBRenderer::ptr pbrrenderer)
{
	MeshMgr::ptr meshMgr = pbrrenderer->getMeshMgr();
	ShaderMgr::ptr shaderMgr = pbrrenderer->getShaderMgr();
	TextureMgr::ptr textureMgr = pbrrenderer->getTextureMgr();

	unsigned int sphereMeshIndex = meshMgr->loadMesh(new Sphere(1, 64, 64), "Sphere");
	unsigned int cubeMeshIndex = meshMgr->loadMesh(new Cube(1, 1, 1), "Cube");
	unsigned int quadMeshIndex = meshMgr->loadMesh(new ScreenQuad(), "Quad");

	//shaders
	unsigned int debugShader = shaderMgr->loadShader("debug", "shaders/debug_vert.vs", "shaders/debug_frag.fs");
	unsigned int pbrShader = shaderMgr->loadShader("pbrShader", "shaders/pbr_vert.vs", "shaders/pbr_frag.fs");
	unsigned int hdrToCubemapShader = shaderMgr->loadShader("hdrToCubeShader", "shaders/hdrToCubemap_vert.vs", "shaders/hdrToCubemap_frag.fs");
	unsigned int skyboxShader = shaderMgr->loadShader("skyboxShader", "shaders/skybox_vert.vs", "shaders/skybox_frag.fs");
	unsigned int skyboxConvShader = shaderMgr->loadShader("skyboxConvShader", "shaders/irradiance_convolution_vert.vs", "shaders/irradiance_convolution_frag.fs");
	unsigned int skyboxPrefilterShader = shaderMgr->loadShader("skyboxPrefilterShader", "shaders/prefilter_env_map_vert.vs", "shaders/prefilter_env_map_frag.fs");
	unsigned int skyboxPrecomputeBRDFShader = shaderMgr->loadShader("skyboxPrecomputeBRDFShader", "shaders/precompute_brdfLut_vert.vs", "shaders/precompute_brdfLut_frag.fs");

	//pbr shader
	Shader::ptr shader = shaderMgr->getShader("pbrShader");
	shader->use();
	shader->setInt("irradianceMap", 0);
	shader->setInt("prefilterMap", 1);
	shader->setInt("brdfLUT", 2);

	//hdr to cube map
	shader = shaderMgr->getShader("hdrToCubeShader");
	shader->use();
	shader->setInt("equirectangularMap", 0);

	//conv sky box shader
	shader = shaderMgr->getShader("skyboxConvShader");
	shader->use();
	shader->setInt("environmentMap", 0);

	//prefilter sky box specular
	shader = shaderMgr->getShader("skyboxPrefilterShader");
	shader->use();
	shader->setInt("environmentMap", 0);


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
	unsigned int pbrShader = shaderMgr->loadShader("pbrShader", "shaders/pbr_vert.vs", "shaders/pbr_frag.fs");

	//models
	StaticModelDrawable* lamp = new StaticModelDrawable(pbrShader, "assets/lamp/lamp.obj");

	unsigned int planeMeshIndex = meshMgr->loadMesh(new Plane(900, 900), "Plane");
	unsigned int sphereMeshIndex = meshMgr->loadMesh(new Sphere(10, 25, 25), "Sphere");

	PBRMaterial mat;

	pbrrenderer->addDrawable(lamp);

}
