// clang -c ../src/glad.c  -I ../includes -o ./builders/glad.o &&
// clang++ -c main.cpp -I ../includes -std=c++17 -o ./builders/main.o &&
// clang++ ./builders/main.o ./builders/glad.o -L ../libs/mac -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++17  -o main

// g++ main.cpp ..\src\glad.c -I ..\includes -L ..\libs -lopengl32 -lglfw3 -lgdi32 -lassimp -lz -fno-permissive -Wno-unused-variable -Wno-unused-parameter  -Wall -Wextra -std=c++17 -o main.exe
// #define FILE_DEBUG
// #define TRANSFORM_DEBUG

#include <chrono>
#include "../includes/Window.hpp"
#include "../includes/Event.hpp"
#include "LocationManger.hpp"

void glClearConfig();
void ObjectIndexManger(ObjectIndex *OID);
void BindShader(GLAD &glad, Tools::TheFile *&file, Shader &shader, const char *path_vertex, const char *path_fragment);
void DrawCallGlobalConfigs();

float speed = 0.0f;

int main()
{
    GLFW glfw;
    GLAD glad;
    if (Init_GLFW_GLAD(glfw, glad))
    {
        // Mangers
        ObjectIndex *OID = ObjectIndex::GetIntance();
        ObjectIndexManger(OID);
        Tools::ImageManager *image = Tools::ImageManager::GetInstance();
        Tools::TheFile *file = Tools::TheFile::GetInstance();

        // --------------------------------------------------------------------
        // Textures------------------------------------------------------------

        ShadowMap directLightShadow;
        directLightShadow.size = 20.0f;
        directLightShadow.near = 0.01f;
        directLightShadow.far = 10.0f;
        directLightShadow.InitShadowMapMatrix(Ortho);

        glGenTextures(1, &directLightShadow.textureId);
        glBindTexture(GL_TEXTURE_2D, directLightShadow.textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, directLightShadow.textureSize, directLightShadow.textureSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenFramebuffers(1, &directLightShadow.framebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER,directLightShadow.framebufferId);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,directLightShadow.textureId,0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        // Scene Objects-------------------------------------------------------
        Hierarchy hierarchy;
        // Global Scene Object-----------------------------
        // Camera------------------------------------------
        hierarchy.sceneObjects.emplace("Camera", Camera());
        Camera *HierarchyPointer_camera = &std::get<Camera>(hierarchy.sceneObjects["Camera"]);
        {
            SetCamera(HierarchyPointer_camera, 10.0f, 10.0f, frameBufferWidth, frameBufferHeight);
        }

        // Sunlight-----------------------------------------
        hierarchy.sceneObjects.emplace("Sunlight", Light());
        Light *HierarchyPointer_sunlight = &std::get<Light>(hierarchy.sceneObjects["Sunlight"]);
        {
            Vec4 SunlightDir = Normalize(0.0, 1.0, -1.0, 0.0);
            HierarchyPointer_sunlight->Direction.Set(SunlightDir);
        }

        BindTransformUniformBufferObject(OID, HierarchyPointer_camera, HierarchyPointer_sunlight, directLightShadow);
        // Cube Map Box------------------------------------
        hierarchy.sceneObjects.emplace("CubeMap", SceneObject());
        SceneObject *HierarchyPointer_cubmap = &std::get<SceneObject>(hierarchy.sceneObjects["CubeMap"]);
        {
        }
        BindSceneObject<float>(OID, Cube::mesh, Cube::normal, Cube::uv);

        BindCubeMapTexture(OID, image);

        Shader Shader_cubeMap;
        BindShader(glad, file, Shader_cubeMap, "/D_OpenGL/Assets/Materials/CubeMap.vertex", "/D_OpenGL/Assets/Materials/CubeMap.fragment");

        Loaction_CubeMap location_cubmap;
        location_cubmap._skybox = glGetUniformLocation(Shader_cubeMap.Program, "skybox");
        location_cubmap._M = glGetUniformLocation(Shader_cubeMap.Program, "M");

        // Container---------------------------------------
        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        SceneObject *HierarchyPointer_cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);

        BindSceneObject<float>(OID, Cube::mesh, Cube::normal, Cube::uv);

        Shader Shader_container;
        BindShader(glad, file, Shader_container, "/D_OpenGL/A_3_Shadow/Shaders/box.vertex", "/D_OpenGL/A_3_Shadow/Shaders/box.fragment");
        // Loactions
        Location_Container location_container;
        location_container._M = glGetUniformLocation(Shader_container.Program, "M");

        // --------------------------------------------------------------------
        // Clear config
        glClearConfig();
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
            last_frame_time = std::chrono::high_resolution_clock::now();
            time = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - first_frame_time).count();
            speed = deltTime;

            glfwPollEvents();
            DrawCallGlobalConfigs();
            // Camera
            CameraRotate(glfw.window, *HierarchyPointer_camera, speed * 10.0f);
            CameraTranslate(glfw.window, *HierarchyPointer_camera, speed);
            glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[0]);
            glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 2, MAT4_SIZE, HierarchyPointer_camera->RotationMatrix);
            glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 3, MAT4_SIZE, HierarchyPointer_camera->TranslateMatrix);

            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // HierarchyPointer_camera->UpdateCameraSpaceMatrix();
            // ----------------------------------------------------------------
            // Draw the cube map box-------------------------------------------
            glCullFace(GL_FRONT);
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);

            glUseProgram(Shader_cubeMap.Program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, OID->TEX[0]);
            glUniform1i(location_cubmap._skybox, 0);

            if (Shader_cubeMap.init == false)
            {

                Shader_cubeMap.init = true;
            }

            HierarchyPointer_cubmap->UpdataModelMatrix();
            glUniformMatrix4fv(location_cubmap._M, 1, GL_FALSE, HierarchyPointer_cubmap->Transform._mat4);

            glBindVertexArray(OID->VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);

            // ----------------------------------------------------------------
            // Draw a container------------------------------------------------
            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            glDepthMask(GL_TRUE);
            glUseProgram(Shader_container.Program);

            // Key Board Event-----------------------------
            KeyRotate(glfw.window, *HierarchyPointer_cube, speed * 35.0f, HierarchyPointer_cube->Rotation._mat4);
            KeyTranslate(glfw.window, *HierarchyPointer_cube, speed * 10.0f, HierarchyPointer_cube->Translate._mat4);

            if (Shader_container.init == false)
            {
                Shader_container.init = true;
            }

            HierarchyPointer_cube->UpdataModelMatrix();
            glUniformMatrix4fv(location_container._M, 1, GL_FALSE, HierarchyPointer_cube->Transform._mat4);

            glBindVertexArray(OID->VAO[1]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glfwSwapBuffers(glfw.window);
        }
        glfwTerminate();
    }
    return 0;
}

void glClearConfig()
{
    glClearColor(0.2f, 0.2f, 0.22f, 1.0f);
    glClearStencil(0.0f);
    glClearDepth(1.0f);
}

void ObjectIndexManger(ObjectIndex *OID)
{
    OID->CreateAndGenObjectIndex(OID_VAO, 2);
    glGenVertexArrays(OID->sizeVAO, OID->VAO);

    OID->CreateAndGenObjectIndex(OID_VBO, 2);
    glGenBuffers(OID->sizeVBO, OID->VBO);

    OID->CreateAndGenObjectIndex(OID_TEX, 1);
    glGenTextures(OID->sizeTEX, OID->TEX);

    OID->CreateAndGenObjectIndex(OID_EBO, 1);
    glGenBuffers(OID->sizeEBO, OID->EBO);

    OID->CreateAndGenObjectIndex(OID_UBO, 1);
    glGenBuffers(OID->sizeUBO, OID->UBO);

    OID->CreateAndGenObjectIndex(OID_FBO, 1);
}

void BindShader(GLAD &glad, Tools::TheFile *&file, Shader &shader, const char *path_vertex, const char *path_fragment)
{
#ifdef __APPLE__
    glad.GetShadersData(file, path_vertex, shader.ShaderData);
    glad.CompileAndAttachShader(shader.Program, shader.Vertex, shader.ShaderData, "Vertex");

    glad.GetShadersData(file, path_fragment, shader.ShaderData);
    glad.CompileAndAttachShader(shader.Program, shader.Fragment, shader.ShaderData, "Fragment");
#else
    glad.GetShadersData(file, path_vertex, shader.ShaderData);
    glad.CompileAndAttachShader(shader.Program, shader.Vertex, shader.ShaderData, "Vertex");

    glad.GetShadersData(file, path_fragment, shader.ShaderData);
    glad.CompileAndAttachShader(shader.Program, shader.Fragment, shader.ShaderData, "Fragment");
#endif

    glad.LinkShaderProgram(shader.Program);

    glad.DetachAndDeleteShaders(shader);
}

void DrawCallGlobalConfigs()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Draw Calls
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}