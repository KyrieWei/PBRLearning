#pragma once
#include "Mesh.h"
#include "../tools/Singleton.h"

class MeshMgr : public Singleton<MeshMgr>
{
private:
	std::vector<Mesh::ptr> objects;

public:
	typedef std::shared_ptr<MeshMgr> ptr;

	static MeshMgr::ptr getSingleton();

	unsigned int loadMesh(Mesh* mesh)
	{
		Mesh::ptr mptr(mesh);
		objects.push_back(mptr);
		return objects.size() - 1;
	}

	Mesh::ptr getMesh(unsigned int index)
	{
		if (index > objects.size())
			return nullptr;
		return objects[index];
	}

	bool drawMesh(unsigned int index)
	{
		if (index >= objects.size())
			return false;
		objects[index]->draw();
		return true;
	}

};

