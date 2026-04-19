#include "../../Include/Window/CreateWindow.hpp"
// Glad------------------------------------------------------------------------
int GLAD::InitGlad(GLFWwindow *window, int &frameWidth, int &frameHeight)
{
    printf("Initing The Glad\n");
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == SUCCESS)
    {
        printf("Init Glad Succeed\n");
        glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
        glViewport(0, 0, frameWidth, frameHeight);
        return SUCCESS;
    }
    else
    {
        printf("Init the glad failed\n");
        return FAIL;
    }
}

GLAD *GLAD::GetInstance()
{
    static GLAD instance;
    return &instance;
}

void GLAD::BindEBO(unsigned int &EBO, const int data_size, const unsigned int *index, int ebo_draw_type = GL_STATIC_DRAW)
{
    printf("Bind EBO : %d\n", EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, index, ebo_draw_type);
}

ShadersProgram::ShadersProgram()
{
    ShaderData = new char;
    this->Program = glCreateProgram();
    this->Vertex = glCreateShader(GL_VERTEX_SHADER);
    this->Fragment = glCreateShader(GL_FRAGMENT_SHADER);
}
// ----------------------------------------------------------------------------
