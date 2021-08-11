#pragma once
#include <map>
#include <string>
#include <iostream>
#include "Mesh.h"
#include "../tools/Singleton.h"

class MeshMgr : public Singleton<MeshMgr>
{
private:
	std::vector<Mesh::ptr> objects;
	std::map <std::string, unsigned int> objectMap;
public:
	typedef std::shared_ptr<MeshMgr> ptr;

	static MeshMgr::ptr getSingleton();

	unsigned int loadMesh(Mesh* mesh, const std::string& name)
	{
		if (objectMap.find(name) != objectMap.end())
		{
			std::cout << "Mesh name duplicated, failed to create mesh " + name << std::endl;
			return -1;
		}

		Mesh::ptr mptr(mesh);
		objects.push_back(mptr);
		objectMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	Mesh::ptr getMesh(unsigned int index)
	{
		if (index > objects.size())
			return nullptr;
		return objects[index];
	}

	Mesh::ptr getMesh(const std::string& name)
	{
		if (objectMap.find(name) == objectMap.end())
		{
			std::cout << "no mesh named " << name << ", failed to get mesh!" << std::endl;
			return nullptr;
		}

		return objects[objectMap[name]];
	}

	bool drawMesh(unsigned int index)
	{
		if (index >= objects.size())
			return false;
		objects[index]->draw();
		return true;
	}

};

