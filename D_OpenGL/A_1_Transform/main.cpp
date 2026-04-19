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

// 全局数据------------------------------------------------

Vec4 AmbientColor = Vec4(0.76f, 0.76f, 0.80f, 1.0f);

// 全局 uniform
// Cube map
int _cubeMapProjection;
int _cubeMapView;

// Container
// int _camera_space_matrix;
int ViewPos;
int _P;
int _model_scale;
int _model_rotation;
int _model_translate;
int _containerTransform;

// Light Box
int PointLightLightPos;
int PointLightLightColor;
int PointLightLinear;
int PointLightConstant;
int PointLightQuadratic;
int BaseColor;

int _view_space_matrix2;
int _P2;
int _scale2;
int _rotation2;
int _translate2;

// Environment
int Ambient;

// Directional Light
int DirectionalLightDir;
int DirectionalLightCol;
int DirectionalLightInten;

// render texture:
int _transformRT;
int _RT_ModelTranslate;
int _RT_ModelScale;

// ----------------------------------------------------------------------------
char *faces[] =
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"};
//---
float speed = 0.0f;

void GlobalFrameConfig()
{
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

void DrawCallSecond()
{
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_STENCIL_TEST);

    glDisable(GL_BLEND);
}

int main()
{
    GLFW glfw;
    GLAD *glad;

    if (glfw.InitGlfw(width, height, "OpenGLClass"))
    {

        glad->InitGlad(glfw.window, width, height, frameBufferWidth, frameBufferHeight);

        //---------------------------------------------------------------------
        // Global Config-------------------------------------------------------
        // File Mangger--------------------------------------------------------
        Tools::TheFileManager *file = Tools::TheFileManager::GetInstance();
        // --------------------------------------------------------------------

        // Camera--------------------------------------------------------------
        Camera *camera;
        SetMainCamera(camera, (float)frameBufferWidth, (float)frameBufferHeight);

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

        // Used in Binding VAO & VBO & Shaders---------------------------------
        ObjectCurrentID OID;
        // --------------------------------------------------------------------

        // All Buffer Object---------------------------------------------------
        // Buffer Object Mannger
        ObjectID attri(4, 4, 1, 4, 1, 2);
        // Buffer Generation
        glGenVertexArrays(attri.sizeVAO, attri.VAO);
        glGenBuffers(attri.sizeEBO, attri.EBO);
        glGenBuffers(attri.sizeVBO, attri.VBO);
        glGenTextures(attri.sizeTEX, attri.TEX);
        glGenBuffers(attri.sizeUBO, attri.UBO);
        // --------------------------------------------------------------------
        // Uniform buffer object config
        printf("UBO : %d\n", attri.UBO[OID.UBO]);
        glBindBuffer(GL_UNIFORM_BUFFER, attri.UBO[OID.UBO]);
        glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), Projection[1]);
        glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), camera.CameraSpace);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, attri.UBO[OID.UBO]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        OID.UBO++;

        // Vertex Array Mannager-----------------------------------------------
        Vertices *vertex = new Vertices[4];
        // --------------------------------------------------------------------

        // Texture Index-------------------------------------------------------
        Tools::ImageManager *image = Tools::ImageManager::GetInstance();
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------

        // Draw Cube Map-------------------------------------------------------
        printf("\n****Draw Cube map box****\n");
        printf("CID | Vertex : %d , VBO : %d , VAO : %d\n", OID.Vertex, OID.VBO, OID.VAO);
        glad->BindVAO(attri.VAO[OID.VAO]);
        glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[OID.VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::mesh) + sizeof(Cube::uv), NULL, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Cube::mesh), &Cube::mesh);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cube::mesh), sizeof(Cube::normal), &Cube::normal);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)sizeof(Cube::mesh));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        OID.VAO += 1;
        OID.VBO += 1;
        OID.Vertex += 1;

        Shader cubeMapShader;

        // vertex
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Materials/CubeMap.vertex";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Materials\\CubeMap.vertex";
#endif
        file.GetContent(file.path, "rb", cubeMapShader.ShaderData);
        glad.CompileAndAttachShader(cubeMapShader.Program, cubeMapShader.Vertex, cubeMapShader.ShaderData, "Cube map vertex shader");

        // fragment
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Materials/CubeMap.fragment";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Materials\\CubeMap.fragment";
#endif
        file.GetContent(file.path, "rb", cubeMapShader.ShaderData);
        glad.CompileAndAttachShader(cubeMapShader.Program, cubeMapShader.Fragment, cubeMapShader.ShaderData, "Cube map fragment shader");

        glad.LinkShaderProgram(cubeMapShader.Program);

        // Detch And Delete Shaders
        glad.DetachAndDeleteShaders(cubeMapShader);

        // Cube Map Texture----------------------------------------------------
        printf("\n****Cube Map****\n");
        unsigned int _cubeMapLocation = glGetUniformLocation(cubeMapShader.Program, "skybox");
        unsigned int cubeMapTexId;
        glGenTextures(1, &cubeMapTexId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexId);
#ifdef __APPLE__
        image.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/Assets/Textures/skybox";
#else
        image.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\skybox";
#endif
        size_t path1 = strlen(image.path);
        for (int i = 0; i < 6; i++)
        {
            // char *path = image.path + faces[i];
            size_t path2 = strlen(faces[i]);
            size_t whole_path_size = path1 + path2 + 2;
            char path[whole_path_size];
            for (size_t j = 0; j < whole_path_size; j++)
            {
                if (j < path1)
                    path[j] = image.path[j];
                else if (j == path1)
#ifdef __APPLE__
                    path[j] = '/';
#else
                    path[j] = '\\';
#endif
                else
                    path[j] = faces[i][(j - 1) - path1];
                printf("%c", path[j]);
            }
            printf("\n");
            path[whole_path_size] = '\0';

            image.data = stbi_load(path, &image.width, &image.height, &image.channels, 0);
            if (image.data != NULL)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
                stbi_image_free(image.data);
            }
            else
            {
                printf("Wrong image data");
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        // --------------------------------------------------------------------
        // Draw Container box
        printf("\n****Draw container box mesh****\n");
        printf("CID | Vertex : %d , VBO : %d , VAO : %d\n", OID.Vertex, OID.VBO, OID.VAO);
        glad.BindVAO(attri.VAO[OID.VAO]);

        glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[OID.VBO]);

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

        OID.VAO += 1;
        OID.VBO += 1;
        OID.Vertex += 1;

        Shader containerShader;
// Vertex Shader
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/shaders.vertex";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\shaders.vertex";
#endif

        file.GetContent(file.path, "rb", containerShader.ShaderData);
        glad.CompileAndAttachShader(containerShader.Program, containerShader.Vertex, containerShader.ShaderData, "vertex shader");

// Fragment Shader
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/shaders.fragment";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\shaders.fragment";
#endif
        file.GetContent(file.path, "rb", containerShader.ShaderData);
        glad.CompileAndAttachShader(containerShader.Program, containerShader.Fragment, containerShader.ShaderData, "fragment shader");

        // Link Shader Program
        glad.LinkShaderProgram(containerShader.Program);

        // Detch And Delete Shaders
        glad.DetachAndDeleteShaders(containerShader);

        //---------------------------------------------------------------------
        // Draw light box
        printf("\n****Draw light box mesh****\n");
        printf("CID | Vertex : %d , VBO : %d , VAO : %d\n", OID.Vertex, OID.VBO, OID.VAO);
        glad.BindVAO(attri.VAO[OID.VAO]);
        ReadVertexData(vertex[OID.Vertex], Cube::mesh, sizeof(Cube::mesh));
        BindVBO(attri, OID.VBO, vertex[OID.Vertex]);
        OID.VAO += 1;
        OID.VBO += 1;
        OID.Vertex += 1;

        Shader lightBoxShader;
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/Shaders/shaders.vertex";

#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\Shaders\\shaders.vertex";
#endif
        file.GetContent(file.path, "rb", lightBoxShader.ShaderData);
        glad.CompileAndAttachShader(lightBoxShader.Program, lightBoxShader.Vertex, lightBoxShader.ShaderData, "Vertex Shader");

#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/Shaders/shaders.fragment";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\Shaders\\shaders.fragment";
#endif
        file.GetContent(file.path, "rb", lightBoxShader.ShaderData);
        glad.CompileAndAttachShader(lightBoxShader.Program, lightBoxShader.Fragment, lightBoxShader.ShaderData, "Fragment Shader");

        // Link Shader Program
        glad.LinkShaderProgram(lightBoxShader.Program);

        glad.DetachAndDeleteShaders(lightBoxShader);
        //---------------------------------------------------------------------
        // Draw Render View
        printf("\n****Draw Render View Window****\n");
        // instance datas---
        float *translate = new float[100];
        int index = 0;
        float offset = 1.0f;
        for (int y = -10; y < 10; y += 2)
        {
            for (int x = -10; x < 10; x += 2)
            {

                float xoffset = (float)x / 2.0 + offset;
                float yoffset = (float)y / 2.0 + offset;
                if (index % 2 == 0)
                    translate[index] = xoffset;
                else
                    translate[index] = yoffset;
                printf("index : %d | size of translate data : %f \n", index, translate[index]);
                index++;
            }
        }

        unsigned int instanceID;
        glGenBuffers(1, &instanceID);
        glBindBuffer(GL_ARRAY_BUFFER, instanceID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 100, translate, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind logic---
        printf("CID | Vertex : %d , VBO : %d , VAO : %d\n", OID.Vertex, OID.VBO, OID.VAO);
        glad.BindVAO(attri.VAO[OID.VAO]);
        printf("Bind VBO : %d\n", attri.VBO[OID.VBO]);
        glBindBuffer(GL_ARRAY_BUFFER, attri.VBO[OID.VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Plane::mesh) + sizeof(Plane::uv), NULL, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Plane::mesh), &Plane::mesh);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Plane::mesh), sizeof(Plane::uv), &Plane::uv);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)(sizeof(Plane::mesh)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        OID.VAO += 1;
        OID.VBO += 1;
        OID.Vertex += 1;

        glBindBuffer(GL_ARRAY_BUFFER, instanceID);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Shader renderTextureShader;
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/Shaders/RenderTextureShader.vertex";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\Shaders\\RenderTextureShader.vertex";
#endif
        file.GetContent(file.path, "rb", renderTextureShader.ShaderData);
        glad.CompileAndAttachShader(renderTextureShader.Program, renderTextureShader.Vertex, renderTextureShader.ShaderData, "vertex shader");
#ifdef __APPLE__
        file.path = "/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/A_1_Transform/Shaders/RenderTextureShader.fragment";
#else
        file.path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\A_1_Transform\\Shaders\\RenderTextureShader.fragment";
#endif
        file.GetContent(file.path, "rb", renderTextureShader.ShaderData);
        glad.CompileAndAttachShader(renderTextureShader.Program, renderTextureShader.Fragment, renderTextureShader.ShaderData, "fragment shader");

        glad.LinkShaderProgram(renderTextureShader.Program);

        glad.DetachAndDeleteShaders(renderTextureShader);

        // Set Textures--------------------------------------------------------
        printf("\n****Set Textures****\n");
        TexAttri *texAttri = new TexAttri[4];

        // Set the box textures
        SetTextures(&attri, texAttri, containerShader);
        // Set the logo texture
        SetBindLogoMesh(&attri, texAttri, renderTextureShader);

        //---------------------------------------------------------------------
        // Frame buffer--------------------------------------------------------
        printf("\n****Frame buffer****\n");

        unsigned int frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        printf("FBO Index : %d\n", frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        unsigned int colorBuffertTextureId;
        glGenTextures(1, &colorBuffertTextureId);
        glBindTexture(GL_TEXTURE_2D, colorBuffertTextureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffertTextureId, 0);

        unsigned int depthRenderBufferId;
        glGenRenderbuffers(1, &depthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Good , completed create the frame buffer : %d\n", frameBuffer);
            // glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
            printf("Is NOT a completed creation to the frame buffer : %d\n", frameBuffer);
        //---------------------------------------------------------------------

        // Hierarchy-----------------------------------------------------------
        // SceneObject指针数组
        /*
        //     size_t hierarchyCount = hierarchy.sceneObjects.size();
        //     SceneObject **sceneObjectPointer = new SceneObject *[hierarchyCount];
        //     printf("Scene Objects Count In Hierarchy : %ld\n", hierarchyCount);
        //     sceneObjectPointer[0] = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);
        // */

        //---------------------------------------------------------------------
        Hierarchy hierarchy;

        hierarchy.sceneObjects.emplace("Camera", Camera());
        Camera *CameraPointer = &std::get<Camera>(hierarchy.sceneObjects["Camera"]);
        {
            // CameraPointer->Position
        }

        hierarchy.sceneObjects.emplace("PointLight", Light());
        Light *PointLightPointer = &std::get<Light>(hierarchy.sceneObjects["PointLight"]);
        {
            PointLightPointer->Color = Vec4(1.0f, 1.0f, 1.0f, 0.0f);
            PointLightPointer->position = Vec4(0.0f, 1.0f, 5.0f, 1.0f);
            PointLightPointer->Intensity = 1.0f;
        }

        hierarchy.sceneObjects.emplace("DirectionLight", Light());
        Light *DirectionLightPotinter = &std::get<Light>(hierarchy.sceneObjects["DirectionLight"]);
        {
            DirectionLightPotinter->Direction.SetV(Normalize(Vec4(0.3f, 0.5f, 0.1f, 0.0f)));
            DirectionLightPotinter->Color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
            DirectionLightPotinter->Intensity = 1.0f;
        }

        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        SceneObject *cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);
        {
            std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components.emplace("Color", ColorComponent());
            std::get<ColorComponent>(std::get<SceneObject>(hierarchy.sceneObjects["Cube"]).components["Color"]) = Vec4(0.6f, 0.5f, 0.31f, 1.0);
        }

        hierarchy.sceneObjects.emplace("RTMesh", SceneObject());
        SceneObject *RTMesh = &std::get<SceneObject>(hierarchy.sceneObjects["RTMesh"]);
        {
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
#ifndef __APPLE__
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#endif
            //--------------------------------------------
            // CameraRotate(glfw.window, camera,speed *30.0f);
            // camera.viewMatrix = camera.ViewMatrix();
            //--------------------------------------------
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glEnable(GL_MULTISAMPLE);
            // Draw Call-------------------------------------------------------
            // Draw Call 1-----------------------------------------------------

            glDepthMask(GL_FALSE);
            glUseProgram(cubeMapShader.Program);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexId);
            glUniform1i(_cubeMapLocation, 0);

            if (cubeMapShader.init == false)
            {
                _cubeMapView = glGetUniformLocation(cubeMapShader.Program, "V");
                _cubeMapProjection = glGetUniformLocation(cubeMapShader.Program, "P");

                glUniformMatrix4fv(_cubeMapView, 1, GL_FALSE, camera.UpdateCameraSpaceMatrix());
                glUniformMatrix4fv(_cubeMapProjection, 1, GL_FALSE, Projection[1]);

                cubeMapShader.init = true;
            }

            glBindVertexArray(attri.VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);

            // Global Draw Call Configging
            GlobalFrameConfig();

            // Draw Call 2-----------------------------------------------------
            DrawCallFirst();
            glUseProgram(containerShader.Program);
            // Textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, attri.TEX[0]);
            glUniform1i(texAttri[0].textureLocation, 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, attri.TEX[1]);
            glUniform1i(texAttri[1].textureLocation, 1);

            KeyRotate(glfw.window, *cube, speed * 30.0f, cube->Rotation._mat4, _model_rotation);
            KeyTranslate(glfw.window, *cube, speed * 10.0f, cube->Translate._mat4, _model_translate);

            if (containerShader.init == false)
            {

                // 传入MVP矩阵
                Transform::Scale(1.0f, 1.0f, 1.0f, cube->Scale._mat4);
                Transform::Rotation(0.0f, 0.0f, 0.0f, cube->Rotation._mat4);
                Transform::Translate(0.0f, 0.0f, 0.0f, cube->Translate._mat4);

                _model_scale = glGetUniformLocation(containerShader.Program, "_model_scale");
                _model_rotation = glGetUniformLocation(containerShader.Program, "_model_rotation");
                _model_translate = glGetUniformLocation(containerShader.Program, "_model_translate");

                // _camera_space_matrix = glGetUniformLocation(containerShader.Program, "V");

                _containerTransform = glGetUniformBlockIndex(containerShader.Program, "Transform");
                // _P = glGetUniformLocation(containerShader.Program, "P");
                PointLightLightPos = glGetUniformLocation(containerShader.Program, "_LightPosition");
                PointLightLightColor = glGetUniformLocation(containerShader.Program, "_LightColor");
                BaseColor = glGetUniformLocation(containerShader.Program, "_BaseColor");
                Ambient = glGetUniformLocation(containerShader.Program, "_AmbientColor");

                ViewPos = glGetUniformLocation(containerShader.Program, "_ViewPos");

                DirectionalLightDir = glGetUniformLocation(containerShader.Program, "_direct_direction");
                // printf("%d\n",DirectionalLightDir);
                DirectionalLightCol = glGetUniformLocation(containerShader.Program, "_direct_color");
                DirectionalLightInten = glGetUniformLocation(containerShader.Program, "_direct_intensity");

                PointLightLinear = glGetUniformLocation(containerShader.Program, "_point_linear");
                PointLightConstant = glGetUniformLocation(containerShader.Program, "_point_constant");
                PointLightQuadratic = glGetUniformLocation(containerShader.Program, "_point_quadatic");

                glUniformMatrix4fv(_model_scale, 1, GL_FALSE, cube->Scale._mat4);
                glUniformMatrix4fv(_model_rotation, 1, GL_FALSE, cube->Rotation._mat4);
                glUniformMatrix4fv(_model_translate, 1, GL_FALSE, cube->Translate._mat4);

                // glUniformMatrix4fv(_camera_space_matrix, 1, GL_FALSE, camera.ViewMatrix());
                // glUniformMatrix4fv(_P, 1, GL_FALSE, Projection[1]);
                glUniformBlockBinding(containerShader.Program, _containerTransform, 0);

                glUniform3fv(PointLightLightPos, 1, PointLightPointer->position.v3);
                glUniform4fv(PointLightLightColor, 1, (PointLightPointer->Color * PointLightPointer->Intensity).v4);
                glUniform4fv(BaseColor, 1, std::get<ColorComponent>(cube->components["Color"]).rgba);
                glUniform4fv(Ambient, 1, AmbientColor.v4);
                glUniform4fv(ViewPos, 1, camera.Position.v);

                glUniform3fv(DirectionalLightDir, 1, DirectionLightPotinter->Direction.v3);
                glUniform4fv(DirectionalLightCol, 1, DirectionLightPotinter->Color.v4);
                glUniform1f(DirectionalLightInten, DirectionLightPotinter->Intensity);

                glUniform1f(PointLightConstant, 0.8f);
                glUniform1f(PointLightLinear, 0.009f);
                glUniform1f(PointLightQuadratic, 0.002f);

                containerShader.init = true;
            }

            // Key Board Event-----------------------------
            glBindVertexArray(attri.VAO[1]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            //-----------------------------------------------------------------
            // Draw Call 2-----------------------------------------------------
            DrawCallSecond();
            glUseProgram(lightBoxShader.Program);

            if (lightBoxShader.init == false)
            {
                Transform::Scale(0.2f, 0.2f, 0.2f, PointLightPointer->Scale._mat4);
                Transform::Rotation(0.0f, 0.0f, 0.0f, PointLightPointer->Rotation._mat4);
                Transform::Translate(PointLightPointer->position.x, PointLightPointer->position.y, PointLightPointer->position.z, PointLightPointer->Translate._mat4);

                _scale2 = glGetUniformLocation(lightBoxShader.Program, "_scale2");
                _rotation2 = glGetUniformLocation(lightBoxShader.Program, "_rotation2");
                _translate2 = glGetUniformLocation(lightBoxShader.Program, "_translate2");
                _view_space_matrix2 = glGetUniformLocation(lightBoxShader.Program, "V");
                _P2 = glGetUniformLocation(lightBoxShader.Program, "P");

                // 传入MVP矩阵
                glUniformMatrix4fv(_scale2, 1, GL_FALSE, PointLightPointer->Scale._mat4);

                glUniformMatrix4fv(_rotation2, 1, GL_FALSE, PointLightPointer->Rotation._mat4);

                glUniformMatrix4fv(_translate2, 1, GL_FALSE, PointLightPointer->Translate._mat4);

                glUniformMatrix4fv(_view_space_matrix2, 1, GL_FALSE, camera.UpdateCameraSpaceMatrix());

                glUniformMatrix4fv(_P2, 1, GL_FALSE, Projection[1]);

                lightBoxShader.init = true;
            }

            glBindVertexArray(attri.VAO[2]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            // //-----------------------------------------------------------------
            // Draw Call 3-----------------------------------------------------
            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
#ifndef __APPLE__
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
            glBlitFramebuffer(0, 0, frameBufferWidth, frameBufferHeight, 0, 0, frameBufferWidth, frameBufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif
            glUseProgram(renderTextureShader.Program);
            glBindVertexArray(attri.VAO[3]);

            glActiveTexture(GL_TEXTURE0);
#ifdef __APPLE__
            glBindTexture(GL_TEXTURE_2D, attri.TEX[2]);
#else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, colorBuffertTextureId);
#endif
            glUniform1i(texAttri[2].textureLocation, 0);
            if (renderTextureShader.init == false)
            {
                _transformRT = glGetUniformBlockIndex(renderTextureShader.Program, "Transform");
                if (_transformRT == GL_INVALID_INDEX)
                    printf("Wrong!!!!!!, _transformRT is error index\n");
                else
                    printf("Right, _transformRT index : %d\n", _transformRT);

                glUniformBlockBinding(renderTextureShader.Program, _transformRT, 0);

                _RT_ModelTranslate = glGetUniformLocation(renderTextureShader.Program, "translate");
                Transform::Translate(10.8f, 7.2f, 10.0f, RTMesh->Translate._mat4);
                glUniformMatrix4fv(_RT_ModelTranslate, 1, GL_FALSE, RTMesh->Translate._mat4);

                _RT_ModelScale = glGetUniformLocation(renderTextureShader.Program, "scale");
                Transform::Scale(camera.aspect, 1.0f, 1.0f, RTMesh->Scale._mat4);
                glUniformMatrix4fv(_RT_ModelScale, 1, GL_FALSE, RTMesh->Scale._mat4);

                renderTextureShader.init = true;
            }

            glDrawArrays(GL_TRIANGLES, 0, 6);
            // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
            glBindVertexArray(0);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);

            //-----------------------------------------------------------------

            // printf("Degbug\n");
            glfwSwapBuffers(glfw.window);
        }
        delete[] (texAttri);
        texAttri = NULL;
        glDeleteRenderbuffers(1, &depthRenderBufferId);
        glDeleteFramebuffers(1, &attri.FBO[0]);
        glfwTerminate();
    }
    else
    {
        printf("Init the glfw failed");
    }

    return 0;
}

//{{cos(b) cos(c), -cos(b) sin(c), sin(b), 0}, {sin(a) sin(b) cos(c) + cos(a) sin(c), sin(a) sin(b) sin(c) + cos(a) cos(c), -sin(a) cos(b), 0}, {-cos(a) sin(b) cos(c) + sin(a) sin(c), cos(a) sin(b) sin(c) + sin(a) cos(c), cos(b), 0}, {0, 0, 0, 1}}
