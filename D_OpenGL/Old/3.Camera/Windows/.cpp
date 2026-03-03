#include <stdio.h>
// #include
// #incldue

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "../includes/xyl_tools.hpp"
// g++ main.cpp ..\src\glad.c -I ..\include -L ..\lib -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -Werror=conversion  -std=c++17 -o main.exe
// clang++ main.cpp ../src/glad.c  -I ../includes -L ../libs -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo  -o main
#define FAIL 0
#define SUCCESS 1

class GLFW
{
public:
    GLFWwindow *window;
    GLFW() {}
    int InitGlfw(const int &width, const int &height, const char *title, GLFWmonitor *glfwmonitor = nullptr, GLFWwindow *share = nullptr)
    {
        printf("Init Glfw\n");
        if (glfwInit() != FAIL)
        {
            printf("Init Glfw Succeed\n");

            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
            glfwWindowHint(GLFW_DECORATED, GL_TRUE);
            glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            this->window = glfwCreateWindow(width, height, title, glfwmonitor, share);

            if (this->window != NULL)
            {
                glfwMakeContextCurrent(this->window);
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
    int BUFFERTYPE = GL_ARRAY_BUFFER;
    int DRAWYPE = GL_STATIC_DRAW;

    int Location = -1;
    int DataSize = 3;
    int DATATYPE = GL_FLOAT;

    unsigned char NORMILAZED = GL_FALSE;
    int Stride = 0;
    int StartPointer = 0;
    bool AttributeChecker() const
    {
        if (this->DRAWYPE != 0 && this->Location >= 0 && DataSize > 0)
            return true;
        else
            return false;
    }

} Binder;

typedef struct ObjectIndex
{
    unsigned int *VAO;
    unsigned int *VBO;
    unsigned int *EBO;
    unsigned int *TEX;

    ObjectIndex() {}

    ObjectIndex(unsigned int sizeVAO, unsigned int sizeVBO, unsigned int sizeEBO, unsigned int sizeTEX)
    {
        this->VAO = new unsigned int[sizeVAO];
        this->VBO = new unsigned int[sizeVBO];
        this->EBO = new unsigned int[sizeEBO];
        this->TEX = new unsigned int[sizeTEX];
    }
    ~ObjectIndex()
    {

        delete[] (this->VBO);
        this->VBO = nullptr;

        delete[] (this->EBO);
        this->EBO = nullptr;

        delete[] (this->TEX);
        this->TEX = nullptr;

        delete[] (this->VAO);
        this->VAO = nullptr;
    }
} ObjectID;

typedef struct ShadersProgram
{
    unsigned int ShaderProgram = 0;
    unsigned int VertexShader = 0;
    unsigned int FragmentShader = 0;
    char *ShaderData;
    ShadersProgram()
    {
        ShaderData = new char;
        this->ShaderProgram = glCreateProgram();
        this->VertexShader = glCreateShader(GL_VERTEX_SHADER);
        this->FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    }

} Shader;

template <typename T>
class GLAD
{
public:
    int status = 0;

    GLAD() {}

    void InitGlad(const int width, const int height)
    {
        printf("Init Glad\n");
        this->status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (this->status == SUCCESS)
        {
            printf("Init Glad Succeed\n");
            glViewport(0, 0, width, height);
        }
        else
        {
            printf("Init the glad failed\n");
        }
    }
    void BindVAO(unsigned int &VAO)
    {
        printf("Bind VAO : %d\n", VAO);
        glBindVertexArray(VAO);
    }

    void BindEBO(unsigned int &EBO, const int bytesize, const unsigned int *index, int ebo_draw_type = GL_STATIC_DRAW)
    {
        printf("Bind EBO : %d\n", EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesize, index, ebo_draw_type);
    }

    void BindVBO(unsigned int &VBO, const int bytesize, const Binder binder, const T *data)
    {
        if (binder.AttributeChecker())
        {
            printf("Bind VBO : %d\n", VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, bytesize, data, binder.DRAWYPE);
            // 第二个参数表示分量vec2传2, vec3传3, vec4传4
            glVertexAttribPointer(binder.Location, binder.DataSize, binder.DATATYPE, binder.NORMILAZED, sizeof(float) * binder.Stride, (void *)binder.StartPointer);
            glEnableVertexAttribArray(binder.Location);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void UnbindVAO()
    {
        glBindVertexArray(0);
    }

    void CompileAndAttachShader(unsigned int shader_program, unsigned int shader_id, char *shader_data, char *shader_type)
    {
        printf("Compile Shader %s : %d\n", shader_type, shader_id);

        glShaderSource(shader_id, 1, &shader_data, NULL);
        glCompileShader(shader_id);

        int checker;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &checker);

        if (checker == FAIL)
        {
            ShaderChecker
        }
        else
        {
            printf("Attached shader program(%d -> %d).\n", shader_id, shader_program);
            glAttachShader(shader_program, shader_id);
        }
    }
    void LinkeShaderPorgram(unsigned int shader_program)
    {
        glLinkProgram(shader_program);
        int checker;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &checker);
        if (checker == FAIL)
        {
            LinkChecker
        }
    }

    void UseShaderProgram(unsigned int shader_program)
    {
        if (shader_program != FAIL)
            glUseProgram(shader_program);
        else
            printf("Shader program has something wrong\n");
    }

    void DeleteShaders(unsigned int shader)
    {
        glDeleteShader(shader);
    }
};

static int width = 860;
static int height = 720;

// 准备顶点数据------------------------------------------------
const float vertices[] = {
    // positions
    0.5f, 0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f   // top left
};
const unsigned int indices[] = {
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
    GLFW glfw;
    GLAD<float> glad;
    ObjectID attri(2, 2, 2, 2);
    Tools file;
    if (glfw.InitGlfw(width, height, "OpenGLClass"))
    {
        glad.InitGlad(width, height);



        glGenVertexArrays(2, attri.VAO);
        glGenBuffers(2, attri.EBO);
        glGenBuffers(2, attri.VBO);

        glad.BindVAO(attri.VAO[0]);
        glad.BindEBO(attri.EBO[0], sizeof(indices), indices, GL_STATIC_DRAW);

        Binder VBO0;
        {
            VBO0.DATATYPE = GL_FLOAT;
            VBO0.DataSize = 3;
            VBO0.DRAWYPE = GL_STATIC_DRAW;
            VBO0.Location = 0;
            VBO0.NORMILAZED = GL_FALSE;
            VBO0.Stride = 3;
            VBO0.StartPointer = 0;
        }
        glad.BindVBO(attri.VBO[0], sizeof(vertices), VBO0, vertices);

        glad.UnbindVAO();

        Shader shaders;
        // Vertex Shader
        file.GetContent("/Users/bytedance/Desktop/C++/OJSN%!A12YHA/A_Mac/A_OpenGLClass/shaders.vertex", "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.VertexShader, shaders.ShaderData, "vertex shader");

        // Fragment Shader
        file.GetContent("/Users/bytedance/Desktop/C++/OJSN%!A12YHA/A_Mac/A_OpenGLClass/shaders.fragment", "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.FragmentShader, shaders.ShaderData, "fragment shader");

        // Link Shader Program
        glad.LinkeShaderPorgram(shaders.ShaderProgram);
        glad.UseShaderProgram(shaders.ShaderProgram);


        


        // 设置每帧重置属性
        glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
        glClearDepth(0.0f);
        glClearStencil(0.0f);

        while (!glfwWindowShouldClose(glfw.window))
        {
            glfwPollEvents();
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glUseProgram(shaders.ShaderProgram);
            glBindVertexArray(attri.VAO[0]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

            glfwSwapBuffers(glfw.window);
        }
        // gld
        glad.DeleteShaders(shaders.VertexShader);
        glad.DeleteShaders(shaders.FragmentShader);

        glfwTerminate();
    }
    else
    {
        printf("Init the glfw failed");
    }

    return 0;
}
