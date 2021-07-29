#pragma once
#include "../tools/Shader.h"
#include "../tools/Camera.h"
#include "../objects/Sphere.h"

class Scene
{
public:
	glm::vec3* lightPositions;
	glm::vec3* lightColors;

	Shader shader;

	Sphere sphere;

public:
	Scene() : shader("shaders/pbr_vert.vs", "shaders/pbr_frag.fs") {}
	~Scene();

	void initializeScene();
	void render(const Camera& camera, unsigned int scr_width, unsigned int scr_height);
};