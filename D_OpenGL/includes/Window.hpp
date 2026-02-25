#ifndef CALLBACK
#define CALLBACK
#include "./glad/glad.h"
#include "./GLFW/glfw3.h"
#include "./BasicIncludes.hpp"
#include "../includes/xyl_tools.hpp"
#include "./GlobalDatas.hpp"
#define ShaderChecker                               \
    printf("Wrong Compile (%d)\n", checker);        \
    char info[512];                                 \
    glGetShaderInfoLog(shader_id, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                   \
    {                                               \
        printf("%c", info[i]);                      \
    }                                               \
    printf("\n");

#define LinkChecker                                      \
    printf("Wrong Link (%d):\n", checker);               \
    char info[512];                                      \
    glGetShaderInfoLog(shader_program, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                        \
    {                                                    \
        printf("%c", info[i]);                           \
    }                                                    \
    printf("\n");

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
            glfwWindowHint(GLFW_DEPTH_BITS, 32);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);

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

    void BindEBO(unsigned int &EBO, const int data_size, const unsigned int *index, int ebo_draw_type = GL_STATIC_DRAW)
    {
        printf("Bind EBO : %d\n", EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, index, ebo_draw_type);
    }
    
    void BindVAO(unsigned int &VAO)
    {
        printf("VAO Index : %d \n", VAO);

        glBindVertexArray(VAO);
        // currentIndex += 1;
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

    void BindFBO()
    {
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
    inline void LinkeShaderPorgram(unsigned int shader_program)
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

void ReadVertexData(Vertices &vertex, float *vertices, unsigned long long size)
{
    vertex.vertices = vertices;
    vertex.size = size;
    if (vertex.vertices != NULL)
    {
        printf("Vertex Size : %llu\n", vertex.size);
    }
    else
        printf("Vertex is Null\n");
}

void BindVBO_WithNormalAndUV(Attributes &attri, const int &currentIndex, Vertices &vertex)
{
    printf("Bind VBO : %d\n", attri.VBO[currentIndex]);

    glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[currentIndex]);

    glBufferData(GL_ARRAY_BUFFER, vertex.size, vertex.vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer只是写VAO的规则,并不是存入数据,真正存入数据的是glBufferData,VAO只是从glBufferData中根据现在制定好的规则去读取数据.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // vertex.vertices=NULL;
}

void BindVBO(Attributes &attri, const int &currentIndex, Vertices &vertex)
{
    printf("Bind VBO : %d\n", attri.VBO[currentIndex]);

    glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[currentIndex]);

    glBufferData(GL_ARRAY_BUFFER, vertex.size, vertex.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void BindVBO_WithUV(Attributes &attri, const int &currentIndex, Vertices &vertex)
{
    printf("Bind VBO : %d\n", attri.VBO[currentIndex]);

    glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[currentIndex]);

    glBufferData(GL_ARRAY_BUFFER, vertex.size, vertex.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void SetTextures(Attributes *attri, TexAttri *texAttri, Shader shaders)
{
    unsigned char *image_date;
    const char *path;

    // Texture Albedo-----------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, attri->TEX[0]);
    printf("Bind Container Albedo TEX : %d\n", attri->TEX[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\container2.png";
    image_date = stbi_load(path, &texAttri[0].width, &texAttri[0].height, &texAttri[0].nChanel, 4);
    printf("Texture Container Albedo width : %d , height : %d , nChanel : %d\n", texAttri[0].width, texAttri[0].height, texAttri[0].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[0].width, texAttri[0].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);

    texAttri[0].textureLocation = glGetUniformLocation(shaders.Program, "_Conatiner");
    printf("Texture Container Albedo Id : %d\n", texAttri[0].textureLocation);

    // Texture Specular---------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, attri->TEX[1]);
    printf("Bind Conatiner Specular TEX : %d\n", attri->TEX[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\container2_specular.png";
    image_date = stbi_load(path, &texAttri[1].width, &texAttri[1].height, &texAttri[1].nChanel, 4);
    printf("Texture Conatiner Specular width : %d , height : %d , nChanel : %d\n", texAttri[1].width, texAttri[1].height, texAttri[1].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[1].width, texAttri[1].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);

    texAttri[1].textureLocation = glGetUniformLocation(shaders.Program, "_ConatinerSpecular");
    printf("Texture Conatiner Specular Id : %d\n", texAttri[1].textureLocation);
}

void SetBindLogoMesh(Attributes *attri, TexAttri *texAttri, Shader shaders)
{
    texAttri[2].textureLocation = glGetUniformLocation(shaders.Program, "_maizhimeng");
    printf("Texture Logo Mesh Id : %d\n", texAttri[2].textureLocation);

    unsigned char *image_date;
    const char *path;
    // Logo texture:
    glBindTexture(GL_TEXTURE_2D, attri->TEX[2]);
    printf("\nBind Logo Mesh TEX : %d\n", attri->TEX[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\maizhimeng.png";
    image_date = stbi_load(path, &texAttri[2].width, &texAttri[2].height, &texAttri[2].nChanel, 4);
    printf("Texture Logo Mesh width : %d , height : %d , nChanel : %d\n", texAttri[2].width, texAttri[2].height, texAttri[2].nChanel);
    stbi_set_flip_vertically_on_load(true);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texAttri[2].width, texAttri[2].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_date);
}
#endif
