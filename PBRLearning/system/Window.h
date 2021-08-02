#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../tools/Singleton.h"
#include "Scene.h"
#include "PBRenderer.h"

class Window : public Singleton<Window>
{
private:
	int width, height;
	std::string title;
	GLFWwindow* window;
	PBRenderer::ptr pbrenderer;
	simpleScene::ptr simplescene;
public:
	typedef std::shared_ptr<Window> ptr;

	//input key
	static float deltaTime, lastFrame;
	static float lastX, lastY;
	static bool firstMouse;

	static Window::ptr getSingleton();

public:
	Window() = default;
	~Window() = default;

	PBRenderer::ptr getPBRenderer() const { return pbrenderer; }

	void setWindowSize(unsigned int width_, unsigned int height_);
	void setWindowTitle(const std::string& title);

	void initialize();
	void beginFrame();
	void endFrame();
	void run();

protected:

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	void processInput(GLFWwindow* window, float deltaTime);
};

