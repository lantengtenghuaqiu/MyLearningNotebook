#include <vector>
#include <chrono>

#include "../includes/Window.hpp"
#include "../includes/Transformation.hpp"
#include "../includes/Event.hpp"
// g++ main.cpp ..\src\glad.c -I ..\includes -L ..\libs -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -pedantic -std=c++17 -o main.exe
// clang++ main.cpp ../src/glad.c  -I ../includes -L ../libs -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo  -o main

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

float speed = 0.0f;

int main()
{
    GLFW glfw;
    GLAD<float> glad;
    ObjectID attri(2, 2, 1, 1);

    Tools::TheFile file;

    SceneRoot objects(2);

    TexAttri *texAttri = new TexAttri[2];

    if (glfw.InitGlfw(width, height, "OpenGLClass"))
    {
        glad.InitGlad(width, height);

        glGenVertexArrays(attri.sizeVAO, attri.VAO);
        glGenBuffers(attri.sizeEBO, attri.EBO);
        glGenBuffers(attri.sizeVBO, attri.VBO);
        glGenTextures(attri.sizeTEX, attri.TEX);

        glad.BindVAO(attri.VAO[0]);
        glad.BindEBO(attri.EBO[0], sizeof(indices), indices, GL_STATIC_DRAW);

        Vertices *vertex = new Vertices[2];
        vertex[0].vertices = vertices;
        vertex[0].size = sizeof(vertices);
        VBO(attri, vertex[0]);

        Shader shaders;
        // Vertex Shader
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\3.Camera\\shaders.vertex";
        file.GetContent(file.path, "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.VertexShader, shaders.ShaderData, "vertex shader");

        // Fragment Shader
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\3.Camera\\shaders.fragment";
        file.GetContent(file.path, "rb", shaders.ShaderData);
        glad.CompileAndAttachShader(shaders.ShaderProgram, shaders.FragmentShader, shaders.ShaderData, "fragment shader");

        // Link Shader Program
        glad.LinkeShaderPorgram(shaders.ShaderProgram);
        glad.UseShaderProgram(shaders.ShaderProgram);
        //-----------------------------------------------------------------------------

        int _color = glGetUniformLocation(shaders.ShaderProgram, "_color");
        float _Color[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glUniform4fv(_color, 1, _Color);
        printf("Get uniform of _Color : %d\n", _color);
        // 传入MVP矩阵
        int _identity = glGetUniformLocation(shaders.ShaderProgram, "_identity");
        printf("Get uniform of _model_identity : %d\n", _identity);
        glUniformMatrix4fv(_identity, 1, GL_FALSE, identity);

        mat4 model_scale;
        Transform::Scale(1.0f, 1.0f, 1.0f, model_scale._mat4);
        int _model_scale = glGetUniformLocation(shaders.ShaderProgram, "_model_scale");
        printf("Get uniform of _model_scale : %d\n", _model_scale);
        glUniformMatrix4fv(_model_scale, 1, GL_FALSE, model_scale._mat4);

        mat4 model_rotation;
        Transform::Rotation(0.0f, 0.0f, 0.0f, model_rotation._mat4);
        int _model_rotation = glGetUniformLocation(shaders.ShaderProgram, "_model_rotation");
        printf("Get uniform of _model_rotation : %d\n", _model_rotation);
        glUniformMatrix4fv(_model_rotation, 1, GL_TRUE, model_rotation._mat4);

        mat4 model_translate;
        Transform::Translate(0.0f, 0.0f, 0.0f, model_translate._mat4);
        int _model_translate = glGetUniformLocation(shaders.ShaderProgram, "_model_translate");
        printf("Get uniform of _model__translate : %d\n", _model_translate);
        glUniformMatrix4fv(_model_translate, 1, GL_TRUE, model_translate._mat4);

        //---------------------------------------------------------------------
        // Camera:
        // mat4 camera_scale;
        // Transform::Scale(1.0f, 1.0f, 1.0f, camera_scale._mat4);
        // int _camera_scale = glGetUniformLocation(shaders.ShaderProgram, "_camera_scale");
        // printf("Get uniform of _camera_scale : %d\n", _camera_scale);
        // glUniformMatrix4fv(_camera_scale, 1, GL_TRUE, camera_scale._mat4);
        // mat4 camera_rotation;
        // Transform::Rotation(0.0f, 0.0f, 0.0f, camera_rotation._mat4);
        // int _camera_rotation = glGetUniformLocation(shaders.ShaderProgram, "_camera_rotation");
        // printf("Get uniform of _camera_rotation : %d\n", _camera_rotation);
        // glUniformMatrix4fv(_camera_rotation, 1, GL_TRUE, camera_rotation._mat4);
        // mat4 camera_translate;
        // Transform::Translate(0.0f, 0.0f, 0.0f, camera_translate._mat4);
        // int _camera_translate = glGetUniformLocation(shaders.ShaderProgram, "_camera_translate");
        // printf("Get uniform of _camera_rotation : %d\n", _camera_translate);
        // glUniformMatrix4fv(_camera_translate, 1, GL_TRUE, camera_translate._mat4);
        // // Vec4 Eye(Tx, Ty, Tz, 1.0f);
        Camera camera;
        float cameraspacematrix[] = {
            (camera.Right).x, (camera.Up).x, (camera.Forward).x, 0.0f,
            (camera.Right).y, (camera.Up).y, (camera.Forward).y, 0.0f,
            (camera.Right).z, (camera.Up).z, (camera.Forward).z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
        mat4 camera_space_matrix(cameraspacematrix);
        int _camera_space_matrix = glGetUniformLocation(shaders.ShaderProgram, "_camera_space_matrix");
        printf("Get uniform of _camera_space_matrix : %d\n", _camera_space_matrix);
        glUniformMatrix4fv(_camera_space_matrix, 1, GL_TRUE, camera_space_matrix._mat4);
        //---------------------------------------------------------------------

        // Projection matrix:
        float w = 5.0f;
        float h = 5.0f;
        float n = 0.01f;
        float f = 30.0f;

        float projection[] = {
            2.0f / w, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / h, 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f / (f - n), (f + n) / (f - n),
            0.0f, 0.0f, 0.0f, 1.0f};

        mat4 P(projection);
        int _P = glGetUniformLocation(shaders.ShaderProgram, "_P");
        printf("Get uniform of _P : %d\n", _P);
        glUniformMatrix4fv(_P, 1, GL_TRUE, P._mat4);

        // for (int i = 0; i < 16; i++)
        // {
        //     printf("%f ", model_rotation._mat4[i]);
        //     if ((i + 1) % 4 == 0)
        //         printf("\n");
        // }

        // for (int i = 0; i < 4; i++)
        // {
        //     printf("%f ", Forward.v[i]);
        //     if ((i + 1) % 4 == 0)
        //         printf("\n");
        // }
        // for (int i = 0; i < 4; i++)
        // {
        //     printf("%f ", Up.v[i]);
        //     if ((i + 1) % 4 == 0)
        //         printf("\n");
        // }
        // for (int i = 0; i < 4; i++)
        // {
        //     printf("%f ", Right.v[i]);
        //     if ((i + 1) % 4 == 0)
        //         printf("\n");
        // }

        // Set Textures--------------------------------------------------------
        SetTextures(&attri,texAttri,shaders);


        //-----------------------------------------------------------------------------

        // 设置每帧重置属性
        glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
        glClearDepth(1.0f);
        glClearStencil(0.0f);
        glEnable(GL_DEPTH_TEST);

        // 记录帧速率:--------------------------------------------------------
        auto first_frame_time = std::chrono::high_resolution_clock::now();
        auto last_frame_time = std::chrono::high_resolution_clock::now();
        auto current_frame_time = std::chrono::high_resolution_clock::now();
        float deltTime = 0;
        float time = 0;
        //-------------------------------------------------------------------
        while (!glfwWindowShouldClose(glfw.window))
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
            glfwPollEvents();
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // Textures-----------------------------------
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, attri.TEX[0]);
            //--------------------------------------------

            // printf("Degbug\n");
            // Key Board Event-----------------------------
            speed = deltTime;
            KeyRotate(glfw.window, objects.scene_objects[0], speed * 20.0f, model_rotation._mat4, _model_rotation);
            KeyTranslate(glfw.window, objects.scene_objects[0], speed * 10.0f, model_translate._mat4, _model_translate);

            // Camera
            // CameraRotate(glfw.window, camera, speed * 20.0f, camera_rotation._mat4, _camera_rotation);
            // CameraTranslate(glfw.window, camera, speed * 10.0f, camera_translate._mat4, _camera_translate);
            // printf("%f : %f : %f\n",objects.scene_objects[0].rotationX,objects.scene_objects[0].rotationY,objects.scene_objects[0].rotationZ);
            // CameraRotate(glfw.window,)

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

//{{cos(b) cos(c), -cos(b) sin(c), sin(b), 0}, {sin(a) sin(b) cos(c) + cos(a) sin(c), sin(a) sin(b) sin(c) + cos(a) cos(c), -sin(a) cos(b), 0}, {-cos(a) sin(b) cos(c) + sin(a) sin(c), cos(a) sin(b) sin(c) + sin(a) cos(c), cos(b), 0}, {0, 0, 0, 1}}
