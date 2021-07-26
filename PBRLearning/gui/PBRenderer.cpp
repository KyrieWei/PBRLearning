#include "PBRenderer.h"

PBRenderer::PBRenderer()
{
	width = 1280;
	height = 720;

	title = "PBRLearning";

	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	lastX = width / 2.0f;
	lastY = height / 2.0f;
	firstMouse = true;

	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

void PBRenderer::setWindowSize(unsigned int width_, unsigned int height_)
{
	width = width_;
	height = height_;

	lastX = width / 2.0f;
	lastY = height / 2.0f;
}

void PBRenderer::setWindowTitle(const std::string& str)
{
	title = str;
}


void PBRenderer::run()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    auto framebuffersizecallback = [](GLFWwindow* w, int width, int height)
    {
        static_cast<PBRenderer*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(w, width, height);
    };
    auto mousecallback = [](GLFWwindow* w, double xpos, double ypos)
    {
        static_cast<PBRenderer*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
    };
    auto scrollcallback = [](GLFWwindow* w, double xoffset, double yoffset)
    {
        static_cast<PBRenderer*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
    };

    glfwSetFramebufferSizeCallback(window, framebuffersizecallback);
    glfwSetCursorPosCallback(window, mousecallback);
    glfwSetScrollCallback(window, scrollcallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void PBRenderer::processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void PBRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void PBRenderer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void PBRenderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}