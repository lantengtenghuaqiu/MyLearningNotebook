#include "../../Include/Window/CreateWindow.hpp"
// Glfw------------------------------------------------------------------------

GLFW *GLFW::GetInstance()
{
    static GLFW instance;
    return &instance;
}

int GLFW::InitGlfw(GLFWwindow *&window, const int &width, const int &height, const char *title, GLFWmonitor *glfwmonitor, GLFWwindow *share)
{

    printf("Initing The Glfw\n");
    if (glfwInit() != FAIL)
    {
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
        glfwWindowHint(GLFW_DECORATED, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DEPTH_BITS, 32);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_SAMPLES, 4);
#endif
        window = glfwCreateWindow(width, height, title, glfwmonitor, share);

        if (window != NULL)
        {
            glfwMakeContextCurrent(window);
            printf("Init Glfw Succeed\n");

        }
        else
        {
            printf("window is NULL!!!\n");
            glfwTerminate();
        }
    }
    else
    {
        printf("glfwInit with something wrong,initiation is failed!!!\n");
        glfwTerminate();
        return FAIL;
    }

    return SUCCESS;
}
GLFW::~GLFW()
{
    glfwTerminate();
}
// ----------------------------------------------------------------------------
