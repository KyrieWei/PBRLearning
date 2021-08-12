#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "Texture.h"
#include "../tools/Singleton.h"

class TextureMgr : public Singleton<TextureMgr>
{
private:
	std::vector<Texture::ptr> objects;
	std::map<std::string, unsigned int> objectsMap;

public:
	typedef std::shared_ptr<TextureMgr> ptr;

	TextureMgr() = default;
	~TextureMgr() = default;

	static TextureMgr::ptr getSingleton();

	unsigned int loadTexture2D(const std::string& name, const std::string& path, glm::vec4 bColor = glm::vec4(1.0f))
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new Texture2D(path, bColor));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadTexture2D(const std::string& name, unsigned char* data, int width, int height, int channel)
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new Texture2D(data, width, height, channel));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadTexture2DHDR(const std::string& name, const std::string& path)
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new Texture2DHDR(path));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadTexture2DHDRRaw(const std::string& name, const char* data, int width, int height)
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new Texture2DHDR(data, width, height));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadTextureCube(const std::string& name, const std::string& path, const std::string& pFix)
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new TextureCube(path, pFix));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadTextureCubeHDR(const std::string& name, const char* data, int width, int height, bool mipmap = false)
	{
		if (objectsMap.find(name) != objectsMap.end())
			return objectsMap[name];
		Texture::ptr tex(new TextureCubeHDR(data, width, height, mipmap));
		objects.push_back(tex);
		objectsMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	Texture::ptr getTexture(const std::string& name)
	{
		if (objectsMap.find(name) == objectsMap.end())
			return nullptr;
		return objects[objectsMap[name]];
	}

	Texture::ptr getTexture(unsigned int unit)
	{
		if (unit >= objects.size())
			return nullptr;
		return objects[unit];
	}

	bool bindTexture(unsigned int index, unsigned int unit)
	{
		if (index >= objects.size())
			return false;
		objects[index]->bind(unit);
		return true;
	}

	bool bindTexture(const std::string& name, unsigned int unit)
	{
		if (objectsMap.find(name) == objectsMap.end())
			return false;
		objects[objectsMap[name]]->bind(unit);
		return true;
	}

	bool unbindTexture(unsigned int index)
	{
		if (index >= objects.size())
			return false;
		objects[index]->unbind();
		return true;
	}

	bool unbindTexture(const std::string& name)
	{
		if (objectsMap.find(name) == objectsMap.end())
			return false;
		objects[objectsMap[name]]->unbind();
		return true;
	}
};

