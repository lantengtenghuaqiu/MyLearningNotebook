#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"


class ReadFile
{
private:
    FILE *file;

public:
    void ReatText(char *const &path, char *const &mode, char *&content)
    {
        this->file = fopen(path, mode);
        if (this->file)
        {
            fseek(this->file, 0, SEEK_END);
            long content_size = ftell(this->file);
            fseek(this->file, 0, SEEK_SET);
            if (content_size > 0)
            {
                content = new char[content_size + 1]();
                fread(content, 1, content_size, this->file);

                if (content)
                    fclose(this->file);
                else
                {
                    fclose(this->file);
                    printf("Content is something wrong\n");
                }
            }
            else
            {
                fclose(this->file);
                printf("File content is something wrong\n");
            }
        }
        else
        {
            printf("Opening file is fail\n");
        }
    }
};

static const int screen_width = 800;
static const int screen_height = 600;
static const char *window_title = "HelloOpenGL";

void checkShaderCompileError(unsigned int shader, const char *type)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("%s SHADER COMPILATION FAILED:\n%s\n", type, infoLog);
    }
}
void checkProgramLinkError(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("SHADER PROGRAM LINK FAILED:\n%s\n", infoLog);
    }
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, window_title, nullptr, nullptr);

    glfwMakeContextCurrent(window);

    // 初始化Glad
    int SUCCESS;
    SUCCESS = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, screen_width, screen_height);

    if (SUCCESS == 0)
        return -1;

    // 创建shader program
    unsigned int shaderProgram = glCreateProgram();
    // 创建VAO
    unsigned int VAO[2] = {0};
    glGenVertexArrays(2, VAO);
    std::cout << "VAO[0]:" << VAO[0] << std::endl;
    std::cout << "VAO[0]:" << VAO[1] << std::endl;

    // 创建EBO
    unsigned int EBO[2] = {0};
    glGenBuffers(2, EBO);
    std::cout << "EBO[0]:" << EBO[0] << std::endl;
    std::cout << "EBO[0]:" << EBO[1] << std::endl;

    // 创建材质索引
    unsigned int textureIndex[2] = {0};
    glGenTextures(2, textureIndex);

    // 处理顶点
    {
        // 准备顶点信息:
        const float vertices_position[] = {
            // positions
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };

        const float vertices_UV_Color[] = {
            0.5f, 0.5f, 0.0f, // vertices color p0
            1.0f, 1.0f,       // texture coordinates p0

            1.0f, 0.0f, 0.0f, // vertices color p1
            1.0f, 0.0f,       // texture coordinates p1

            0.0f, 0.0f, 1.0f, // vertices color p2
            0.0f, 0.0f,       // texture coordinates p2

            0.0f, 1.0f, 1.0f, // vertices color p3
            0.0f, 1.0f        // texture coordinates p3
        };

        // 准备EBO数据
        const unsigned int vertices_element_index[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        // 创建VBO
        unsigned int VBO[2] = {0};
        glGenBuffers(2, VBO);
        std::cout << "VBO[0]:" << VBO[0] << std::endl;
        // 绑定VAO[0]:
        glBindVertexArray(VAO[0]);

        // 绑定EBO[0]
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_element_index), vertices_element_index, GL_STATIC_DRAW);

        // 绑定VBO[0]
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

        // 配置读取顶点信息顺序,并传入对应的location
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 绑定VBO[1]
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_UV_Color), vertices_UV_Color, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        glDeleteBuffers(1, VBO);
    }

    {
        // 创建Shader
        // 读取Vertex shader文本文件:
        char *content = nullptr;
        char *path = nullptr;
        char *mode = nullptr;
        ReadFile file;

        // 创建Vertex shader
        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        {
            path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Mac/HelloOpenGL/vertexshader.vs";
            mode = "rb";
            file.ReatText(path, mode, content);
            glShaderSource(vertex_shader, 1, &content, NULL);
            glCompileShader(vertex_shader);
            checkShaderCompileError(vertex_shader, "vertex");
            glAttachShader(shaderProgram, vertex_shader);
            for (int i = 0; i < 60; i++)
            {
                std::cout << content[i];
            }
            std::cout << std::endl;

            delete[] (content);
            content = NULL;
        }

        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        {
            path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Mac/HelloOpenGL/fragmentshader.fs";
            mode = "rb";
            file.ReatText(path, mode, content);
            glShaderSource(fragment_shader, 1, &content, NULL);
            glCompileShader(fragment_shader);
            checkShaderCompileError(fragment_shader, "fragmentshader");
            glAttachShader(shaderProgram, fragment_shader);

            delete (content);
            content = NULL;
        }
        
        glLinkProgram(shaderProgram);
        checkProgramLinkError(shaderProgram);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        // 处理图片
        // 配置贴图数据
        {
            glBindTexture(GL_TEXTURE_2D, textureIndex[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            int width, height, nChanel;
            path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/maizhimeng.png";
            unsigned char *image_date = stbi_load(path, &width, &height, &nChanel, 4);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_date);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(image_date);
        }
        //处理所有uniform数据传入
        {
        //    const xyl::mat4x4<float> transform = xyl::transform::M(2.0,10.0,0.0,0.0,1.0,0.0,2.0);
            // unsigned int uniform_transform = glGetUniformLocation(shaderProgram , "transform");
            // glUniformMatrix4fv(uniform_transform , 1 , GL_FALSE,&transform);

        }
    }

    // unsigned int textureLocation[2];
    // textureLocation[0] = glGetUniformLocation(shaderProgram, "MaiZhiMeng");
    // glUniform1i(textureLocation[0], 0);

    glUseProgram(shaderProgram);

    glClearColor(1.0f, 0.8f, 0.8f, 1.0f);
    glClearDepth(1.0f);
    glClearStencil(0.0f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, textureIndex[0]);
        // glUniform1i(textureLocation[0], 0);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}