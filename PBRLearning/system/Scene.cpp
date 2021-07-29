#include "Scene.h"
#include "../objects/Sphere.h"
#include <GLFW/glfw3.h>

Scene::~Scene()
{
	if (lightPositions != NULL)
		delete[] lightPositions;
	if (lightColors != NULL)
		delete[] lightColors;
}

void Scene::initializeScene()
{
	lightPositions = new glm::vec3[4]
	{
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	lightColors = new glm::vec3[4]
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

}


void Scene::render(const Camera& camera, unsigned int scr_width, unsigned int scr_height)
{
	shader.use();
	
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)scr_width / (float)scr_height, 0.1f, 100.0f);

	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("viewPos", camera.Position);

	shader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	shader.setFloat("ao", 1.0f);

	glm::mat4 model;
	for (int row = 0; row < 7; row++)
	{
		shader.setFloat("metallic", (float)row / 7.0f);
		
		for (int col = 0; col < 7; col++)
		{
			shader.setFloat("roughness", glm::clamp((float)col / 7.0f, 0.05f, 1.0f));
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((col - 7 / 2) * 2.5, (row - 7 / 2) * 2.5, 0.0f));
			shader.setMat4("model", model);
			sphere.render_sphere();
		}
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		shader.setMat4("model", model);
		sphere.render_sphere();
	}
	
}