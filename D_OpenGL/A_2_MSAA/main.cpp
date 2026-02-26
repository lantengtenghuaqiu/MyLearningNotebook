#include <vector>
#include <chrono>

#include "../includes/Window.hpp"
#include "../includes/Transformation.hpp"
#include "../includes/Event.hpp"

#ifdef __WIN32__
#include "../includes/GlobalConfig.hpp"
#include "../includes/ModelLoader.hpp"
#endif
// g++ main.cpp ..\src\glad.c -I ..\includes -L ..\libs -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -pedantic -std=c++17 -o main.exe
// clang++ main.cpp ../src/glad.c  -I ../includes -L ../libs -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo  -o main

// 全局 uniform
// Container
int _model_scale;
int _model_rotation;
int _model_translate;
int _containerTransform;

float speed = 0.0f;

void GlobalFrameConfig()
{
    // glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void DrawCallFirst()
{

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_STENCIL_TEST);
    // glDepthFunc(GL_EQUAL);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

int main()
{
    GLFW glfw;
    GLAD<float> glad;

    if (glfw.InitGlfw(width, height, "OpenGLClass"))
    {

        glad.InitGlad(glfw.window, width, height, frameBufferWidth, frameBufferHeight);

        //---------------------------------------------------------------------
        // Global Config-------------------------------------------------------
        // File Mangger--------------------------------------------------------
        ReadFile::TheFile file;
        // --------------------------------------------------------------------

        // Camera--------------------------------------------------------------
        Camera camera;
        SetCamera(camera, (float)frameBufferWidth, (float)frameBufferHeight);

        // Projection matrix:
        const float Projection[][16] = {
            {
                //@
                2.0f / camera.w, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f / camera.h, 0.0f, 0.0f,
                0.0f, 0.0f, 2.0f / (camera.f - camera.n), 0.0f,
                0.0f, 0.0f, (camera.f + camera.n) / (camera.f - camera.n), 1.0f
                //@
            },

            {
                //@
                1.0f / (camera.aspect * tan(camera.fov / 2.0f)), 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f / tan(camera.fov / 2.0f), 0.0f, 0.0f,
                0.0f, 0.0f, (camera.n + camera.f) / (camera.n - camera.f), -1.0f,
                0.0f, 0.0f, (2.0f * camera.n * camera.f) / (camera.n - camera.f), 0.0f
                //@
            }

        };
        // --------------------------------------------------------------------
        // All Buffer Object---------------------------------------------------
        // Buffer Object Mannger
        Attributes attri(4, 4, 1, 4, 1, 2);
        CurrentID CID;
        // Buffer Generation
        glGenVertexArrays(attri.sizeVAO, attri.VAO);
        glGenBuffers(attri.sizeEBO, attri.EBO);
        glGenBuffers(attri.sizeVBO, attri.VBO);
        glGenTextures(attri.sizeTEX, attri.TEX);
        glGenBuffers(attri.sizeUBO, attri.UBO);
        // --------------------------------------------------------------------
        // Uniform buffer object config
        printf("UBO : %d\n", attri.UBO[CID.UBO]);
        glBindBuffer(GL_UNIFORM_BUFFER, attri.UBO[CID.UBO]);
        glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), Projection[1]);
        glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), camera.cameraspacematrix);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, attri.UBO[CID.UBO]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        CID.UBO++;

        // Vertex Array Mannager-----------------------------------------------
        Vertices *vertex = new Vertices[4];
        // --------------------------------------------------------------------

        // Texture Index-------------------------------------------------------
        Picture::Image image;
        //---------------------------------------------------------------------
        // --------------------------------------------------------------------
        // Draw Container box
        printf("\n****Draw container box mesh****\n");
        printf("CID | Vertex : %d , VBO : %d , VAO : %d\n", CID.Vertex, CID.VBO, CID.VAO);
        glad.BindVAO(attri.VAO[CID.VAO]);

        glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[CID.VBO]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::mesh) + sizeof(Cube::normal) + sizeof(Cube::uv), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Cube::mesh), &Cube::mesh);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cube::mesh), sizeof(Cube::normal), &Cube::normal);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cube::mesh) + sizeof(Cube::normal), sizeof(Cube::uv), &Cube::uv);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)sizeof(Cube::mesh));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(sizeof(Cube::mesh) + sizeof(Cube::normal)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        CID.VAO += 1;
        CID.VBO += 1;
        CID.Vertex += 1;

        Shader containerShader;
// Vertex Shader
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_2_MSAA/shaders.vertex";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_2_MSAA\\shaders.vertex";
#endif

        file.GetContent(file.path, "rb", containerShader.ShaderData);
        glad.CompileAndAttachShader(containerShader.Program, containerShader.Vertex, containerShader.ShaderData, "vertex shader");

// Fragment Shader
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_2_MSAA/shaders.fragment";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_2_MSAA\\shaders.fragment";
#endif
        file.GetContent(file.path, "rb", containerShader.ShaderData);
        glad.CompileAndAttachShader(containerShader.Program, containerShader.Fragment, containerShader.ShaderData, "fragment shader");

        // Link Shader Program
        glad.LinkShaderProgram(containerShader.Program);

        // Detch And Delete Shaders
        glad.DetachAndDeleteShaders(containerShader);
        //---------------------------------------------------------------------

        // Set Textures--------------------------------------------------------
        printf("\n****Set Textures****\n");
        TexAttri *texAttri = new TexAttri[4];

        // Set the box textures
        // SetTextures(&attri, texAttri, containerShader);

        //---------------------------------------------------------------------

        Hierarchy hierarchy;
        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        SceneObject *cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);
        {
            std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components.emplace("Color", ColorComponent());
            std::get<ColorComponent>(std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components["Color"]) = Vec4(0.6f, 0.5f, 0.31f, 1.0);
        }

        //---------------------------------------------------------------------

        // 设置每帧重置属性
        ClearGLAttribute();
        //---------------------------------------------------------------------
        auto first_frame_time = std::chrono::high_resolution_clock::now();
        auto last_frame_time = std::chrono::high_resolution_clock::now();
        auto current_frame_time = std::chrono::high_resolution_clock::now();
        float deltTime = 0;
        float time = 0;
        // Main While-----------------------------------------------------------
        while (!glfwWindowShouldClose(glfw.window))
        {
            // 帧速率:
            current_frame_time = std::chrono::high_resolution_clock::now();
            deltTime = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - last_frame_time).count();
            time = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - first_frame_time).count();
            last_frame_time = std::chrono::high_resolution_clock::now();
            speed = deltTime;

#ifdef LOG_DELTTIME
            printf("delt time: %f\n", deltTime);
            printf("time: %f\n", time);
#endif
            glfwPollEvents();
            // Driver 1 (CPU -> GPU)-------------------------------------------
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            //-----------------------------------------------------------------
            // Camera Changes--------------------------------------------------
            //-----------------------------------------------------------------
            // Draw Call-------------------------------------------------------
            // Draw Call 1-----------------------------------------------------

            // Global Draw Call Configging
            // GlobalFrameConfig();

            // DrawCallFirst();
            glUseProgram(containerShader.Program);
            // Textures

            KeyRotate(glfw.window, *cube, speed * 30.0f, cube->Rotation._mat4, _model_rotation);
            KeyTranslate(glfw.window, *cube, speed * 10.0f, cube->Translate._mat4, _model_translate);

            if (containerShader.init == false)
            {

                // 传入MVP矩阵
                Transform::Scale(1.0f, 1.0f, 1.0f, cube->Scale._mat4);
                _model_scale = glGetUniformLocation(containerShader.Program, "_model_scale");
                glUniformMatrix4fv(_model_scale, 1, GL_FALSE, cube->Scale._mat4);

                Transform::Rotation(0.0f, 0.0f, 0.0f, cube->Rotation._mat4);
                _model_rotation = glGetUniformLocation(containerShader.Program, "_model_rotation");
                glUniformMatrix4fv(_model_rotation, 1, GL_FALSE, cube->Rotation._mat4);

                Transform::Translate(0.0f, 0.0f, 0.0f, cube->Translate._mat4);
                _model_translate = glGetUniformLocation(containerShader.Program, "_model_translate");
                glUniformMatrix4fv(_model_translate, 1, GL_FALSE, cube->Translate._mat4);

                _containerTransform = glGetUniformBlockIndex(containerShader.Program, "Transform");
                glUniformBlockBinding(containerShader.Program,_containerTransform,0);

                containerShader.init = true;
            }

            // Key Board Event-----------------------------
            glBindVertexArray(attri.VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            //-----------------------------------------------------------------

            // printf("Degbug\n");
            glfwSwapBuffers(glfw.window);
        }
        delete[] (texAttri);
        texAttri = NULL;
        glfwTerminate();
    }
    else
    {
        printf("Init the glfw failed");
    }

    return 0;
}
