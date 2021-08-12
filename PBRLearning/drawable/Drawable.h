#pragma once
#include <vector>
#include <memory>
#include "../tools/Shader.h"

class PBRMaterial
{
public:
	unsigned int albedoTexIndex = -1;
	unsigned int normalTexIndex = -1;
	unsigned int roughTexIndex = -1;
	unsigned int metallicIndex = -1;
};

class Drawable
{
protected:
	unsigned int shaderIndex;
	std::vector<unsigned int> meshIndex;
	std::vector<PBRMaterial> texIndex;

	bool visible = true;

public:
	typedef std::shared_ptr<Drawable> ptr;

	Drawable() = default;
	virtual ~Drawable() = default;

	virtual void render(Shader::ptr shader = nullptr) = 0;
	virtual void renderDepth(Shader::ptr shader) = 0;

protected:
	void renderImp();
};

class DrawableList : public Drawable
{
private:
	std::vector<Drawable::ptr> list;

public:
	typedef std::shared_ptr<DrawableList> ptr;

	DrawableList() = default;
	virtual ~DrawableList() = default;

	unsigned int addDrawable(Drawable* ptr)
	{
		Drawable::ptr drawable(ptr);
		list.push_back(drawable);
		return list.size() - 1;
	}

	virtual void render(Shader::ptr shader = nullptr)
	{
		for (auto& it : list)
			it->render(shader);
	}

	virtual void renderDepth(Shader::ptr shader)
	{
		for (auto& it : list)
			it->renderDepth(shader);
	}
};

