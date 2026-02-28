#include <vector>
#include <chrono>

#include "../includes/Window.hpp"
#include "../includes/Transformation.hpp"
#include "../includes/Event.hpp"

#ifdef __WIN32__
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
    glEnable(GL_DEPTH_TEST);
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

void Container(GLAD<float> &glad, ReadFile::TheFile &file, ObjectID *OID, Shader &containerShader)
{
    printf("VAO Index : %d \n", OID->VAO[OID->CID_VAO]);
    glBindVertexArray(OID->VAO[OID->CID_VAO]);

    glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->CID_VBO]);

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

    OID->CID_VAO += 1;
    OID->CID_VBO += 1;
    OID->CID_Vertex += 1;

    // Vertex Shader
#ifdef __APPLE__
    file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_2_MSAA/shaders.vertex";
#else
    file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_2_MSAA\\shaders.vertex";
#endif

    file.GetContent(file.path, "rb", containerShader.ShaderData);
    glad.CompileAndAttachShader(containerShader.Program, containerShader.Vertex, containerShader.ShaderData, "vertex shader");

// // Fragment Shader
// #ifdef __APPLE__
//     file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_2_MSAA/shaders.fragment";
// #else
//     file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_2_MSAA\\shaders.fragment";
// #endif
//     file.GetContent(file.path, "rb", containerShader.ShaderData);
//     glad.CompileAndAttachShader(containerShader.Program, containerShader.Fragment, containerShader.ShaderData, "fragment shader");

//     // Link Shader Program
//     glad.LinkShaderProgram(containerShader.Program);

//     // Detch And Delete Shaders
//     glad.DetachAndDeleteShaders(containerShader);
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
        ObjectID *OID = ObjectIndex::GetIntance();
        OID->CreateAndGenObjectIndex(OID_VAO, 1);
        glGenVertexArrays(OID->sizeVAO, OID->VAO);

        OID->CreateAndGenObjectIndex(OID_VBO, 1);
        glGenBuffers(OID->sizeVBO, OID->VBO);

        OID->CreateAndGenObjectIndex(OID_EBO, 1);
        glGenBuffers(OID->sizeEBO, OID->EBO);

        OID->CreateAndGenObjectIndex(OID_TEX, 1);
        glGenTextures(OID->sizeTEX, OID->TEX);

        OID->CreateAndGenObjectIndex(OID_UBO, 1);
        glGenBuffers(OID->sizeUBO, OID->UBO);

        // OID->CreateAndGenObjectIndex(OID_FBO,1);

        // ObjectCurrentID OID;
        // Vertex Array Mannager-----------------------------------------------
        Vertices *vertex = new Vertices[4];
        // Texture Index-------------------------------------------------------
        printf("\n****Set Textures****\n");
        // Picture::ImageManager *image = Picture::ImageManager::GetInstance();
        // TexAttri *texAttri = new TexAttri[4];

        // Buffer Generation
        // --------------------------------------------------------------------
        // Uniform buffer object config
        printf("UBO : %d\n", OID->UBO[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[OID->CID_UBO]);
        glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), Projection[1]);
        glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), camera.cameraspacematrix);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, OID->UBO[OID->CID_UBO]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        OID->CID_UBO++;

        //---------------------------------------------------------------------
        // Off-Screen MSAA
        unsigned int OID_Texture_MSAA;
        glGenTextures(1, &OID_Texture_MSAA);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, OID_Texture_MSAA);
        int sampleTimes;
        glGetIntegerv(GL_MAX_SAMPLES, &sampleTimes);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sampleTimes, GL_RGB, frameBufferWidth, frameBufferWidth, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        unsigned int OID_FrameBuffer_MSAA;
        glGenFramebuffers(1, &OID_FrameBuffer_MSAA);
        glBindFramebuffer(GL_FRAMEBUFFER, OID_FrameBuffer_MSAA);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, OID_Texture_MSAA, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        unsigned int OID_RenderBuffer_MSAA;
        glGenRenderbuffers(1, &OID_RenderBuffer_MSAA);
        glBindRenderbuffer(GL_RENDERBUFFER, OID_RenderBuffer_MSAA);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, frameBufferWidth, frameBufferHeight);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        unsigned int OID_Texture_ScreenTexture;
        glGenTextures(1, &OID_Texture_ScreenTexture);
        glBindTexture(GL_TEXTURE_2D, OID_Texture_ScreenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned int OID_FrameBuffer_ScreenTexture;
        glGenFramebuffers(1, &OID_FrameBuffer_ScreenTexture);
        glBindFramebuffer(GL_FRAMEBUFFER, OID_FrameBuffer_ScreenTexture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, OID_Texture_ScreenTexture, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //---------------------------------------------------------------------
        // Hierarchy Scene Object
        Hierarchy hierarchy;
        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        hierarchy.sceneObjects.emplace("ScreenTexture", SceneObject());

        //---------------------------------------------------------------------
        // Draw Container box------------------------------
        printf("\n**** Draw container box mesh ****\n");
        SceneObject *cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);
        {
            std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components.emplace("Color", ColorComponent());
            std::get<ColorComponent>(std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components["Color"]) = Vec4(0.6f, 0.5f, 0.31f, 1.0);
        }

        printf("OID | Vertex : %d , VBO : %d , VAO : %d\n", OID->CID_Vertex, OID->CID_VBO, OID->CID_VAO);
        Shader Shader_containerShader;
        Container(glad, file, OID, Shader_containerShader);
        // SetTextures(&attri, texAttri, containerShader);
        // Draw Plane Mesh---------------------------------
        // printf("\n **** Draw MSAA Screen Textuer Mesh ****");
        // SceneObject *ScreenTexture = &std::get<SceneObject>(hierarchy.sceneObjects["ScreenTexture"]);
        // {
        // }
        // printf("OID |  Vertex : %d , VBO : %d , VAO : %d\n", OID->CID_Vertex, OID->CID_VBO, OID->CID_VAO);
        // glBindVertexArray(OID->VAO[OID->currentId.VAO]);
        // glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->currentId.VBO]);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(Plane::mesh) + sizeof(Plane::uv), NULL, GL_STATIC_DRAW);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Plane::mesh), &Plane::mesh);
        // glBufferSubData(GL_ARRAY_BUFFER, sizeof(Plane::mesh), sizeof(Plane::uv), &Plane::uv);

        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)sizeof(Plane::mesh));
        // glEnableVertexAttribArray(1);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);

        // OID->CID_Vertex++;
        // OID->CID_VBO++;
        // OID->CID_VAO++;

//         Shader Shader_screenTexture;

//         // Vertex Shader
// #ifdef __APPLE__
//         file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_2_MSAA/shaders.vertex";
// #else
//         file.path = "G:\\user\\desktop\\C++\\GraphicLearning/D_OpenGL/A_2_MSAA/shaders.vertex";
// #endif

//         file.GetContent(file.path, "rb", Shader_screenTexture.ShaderData);
//         glad.CompileAndAttachShader(Shader_screenTexture.Program, Shader_screenTexture.Vertex, Shader_screenTexture.ShaderData, "vertex shader");

        // glad.GetShadersData(file, "/D_OpenGL/A_2_MSAA/shaders.vertex", Shader_screenTexture);
        // glad.CompileAndAttachShader(Shader_screenTexture.Program, Shader_screenTexture.Vertex, Shader_screenTexture.ShaderData, "Vertex Shader");

        // glad.GetShadersData(file, "/D_OpenGL/A_2_MSAA/screenTexture.fragment", Shader_screenTexture);
        // glad.CompileAndAttachShader(Shader_screenTexture.Program, Shader_screenTexture.Fragment, Shader_screenTexture.ShaderData, "Fragment Shader");

        // glad.LinkShaderProgram(Shader_screenTexture.Program);
        // glad.DetachAndDeleteShaders(Shader_screenTexture);

        // unsigned int OID_Location_screenTexture;
        // OID_Location_screenTexture = glGetUniformLocation(Shader_screenTexture.Program, "screenTexture");
        // printf("OID_Location_screenTexture : %d\n", OID_Location_screenTexture);
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
            // Global Draw Call Configging-----------------
            // glBindFramebuffer(GL_FRAMEBUFFER, OID_FrameBuffer_MSAA);
            GlobalFrameConfig();
            // Clearning
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            //-----------------------------------------------------------------
            // Camera Changes--------------------------------------------------

            //-----------------------------------------------------------------
            // Draw Calls-------------------------------------------------------
            // Draw Call 1-----------------------------------------------------

            // DrawCallFirst();
            glUseProgram(Shader_containerShader.Program);
            // // Textures------------------------------------

            // // Key Board Event-----------------------------
            KeyRotate(glfw.window, *cube, speed * 30.0f, cube->Rotation._mat4, _model_rotation);
            KeyTranslate(glfw.window, *cube, speed * 10.0f, cube->Translate._mat4, _model_translate);

            if (Shader_containerShader.init == false)
            {

                // 传入MVP矩阵
                Transform::Scale(1.0f, 1.0f, 1.0f, cube->Scale._mat4);
                _model_scale = glGetUniformLocation(Shader_containerShader.Program, "_model_scale");
                glUniformMatrix4fv(_model_scale, 1, GL_FALSE, cube->Scale._mat4);

                Transform::Rotation(0.0f, 0.0f, 0.0f, cube->Rotation._mat4);
                _model_rotation = glGetUniformLocation(Shader_containerShader.Program, "_model_rotation");
                glUniformMatrix4fv(_model_rotation, 1, GL_FALSE, cube->Rotation._mat4);

                Transform::Translate(0.0f, 0.0f, 0.0f, cube->Translate._mat4);
                _model_translate = glGetUniformLocation(Shader_containerShader.Program, "_model_translate");
                glUniformMatrix4fv(_model_translate, 1, GL_FALSE, cube->Translate._mat4);

                _containerTransform = glGetUniformBlockIndex(Shader_containerShader.Program, "Transform");
                glUniformBlockBinding(Shader_containerShader.Program, _containerTransform, 0);

                Shader_containerShader.init = true;
            }

            glBindVertexArray(OID->VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            //-----------------------------------------------------------------

            // glBindFramebuffer(GL_READ_FRAMEBUFFER, OID_FrameBuffer_MSAA);
            // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OID_FrameBuffer_ScreenTexture);
            // glBlitFramebuffer(0, 0, frameBufferWidth, frameBufferHeight, 0, 0, frameBufferWidth, frameBufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
            // glClear(GL_COLOR_BUFFER_BIT);
            // glDisable(GL_DEPTH_TEST);

            // glUseProgram(Shader_screenTexture.Program);
            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, OID_Texture_ScreenTexture);
            // // glUniform1i(OID_Location_screenTexture,0);

            // if (Shader_screenTexture.init == false)
            // {

            //     Shader_screenTexture.init = true;
            // }

            // glBindVertexArray(OID->VAO[1]);
            // glDrawArrays(GL_TRIANGLES, 0, 6);
            // glBindVertexArray(0);
            // printf("Degbug\n");
            glfwSwapBuffers(glfw.window);
        }
        // delete[] (texAttri);
        // texAttri = NULL;
        glfwTerminate();
    }
    else
    {
        printf("Init the glfw failed");
    }

    return 0;
}
