#pragma once
#include <vector>
#include <map>

#include "../tools/Singleton.h"
#include "../tools/Shader.h"

class ShaderMgr : public Singleton<ShaderMgr>
{
private:
	std::vector<Shader::ptr> objects;
	std::map<std::string, unsigned int> objectMap;

public:
	typedef std::shared_ptr<ShaderMgr> ptr;

	ShaderMgr() = default;
	~ShaderMgr() = default;

	static ShaderMgr::ptr getSingleton();

	unsigned int loadShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
	{
		if (objectMap.find(name) != objectMap.end())
			return objectMap[name];

		Shader::ptr shader(new Shader(vertex_path.c_str(), fragment_path.c_str()));
		objects.push_back(shader);
		objectMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	unsigned int loadShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
	{
		if (objectMap.find(name) != objectMap.end())
			return objectMap[name];

		Shader::ptr shader(new Shader(vertex_path.c_str(), fragment_path.c_str(), geometry_path.c_str()));
		objects.push_back(shader);
		objectMap[name] = objects.size() - 1;
		return objects.size() - 1;
	}

	Shader::ptr getShader(const std::string& name)
	{
		if (objectMap.find(name) == objectMap.end())
			return nullptr;
		return objects[objectMap[name]];
	}

	Shader::ptr getShader(unsigned int index)
	{
		if (index >= objects.size())
			return nullptr;
		return objects[index];
	}

	bool bindShader(unsigned int unit)
	{
		if (unit >= objects.size())
			return false;
		objects[unit]->use();
		return true;
	}

	bool bindShader(const std::string& name)
	{
		if (objectMap.find(name) == objectMap.end())
			return false;
		objects[objectMap[name]]->use();
		return true;
	}
};

