#pragma once
#pragma once
#include <vector>
#include <glm/glm.hpp>

class Sphere
{
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	std::vector<float> data;

	float radius;

	unsigned int X_SEGMENTS, Y_SEGMENTS;
	unsigned int indexCount, stride;
	
	unsigned int sphere_VAO, sphere_VBO, sphere_EBO;

public:
	Sphere();
	Sphere(unsigned int xSEG, unsigned int ySEG, float rad);
	~Sphere();

	void render_sphere();
};

