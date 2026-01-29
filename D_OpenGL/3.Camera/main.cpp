#include <stdio.h>
#include <vector>
// #include
// #incldue
#define STB_IMAGE_IMPLEMENTATION

#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"

#include "../includes/stb_image.h"
#include "../glm/glm.hpp"
#include "../includes/xyl_tools.hpp"
#include "../includes/Camera.hpp"
#include "./MVP.hpp"

// g++ main.cpp ..\..\src\glad.c -I ..\..\includes -L ..\..\libs -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -Werror=conversion  -std=c++17 -o main.exe
// clang++ main.cpp ../src/glad.c  -I ../includes -L ../libs -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo  -o main

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

typedef struct ObjectAttributes
{
    unsigned int *VAO;
    unsigned int *VBO;
    unsigned int *EBO;
    unsigned int *TEX;
    unsigned int sizeVAO;
    unsigned int sizeVBO;
    unsigned int sizeEBO;
    unsigned int sizeTEX;
    ObjectAttributes() {}

    ObjectAttributes(unsigned int sizeVAO, unsigned int sizeVBO, unsigned int sizeEBO, unsigned int sizeTEX)
    {
        this->sizeVAO = sizeVAO;
        this->sizeVBO = sizeVBO;
        this->sizeEBO = sizeEBO;
        this->sizeTEX = sizeTEX;
        this->VAO = new unsigned int[sizeVAO];
        this->VBO = new unsigned int[sizeVBO];
        this->EBO = new unsigned int[sizeEBO];
        this->TEX = new unsigned int[sizeTEX];
    }
    ~ObjectAttributes()
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
} Attributes;

typedef struct TextureAttribute
{
    int TEXTURE_TYPE = GL_TEXTURE_2D;

} Sampler2D;

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
        if (true)
        {
            printf("Bind VBO : %d\n\n", VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, bytesize, data, binder.DRAWYPE);
            // 第二个参数表示分量vec2传2, vec3传3, vec4传4
            glVertexAttribPointer(binder.Location, binder.DataSize, binder.DATATYPE, binder.NORMILAZED, sizeof(float) * binder.Stride, (void *)binder.StartPointer);
            glEnableVertexAttribArray(binder.Location);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else
        {
            printf("Something wrong in binding VBO\n");
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
            // ShaderChecker
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
            // LinkChecker
        }
    }

    void UseShaderProgram(unsigned int shader_program)
    {
        if (shader_program != FAIL)
        {
            printf("Use the shader program : %d\n", shader_program);
            glUseProgram(shader_program);
        }
        else
            printf("Shader program has something wrong\n");
    }

    void DeleteShaders(unsigned int shader)
    {
        glDeleteShader(shader);
    }
    int GetUniformLoaction(const unsigned int sahder_program, const char *keyword)
    {
        return glGetUniformLocation(sahder_program, keyword);
    }
};

static int width = 860;
static int height = 720;

// 准备顶点数据------------------------------------------------

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

const unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
const float uv[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f};

int main()
{
    GLFW glfw;
    GLAD<float> glad;
    Attributes attri(2, 2, 1, 1);

    ReadFile file;
    if (glfw.InitGlfw(width, height, "OpenGLClass"))
    {
        glad.InitGlad(width, height);

        glGenVertexArrays(attri.sizeVAO, attri.VAO);
        glGenBuffers(attri.sizeEBO, attri.EBO);
        glGenBuffers(attri.sizeVBO, attri.VBO);
        glGenTextures(attri.sizeTEX, attri.TEX);

        glad.BindVAO(attri.VAO[0]);
        glad.BindEBO(attri.EBO[0], sizeof(indices), indices, GL_STATIC_DRAW);


        printf("Bind VBO : %d\n\n", attri.VBO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[0]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
        glEnableVertexAttribArray(0);


        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glad.UnbindVAO();

        Shader shaders;
        // Vertex Shader
        file.GetContent("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\3.Camera\\shaders.vertex", "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.VertexShader, shaders.ShaderData, "vertex shader");

        // Fragment Shader
        file.GetContent("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\3.Camera\\shaders.fragment", "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.FragmentShader, shaders.ShaderData, "fragment shader");

        // Link Shader Program
        glad.LinkeShaderPorgram(shaders.ShaderProgram);
        glad.UseShaderProgram(shaders.ShaderProgram);
        //-----------------------------------------------------------------------------

        {
            int _color = glGetUniformLocation(shaders.ShaderProgram, "_color");
            float _Color[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glUniform4fv(_color, 1, _Color);
            printf("Get uniform of _Color : %d\n", _color);
            //传入MVP矩阵
            int _identity = glGetUniformLocation(shaders.ShaderProgram, "_identity");
            printf("Get uniform of identity : %d\n", _identity);
            glUniformMatrix4fv(_identity, 1, GL_FALSE, identity);
        }
        // Set Textures

        glBindTexture(GL_TEXTURE_2D, attri.TEX[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        int width, height, nChanel;
        stbi_set_flip_vertically_on_load(true); 
        const char *path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\maizhimeng.png";
        unsigned char *image_date = stbi_load(path, &width, &height, &nChanel, 4);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image_date);

        unsigned int textureLocation[2];
        textureLocation[0] = glGetUniformLocation(shaders.ShaderProgram, "_MaiZhiMeng");
        glUniform1i(textureLocation[0], 0);

        //-----------------------------------------------------------------------------

        // 设置每帧重置属性
        glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
        glClearDepth(0.0f);
        glClearStencil(0.0f);

        while (!glfwWindowShouldClose(glfw.window))
        {
            glfwPollEvents();
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // Textures-----------------------------------
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, attri.TEX[0]);
            glUniform1i(textureLocation[0], 0);

            //-------------------------------------------

            // Draw Triangles-----------------------------
            glUseProgram(shaders.ShaderProgram);
            glBindVertexArray(attri.VAO[0]);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //-------------------------------------------

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
