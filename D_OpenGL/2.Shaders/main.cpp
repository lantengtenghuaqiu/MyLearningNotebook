#include <stdio.h>
#include <stdlib.h>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

// #define printwrong printf("Some thing wrong %d",__LINE__);

char *ReadFile(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        fclose(file);
        printf("Wrong");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    if (file_size == 0)
    {
        fclose(file);
        printf("Wrong");
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    char *contents = new char[file_size + 1];

    // memset(contents,0,file_size+1);
    fread(contents, 1, file_size, file);

    contents[file_size + 1] = '\0';

    fclose(file);

    return contents;
}

int main()
{
    // 初始化glfw
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

    const int width = 1920;
    const int height = 1080;
    GLFWwindow *window = glfwCreateWindow(width, height, "Shaders", nullptr, nullptr);

    if (window == NULL)
    {
        exit(0);
    }

    glfwMakeContextCurrent(window);

    // Init glad
    int success;
    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (success == 0)
    {
        exit(0);
    }
    //准备数据------------------------------------------------
    const float vertices1[] = {-1.0, -2.0, 0.0,
                              0.5, 0.5, 0.0,
                              2.0, 0.5, 0.0};

    const float vertices2[] = {-1.0, -1.0, 0.0,
                              0.5, 1.0, 0.0,
                              2.0, 0.5, 0.0};

    const char *vertex_shader = ReadFile("G:/user/desktop/C++/GraphicLearning/D_OpenGL/2.Shaders/shaders.vertex");
    const char *fragment_shader = ReadFile("G:/user/desktop/C++/GraphicLearning/D_OpenGL/2.Shaders/shaders.fragment");
    //--------------------------------------------------------

    //创建vertex attribute object index:
    unsigned int VAO[2];
    glGenVertexArrays(2,VAO);

    //创建Vertex buffer object index:
    unsigned int VBO[2];
    glGenBuffers(2,VBO);

    //绑定顶点缓冲区1:
    glBindBuffer(GL_ARRAY_BUFFER , VBO[0]);
    glBufferData(GL_ARRAY_BUFFER , sizeof(vertices1),vertices1,GL_STATIC_DRAW);

    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    //绑定Vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id,1,&vertex_shader,NULL);
    glCompileShader(vertex_shader_id);

    //绑定Fragment shader
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id,1,&fragment_shader,NULL);
    glCompileShader(fragment_shader_id);

    //创建Shader program(程序)
    unsigned int shader_program_id = glCreateProgram();
    //链接Vertex shader和Fragment shader
    glAttachShader(shader_program_id,vertex_shader_id);
    glAttachShader(shader_program_id,fragment_shader_id);
    glLinkProgram(shader_program_id);

    glUseProgram(shader_program_id);

    //删除已经绑定到Shader程序的shader
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(0.0f);
    glClearStencil(0.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
        
        glfwPollEvents();

        //开始绘制:
        glUseProgram(shader_program_id);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES,0,3);

        glfwSwapBuffers(window);
    }

    return 0;
}