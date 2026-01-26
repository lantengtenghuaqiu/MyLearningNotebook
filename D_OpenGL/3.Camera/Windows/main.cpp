#include <stdio.h>
// #include
// #incldue

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
// g++ main.cpp ..\src\glad.c -I ..\include -L ..\lib -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -Werror=conversion  -std=c++17 -o main.exe

#define FAIL 0
#define SUCCESS 1

class GLFW
{
public:
    GLFWwindow *window;
    GLFW() {}

    int InitGlfw(const int &width, const int &height, const char *title, GLFWmonitor *glfwmonitor = nullptr, GLFWwindow *share = nullptr)
    {
        if (glfwInit() != FAIL)
        {
            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
            glfwWindowHint(GLFW_DECORATED, GL_TRUE);
            glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            this->window = glfwCreateWindow(width, height, title, glfwmonitor, share);

            if (this->window != NULL)
            {
                glfwMakeContextCurrent(this->window);
            }
            else
            {
                printf("window is NULL!!! : %d", __LINE__);
                glfwTerminate();
            }
        }
        else
        {
            printf("glfwInit with something wrong,initiation is failed!!! : %d", __LINE__);
            glfwTerminate();
        }

        return SUCCESS;
    }

    ~GLFW()
    {

        glfwTerminate();
    }
};

typedef struct BufferBinderAttributes
{
    int BUFFERTYPE = 0;
    int DRAWYPE = 0;

    int Location = -1;
    int DataSize = 0;
    int DATATYPE = 0;

    unsigned char NORMILAZED = GL_FALSE;
    int Stride = 0;
    int StartPointer = 0;
    bool AttributeChecker() const
    {
        if (this->BUFFERTYPE != 0 && this->DRAWYPE != 0 && this->Location >= 0 && DataSize > 0)
            return true;
        else
            return false;
    }

} Binder;

template <typename T>
class GLAD
{
public:
    int status = 0;

    Binder binder;
    unsigned int VAO[2] = {0};
    unsigned int EBO[2] = {0};
    unsigned int TEX[2] = {0};
    GLAD()
    {
    }
    GLAD(Binder binder)
    {
        this->binder = binder;
    }
    void InitGlad(const int width, const int height)
    {

        this->status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (this->status == SUCCESS)
        {
            glViewport(0, 0, width, height);
            glGenVertexArrays(2, this->VAO);
            glGenBuffers(2, this->EBO);
            glGenTextures(2, this->TEX);
        }
        else
        {
            printf("Init the glad failed : %d", __LINE__);
        }
    }

    void BindBuffers(unsigned int &VAO, const Binder binder, const T *data, unsigned int &EBO = NULL, unsigned int *index = nullptr, int ebo_draw_type = GL_STATIC_DRAW)
    {

        if (binder.AttributeChecker())
        {
            unsigned VBO;
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(binder.BUFFERTYPE, VBO);
            glBufferData(binder.BUFFERTYPE, sizeof(data), data, binder.DRAWYPE);
            if (EBO)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, ebo_draw_type);
            }

            glVertexAttribPointer(binder.Location, binder.DataSize, binder.DATATYPE, binder.NORMILAZED, sizeof(T) * binder.Stride, (void *)binder.StartPointer);
            // 第二个参数表示分量vec2传2, vec3传3, vec4传4
            glEnableVertexAttribArray(binder.Location);
            glBindBuffer(binder.BUFFERTYPE, 0);
        }
    }

    void UnbindCurrentVAO()
    {
        glBindVertexArray(0);
    }
};

static int width = 1960;
static int height = 1280;

template <typename T>
struct Entity
{
public:
    GLFW glfw;
    GLAD<T> glad;
};
// Entity = 1280;

// 进行临时数据加载和绑定逻辑

// 准备顶点数据------------------------------------------------
const float vertices[] = {
    // positions
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
const float color_coordinates[] = {
    0.5f, 0.5f, 0.0f, // vertices color p0
    1.0f, 1.0f,       // texture coordinates p0

    1.0f, 0.0f, 0.0f, // vertices color p1
    1.0f, 0.0f,       // texture coordinates p1

    0.0f, 0.0f, 1.0f, // vertices color p2
    0.0f, 0.0f,       // texture coordinates p2

    0.0f, 1.0f, 1.0f, // vertices color p3
    0.0f, 1.0f        // texture coordinates p3
};

int main()
{
    Entity<float> entity;
    if (entity.glfw.InitGlfw(width, height, "Camera") == SUCCESS)
    {
        entity.glad.InitGlad(width, height);

        Binder binder;
        {
            binder.BUFFERTYPE = GL_ARRAY_BUFFER;
            binder.DATATYPE = GL_FLOAT;
            binder.DRAWYPE = GL_STATIC_DRAW;
            binder.Location = 0;
            binder.NORMILAZED = GL_FALSE;
            binder.Stride = 3;
            binder.StartPointer = 0;
        }
        entity.glad.BindBuffers(entity.glad.VAO[0], binder, vertices, entity.glad.EBO[0],indices);
        entity.glad.UnbindCurrentVAO();

        // 设置每帧重置属性
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClearDepth(0.0f);
        glClearStencil(0.0f);
        while (!glfwWindowShouldClose(entity.glfw.window))
        {
            glfwPollEvents();
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);
            glClear(GL_STENCIL_BUFFER_BIT);
            glfwSwapBuffers(entity.glfw.window);
        }

        glfwTerminate();
    }
    else
    {
        printf("Init the glfw failed");
    }

    return 0;
}
