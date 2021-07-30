#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 color;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	Vertex() = default;

	Vertex(float px, float py, float pz, float nx, float ny, float nz,
		float u, float v, float r, float g, float b,
		float tx = 0, float ty = 0, float tz = 0, float bx = 0, float by = 0, float bz = 0)
	{
		position = glm::vec3(px, py, pz);
		normal = glm::vec3(nx, ny, nz);
		texcoord = glm::vec2(u, v);
		color = glm::vec3(r, g, b);
		tangent = glm::vec3(tx, ty, tz);
		bitangent = glm::vec3(bx, by, bz);
	}
};


class Mesh
{
private:
	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

public:
	typedef std::shared_ptr<Mesh> ptr;

	Mesh() = default;

	virtual ~Mesh();

	std::vector<Vertex>& getVertices() { return vertices; }
	std::vector<unsigned int>& getIndices() { return indices; }

	unsigned int getVAO() const { return VAO; }
	unsigned int getVBO() const { return VBO; }
	unsigned int getEBO() const { return EBO; }

	void draw() const;

protected:
	void setupMesh(const std::vector<Vertex> vertices_, const std::vector<unsigned int>& indices_);
	void clearMesh();
};

