#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vert, const std::vector<unsigned int>& indi)
{
	setupMesh(vert, indi);
}

Mesh::~Mesh()
{
	clearMesh();
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Mesh::setupMesh(const std::vector<Vertex> vertices_, const std::vector<unsigned int>& indices_)
{
	vertices = vertices_;
	indices = indices_;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	//vertex texture coord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoord)));

	//vertex color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	//vertex tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));

	//vertex bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	glBindVertexArray(0);
}

void Mesh::clearMesh()
{
	vertices.clear();
	indices.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::generateTangentAndBitangent(const glm::vec3& normal, glm::vec3& tangent, glm::vec3& bitangent)
{
	glm::vec3 up = glm::vec3(0, 1, 0);
	if (normal == up)
		up = glm::vec3(1, 0, 0);
	tangent = glm::cross(normal, up);
	bitangent = glm::cross(tangent, normal);
}