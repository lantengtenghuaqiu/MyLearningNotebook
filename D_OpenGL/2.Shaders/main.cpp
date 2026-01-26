#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>

#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
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
    unsigned int mutable width_attri = 0;
    unsigned int mutable height_attrib = 0;

public:
    explicit AutoFile(const char *path, const char *mode)
    {
        this->file = fopen(path, mode);
        if (file == NULL)
        {
            printf("Wrong : file is empty");
        }
    }
    ~AutoFile()
    {
        if (this->file)
        {
            fclose(this->file);
        }
    }

    AutoFile(const AutoFile &) = delete;
    AutoFile &operator=(const AutoFile &) = delete;

    FILE *get() const
    {
        return this->file;
    }

    unsigned int width()
    {
        return this->width_attri;
    }
    unsigned int height()
    {
        return this->height_attrib;
    }

    unsigned char *ReadFile() const
    {
        if (this->get() == NULL)
        {
            printf("File is Wrong");
            return NULL;
        }

        fseek(this->get(), 0, SEEK_END);
        long file_size = ftell(this->get());

        if (file_size == 0)
        {
            printf("Wrong .File_size is 0 ");
            return NULL;
        }
        fseek(this->get(), 0, SEEK_SET);
        unsigned char *contents = new unsigned char[file_size + 1]();

        fread(contents, 1, file_size, this->get());

        {
            // unsigned int type = -1;
            {
                bool is_png = true;
                unsigned char png_sign[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
                for (int i = 0; i < 8; i++)
                {
                    if (contents[i] != png_sign[i])
                    {
                        is_png = false;
                        break;
                    }
                }
                if (is_png)
                {
                    this->width_attri = (unsigned int)contents[16] << 24 |
                                        (unsigned int)contents[17] << 16 |
                                        (unsigned int)contents[18] << 8 |
                                        (unsigned int)contents[19];

                    this->height_attrib = (unsigned int)contents[20] << 24 |
                                          (unsigned int)contents[21] << 16 |
                                          (unsigned int)contents[22] << 8 |
                                          (unsigned int)contents[23];
                    printf("%d\n", this->width_attri);
                    printf("%d\n", this->height_attrib);
                }
                else
                {
                    printf("Wrong is not png\n");
                }
            }
        }
        return contents;
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

// 项目初始化异常缓冲区
bool InitProjection()
{
    AutoFile file("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\maizhimeng.png", "rb");
    if (file.get())
    {
        file.ReadFile();

        return true;
    }
    printf("Something Wrong in init Projection\n");
    return false;
}
//---------------------------------------

int main()
{
    char * absoulte_path[512];
    absoulte_path = _fullpath("../0047.png",absoulte_path,sizeof(absoulte_path));

    printf("%s",*absoulte_path);
    if (!InitProjection())
    {
        exit(0);
    }
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
    GLFWwindow *window = glfwCreateWindow(width, height, "Shaders", nullptr, nullptr);
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
    unsigned int EBO;
    glGenVertexArrays(2, VAO);

    // 创建材质贴图Index
    unsigned int texture_id[2];
    glGenTextures(2, texture_id);

    // 进行临时数据加载和绑定逻辑
    {
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

        const char *vertex_shader = ReadFile("G:/user/desktop/C++/GraphicLearning/D_OpenGL/2.Shaders/shaders.vertex");
        const char *fragment_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\2.Shaders\\shaders.fragment");

        int width, height, nrChannels;
        unsigned char *texture_data = stbi_load("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\2.Shaders\\0047.png", &width, &height, &nrChannels, 0);

        //--------------------------------------------------------

        // 创建Vertex buffer object index:---------------------------------------
        unsigned int VBO[2];

        glGenBuffers(2, VBO);
        glGenBuffers(1, &EBO);
        //----------------------------------------------------------------------

        // 绑定顶点缓冲区1:-------------------------------------------------------
        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 绑定vertices layout 0:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glEnableVertexAttribArray(0);

        // 解绑:
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 绑定color layout 1:
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_coordinates), color_coordinates, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
        glEnableVertexAttribArray(1);
        // color_coordinates layout 2:
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)4);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //----------------------------------------------------------------------

        // 加载texture:-------------------------------------------------------
        glBindTexture(GL_TEXTURE_2D, texture_id[0]);
        // 为当前texture[0]设置属性
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture_data);
        //----------------------------------------------------------------------

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

        // unsigned _texture_id = glGetUniformLocation(shader_program_id, "texture2d");
        // glUniform1i(_texture_id, 0);
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

        // Events
        glfwPollEvents();

        // 绘制第1个三角形:
        // glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id[0]);

        // Drawing:
        glUseProgram(shader_program_id);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 绘制第2个三角形:
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap front and back of frame buffer
        glfwSwapBuffers(window);
    }
    
    return 0;
}