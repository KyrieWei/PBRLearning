#include "Drawable.h"
#include "../objects/MeshMgr.h"
#include "../textures/TextureMgr.h"

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