#include "Window.h"

float Window::deltaTime = 0.0f;
float Window::lastFrame = 0.0f;
float Window::lastX = 0.0f;
float Window::lastY = 0.0f;
bool Window::firstMouse = true;
template<> Window::ptr Singleton<Window>::_instance = nullptr;

Window::ptr Window::getSingleton()
{
	if (!_instance)
		_instance = std::make_shared<Window>();
	return _instance;
}

void Window::setWindowSize(unsigned int width_, unsigned int height_)
{
    width = width_;
    height = height_;

    lastX = width / 2.0f;
    lastY = height / 2.0f;
}

void Window::setWindowTitle(const std::string& str)
{
    title = str;
}

void Window::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    pbrenderer = std::make_shared<PBRenderer>();
    pbrenderer->initialzie(width, height);

    //simplescene = std::shared_ptr<simpleScene>(new simpleScene());
    simplescene = std::make_shared<simpleScene>();
    simplescene->initializeScene(pbrenderer);
}

void Window::beginFrame()
{

}

void Window::endFrame()
{
    glfwTerminate();
}

void Window::run()
{

    initialize();

    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pbrenderer->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endFrame();
}














void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    getSingleton()->getPBRenderer()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    getSingleton()->getPBRenderer()->getCamera()->ProcessMouseScroll(yoffset);
}

void Window::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera::ptr camera = getSingleton()->getPBRenderer()->getCamera();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}