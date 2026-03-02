#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

// #define LOG_DELTTIME

#define printwrong(success)                     \
    if (!success)                               \
    {                                           \
        printf("Some thing wrong %d", __LINE__) \
    }

const static int width = 1920;
const static int height = 1080;

class AutoFile
{

private:
    FILE *file;

public:
    explicit AutoFile(const char *path, const char *mode)
    {
        this->file = fopen(path, mode);
        if (file == NULL)
        {
            printf("Wrong : file is empty");
        }
    }

    AutoFile(const AutoFile &) = delete;
    AutoFile &operator=(const AutoFile &) = delete;

    FILE *get() const
    {
        return this->file;
    }
    ~AutoFile()
    {
        if (this->file)
        {
            fclose(this->file);
        }
    }
};

char *ReadFile(const char *path)
{
    AutoFile file(path, "rb");

    if (file.get() == NULL)
    {

        printf("Wrong");
        return NULL;
    }

    fseek(file.get(), 0, SEEK_END);
    long file_size = ftell(file.get());

    if (file_size == 0)
    {
        printf("Wrong");
        return NULL;
    }
    fseek(file.get(), 0, SEEK_SET);
    char *contents = new char[file_size + 1]();

    fread(contents, 1, file_size, file.get());

    return contents;
}

int main()
{
    // 初始化glfw:
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    // 实例化窗口程序:
    static GLFWwindow *window = glfwCreateWindow(width, height, "Shaders", nullptr, nullptr);
    if (window == NULL)
        exit(0);

    // 建立当前上下文窗口:
    glfwMakeContextCurrent(window);

    // 初始化Glad库,获取驱动函数的地址:
    int success;
    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (success == 0)
        exit(0);

    // 创建全局内容
    //  创建Shader program(程序)
    unsigned int shader_program_id = glCreateProgram();

    // 创建vertex attribute object index:
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    // 进行临时数据加载和绑定逻辑
    {
        // 准备顶点数据------------------------------------------------
        const float vertices1[] = {
            0.2f, 0.2f, 0.0f, // vertices
            0.5f, 0.5f, 0.0f, // vertices color
            // 0.0f, 0.0f,         // texture coordinates

            0.8f, 0.2f, 0.0f,
            1.0f, 0.0f, 0.0f,
            // 1.0f, 0.0f,

            0.5f, 0.8f, 0.0f,
            0.0f, 0.0f, 1.0f
            // 0.5f, 1.0f
        };

        const float vertices2[] = {
            -0.8f, -0.8f, 0.0f,
            -0.2f, -0.8f, 0.0f,
            -0.5f, -0.2f, 0.0f};

        const char *vertex_shader = ReadFile("G:/user/desktop/C++/GraphicLearning/D_OpenGL/2.Shaders/shaders.vertex");
        const char *fragment_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\2.Shaders\\shaders.fragment");
        //--------------------------------------------------------

        // 创建Vertex buffer object index:---------------------------------------
        unsigned int VBO[2];
        glGenBuffers(2, VBO);
        //----------------------------------------------------------------------

        // 绑定顶点缓冲区1:-------------------------------------------------------
        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        // 绑定layout 0:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
        glEnableVertexAttribArray(0);

        // 绑定layout 1:
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 解绑:
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //----------------------------------------------------------------------

        // 绑定顶点缓冲区2:-------------------------------------------------------
        glBindVertexArray(VAO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

        // 绑定layout 0:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // 解绑:
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //----------------------------------------------------------------------

        // 绑定Shader:----------------------------------------------------------
        // 绑定Vertex shader
        unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
        glCompileShader(vertex_shader_id);

        // 绑定Fragment shader
        unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
        glCompileShader(fragment_shader_id);

        // 链接Shader program
        // 链接Vertex shader和Fragment shader
        glAttachShader(shader_program_id, vertex_shader_id);
        glAttachShader(shader_program_id, fragment_shader_id);
        glLinkProgram(shader_program_id);

        // 删除已经绑定到Shader程序的shader
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        vertex_shader = nullptr;
        fragment_shader = nullptr;
        glUseProgram(shader_program_id);
        //----------------------------------------------------------------------

        // 传递值:
        // 传递Uniform值
        unsigned _color_index = glGetUniformLocation(shader_program_id, "color");
        glUniform4f(_color_index, 1.0f, 0.0f, 0.0f, 1.0f);
    }

    // 设置每帧重置属性
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(0.0f);
    glClearStencil(0.0f);

    // 记录帧速率:--------------------------------------------------------
    auto first_frame_time = std::chrono::high_resolution_clock::now();
    auto last_frame_time = std::chrono::high_resolution_clock::now();
    auto current_frame_time = std::chrono::high_resolution_clock::now();
    float deltTime = 0;
    float time = 0;
    //-------------------------------------------------------------------

    // 绘制图像循环:
    while (!glfwWindowShouldClose(window))
    {
        // 帧速率:
        current_frame_time = std::chrono::high_resolution_clock::now();
        deltTime = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - last_frame_time).count();
        time = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - first_frame_time).count();
        last_frame_time = std::chrono::high_resolution_clock::now();

#ifdef LOG_DELTTIME
        printf("delt time: %f\n", deltTime);
        printf("time: %f\n", time);
#endif

        // Clearning  
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);

        //Events
        glfwPollEvents();

        // Drawing:
        glUseProgram(shader_program_id);
        
        // 绘制第1个三角形:
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 绘制第2个三角形:
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Swap front and back of frame buffer
        glfwSwapBuffers(window);
    }

    return 0;
}