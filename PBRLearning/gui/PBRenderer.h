#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "../tools/Camera.h"
//#include "../tools/Shader.h"
//#include "../tools/Utility.h"

class PBRenderer
{
public:
	unsigned int width, height;
	std::string title;

	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;

	//time
	float deltaTime;
	float lastFrame;

public:
	PBRenderer();

	void setWindowSize(unsigned int width_, unsigned int height_);
	void setWindowTitle(const std::string& title);

	void run();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window, float deltaTime);

};

