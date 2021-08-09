#include "Scene.h"
#include "../objects/Geometry.h"
#include "../postprocess/IBLAuxiliary.h"

void simpleScene::initializeScene(PBRenderer::ptr pbrrenderer)
{
	MeshMgr::ptr meshMgr = pbrrenderer->getMeshMgr();
	ShaderMgr::ptr shaderMgr = pbrrenderer->getShaderMgr();
	TextureMgr::ptr textureMgr = pbrrenderer->getTextureMgr();

	unsigned int sphereMeshIndex = meshMgr->loadMesh(new Sphere(1, 64, 64));
	unsigned int cubeMeshIndex = meshMgr->loadMesh(new Cube(1, 1, 1));

	//shaders
	unsigned int pbrShader = shaderMgr->loadShader("pbrShader", "shaders/pbr_vert.vs", "shaders/pbr_frag.fs");
	unsigned int hdrToCubemapShader = shaderMgr->loadShader("hdrToCubeShader", "shaders/hdrToCubemap_vert.vs", "shaders/hdrToCubemap_frag.fs");
	unsigned int skyboxShader = shaderMgr->loadShader("skyboxShader", "shaders/skybox_vert.vs", "shaders/skybox_frag.fs");
	unsigned int skyboxConvShader = shaderMgr->loadShader("skyboxConvShader", "shaders/irradiance_convolution_vert.vs", "shaders/irradiance_convolution_frag.fs");
	unsigned int skyboxPrefilterShader = shaderMgr->loadShader("skyboxPrefilterShader", "shaders/prefilter_env_map_vert.vs", "shaders/prefilter_env_map_frag.fs");

	//pbr shader
	Shader::ptr shader = shaderMgr->getShader("pbrShader");
	shader->use();
	shader->setInt("irradianceMap", 0);

	//hdr to cube map
	shader = shaderMgr->getShader("hdrToCubeShader");
	shader->use();
	shader->setInt("equirectangularMap", 0);

	//conv sky box shader
	shader = shaderMgr->getShader("skyboxConvShader");
	shader->use();
	shader->setInt("environmentMap", 0);

	shader = shaderMgr->getShader("skyboxPrefilterShader");
	shader->use();
	shader->setInt("environmentMap", 0);

	//sky box shader
	shader = shaderMgr->getShader("skyboxShader");
	shader->use();
	shader->setInt("environmentMap", 0);

	//textures
	unsigned int equirectangularMap = textureMgr->loadTexture2DHDR("equirectangularMap", "assets/HDR/newport_loft.hdr");
	unsigned int cube_tex_index = textureMgr->loadTextureCubeHDR("skyboxCubemap", nullptr, 512, 512);
	unsigned int cube_conv_tex_index = textureMgr->loadTextureCubeHDR("skyboxConvCubemap", nullptr, 512, 512);
	unsigned int cube_prefilter_tex_index = textureMgr->loadTextureCubeHDR("skyboxPrefilterMap", nullptr, 256, 256, true);

	//convert hdrmap to cube map
	IBLAuxiliary::convertToCubemap(512, 512, equirectangularMap, cube_tex_index);
	//convolute hdrmap
	IBLAuxiliary::convoluteDiffuseIntegral(512, 512, cube_tex_index, cube_conv_tex_index);
	//prefilter environment map 
	IBLAuxiliary::convoluteSpecularIntegral(256, 256, cube_tex_index, cube_prefilter_tex_index);
}

