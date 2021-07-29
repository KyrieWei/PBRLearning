#include "Sphere.h"
#include "../tools/Maths.h"
#include <glad/glad.h>
#include <iostream>

Sphere::Sphere()
{
	X_SEGMENTS = 64;
	Y_SEGMENTS = 64;

	radius = 1.0;

	sphere_VAO = 0;
	sphere_VBO = 0;
	sphere_EBO = 0;

	for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;

			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos) * radius);
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; y++)
	{
		if (!oddRow)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; x++)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; x--)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	indexCount = indices.size();

	for (unsigned int i = 0; i < positions.size(); i++)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);

		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}

		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
	}

	stride = 3;

	if (normals.size() > 0)
		stride += 3;
	if (uv.size() > 0)
		stride += 2;

}

Sphere::Sphere(unsigned int xSEG, unsigned int ySEG, float rad)
{
	X_SEGMENTS = xSEG;
	Y_SEGMENTS = ySEG;

	radius = rad;

	sphere_VAO = 0;
	sphere_VBO = 0;
	sphere_EBO = 0;

	for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;

			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos) * radius);
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; y++)
	{
		if (!oddRow)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; x++)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (unsigned int x = X_SEGMENTS; x >= 0; x--)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	indexCount = indices.size();

	for (unsigned int i = 0; i < positions.size(); i++)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);

		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}

		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
	}

	stride = 3;

	if (normals.size() > 0)
		stride += 3;
	if (uv.size() > 0)
		stride += 2;

}

void Sphere::render_sphere()
{
	if (sphere_VAO == 0)
	{
		glGenVertexArrays(1, &sphere_VAO);
		glGenBuffers(1, &sphere_VBO);
		glGenBuffers(1, &sphere_EBO);

		glBindVertexArray(sphere_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
		
	}

	glBindVertexArray(sphere_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &sphere_VAO);
	glDeleteBuffers(1, &sphere_VBO);
	glDeleteBuffers(1, &sphere_EBO);
}