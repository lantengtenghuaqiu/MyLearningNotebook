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
void ObjectIndexInitiater(ObjectIndexManger *OID);
void BindShader(GLAD &glad, Tools::TheFileManager *&file, Shader &shader, const char *path_vertex, const char *path_fragment);
void DrawCallGlobalConfigs();

float speed = 0.0f;

int main()
{
    GLFW glfw;
    GLAD glad;
    int SRC_WIDTH = 350;
    int SRC_HEIGHT = 200;
    int SRC_FRAMEBUFFER_WIDTH=0;
    int SRC_FRAMEBUFFER_HEIGHT=0;
    if (Init_GLFW_GLAD(glfw, SRC_WIDTH, SRC_HEIGHT, glad, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT))
    {
        // Mangers
        ObjectIndexManger *OID = ObjectIndexManger::GetIntance();
        ObjectIndexInitiater(OID);
        Tools::ImageManager *image = Tools::ImageManager::GetInstance();
        Tools::TheFileManager *file = Tools::TheFileManager::GetInstance();

        // --------------------------------------------------------------------
        // Textures------------------------------------------------------------

        // --------------------------------------------------------------------

        // --------------------------------------------------------------------
        // Scene Objects-------------------------------------------------------
        Hierarchy hierarchy;
        // Global Scene Object-----------------------------
        // Camera------------------------------------------
        hierarchy.sceneObjects.emplace("Camera", Camera());
        Camera *HierarchyPointer_MainCamera = &std::get<Camera>(hierarchy.sceneObjects["Camera"]);
        {
            SetMainCamera(HierarchyPointer_MainCamera, 10.0f, 10.0f, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT);
            BindTransformUniformBufferObject(OID, HierarchyPointer_MainCamera);
        }

        // Sunlight-----------------------------------------
        hierarchy.sceneObjects.emplace("Sunlight", DirectionalLight());
        DirectionalLight *HierarchyPointer_sunlight = &std::get<DirectionalLight>(hierarchy.sceneObjects["Sunlight"]);
        {
            Vec4 SunlightDir = Normalize(0.0, 1.0, -1.0, 0.0);
            HierarchyPointer_sunlight->Direction.Set(SunlightDir);
            BindLightBufferObject(OID, HierarchyPointer_sunlight);

            // Shadow Configs
            HierarchyPointer_sunlight->InitShadowMap(1);
            HierarchyPointer_sunlight->GetShadowMap()->forward = SunlightDir;
            HierarchyPointer_sunlight->GetShadowMap()[0].size = 100.0f;
            HierarchyPointer_sunlight->GetShadowMap()[0].near = 0.01f;
            HierarchyPointer_sunlight->GetShadowMap()[0].far = 200.0f;
            HierarchyPointer_sunlight->GetShadowMap()[0].InitShadowMapMatrix(Ortho);

            BindShadowMapTexture(HierarchyPointer_sunlight->GetShadowMap()[0]);
            BindShadowMapUniformBufferObject(OID, HierarchyPointer_sunlight->GetShadowMap()[0]);
        }

        Shader Shader_shadowMap;
        BindShader(glad, file, Shader_shadowMap, "/D_OpenGL/Assets/Materials/Shadow.vertex", "/D_OpenGL/Assets/Materials/Shadow.fragment");
        GlobalLocation::ShadowMap location_shadowmap;
        location_shadowmap._M = glGetUniformLocation(Shader_shadowMap.Program, "M");
        BindUBOBlock(Shader_shadowMap.Program, "ShadowMatrix", 2);
        // --------------------------------------------------------------------

        // --------------------------------------------------------------------
        // Cube Map Box------------------------------------
        hierarchy.sceneObjects.emplace("CubeMap", SceneObject());
        SceneObject *HierarchyPointer_cubmap = &std::get<SceneObject>(hierarchy.sceneObjects["CubeMap"]);
        {
        }
        BindSceneObject<float>(OID, Cube::mesh, Cube::normal, Cube::uv);
        BindSixCubeMapTexture(OID, image);

        Shader Shader_cubeMap;
        BindShader(glad, file, Shader_cubeMap, "/D_OpenGL/Assets/Materials/CubeMap.vertex", "/D_OpenGL/Assets/Materials/CubeMap.fragment");

        GlobalLocation::CubeMap location_cubmap;
        location_cubmap._skybox = glGetUniformLocation(Shader_cubeMap.Program, "skybox");

        BindUBOBlock(Shader_cubeMap.Program, "Transform", 0);

        // --------------------------------------------------------------------

        // --------------------------------------------------------------------
        // Container---------------------------------------
        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        SceneObject *HierarchyPointer_cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);

        BindSceneObject<float>(OID, Cube::mesh, Cube::normal, Cube::uv);

        Shader Shader_container;
        BindShader(glad, file, Shader_container, "/D_OpenGL/A_3_Shadow/Shaders/box.vertex", "/D_OpenGL/A_3_Shadow/Shaders/box.fragment");
        // Loactions
        Location_Container location_container;
        location_container._M = glGetUniformLocation(Shader_container.Program, "M");
        location_container._ShadowMapTexture = glGetUniformLocation(Shader_container.Program, "ShadowMapTexture");

        BindUBOBlock(Shader_container.Program, "Transform", 0);
        BindUBOBlock(Shader_container.Program, "LightMatirx", 1);
        BindUBOBlock(Shader_container.Program, "ShadowMap", 2);

        // --------------------------------------------------------------------

        // --------------------------------------------------------------------
        // Ground------------------------------------------
        hierarchy.sceneObjects.emplace("Ground", SceneObject());

        SceneObject *HierarchyPointer_ground = &std::get<SceneObject>(hierarchy.sceneObjects["Ground"]);
        BindSceneObject<float>(OID, Plane::mesh, Plane::normal, Plane::uv);

        Shader Shader_ground;
        BindShader(glad, file, Shader_ground, "/D_OpenGL/A_3_Shadow/Shaders/ground.vertex", "/D_OpenGL/A_3_Shadow/Shaders/ground.fragment");

        // Loactions
        Location_Ground location_ground;
        location_ground._M = glGetUniformLocation(Shader_ground.Program, "M");
        location_ground._ShadowMapTexture = glGetUniformLocation(Shader_ground.Program, "ShadowMapTexture");
        BindUBOBlock(Shader_ground.Program, "Transform", 0);
        BindUBOBlock(Shader_ground.Program, "LightMatirx", 1);
        BindUBOBlock(Shader_ground.Program, "ShadowMap", 2);
        // --------------------------------------------------------------------

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

            // ----------------------------------------------------------------
            // Shadow Map----------------------------------
            glViewport(0, 0, HierarchyPointer_sunlight->GetShadowMap()[0].textureSize, HierarchyPointer_sunlight->GetShadowMap()[0].textureSize);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, HierarchyPointer_sunlight->GetShadowMap()[0].framebufferId);
            glClear(GL_DEPTH_BUFFER_BIT);
            glUseProgram(Shader_shadowMap.Program);
            glUniformMatrix4fv(location_shadowmap._M, 1, GL_FALSE, HierarchyPointer_cube->Transform._mat4);
            glad.BindVAOAndDrawArrays(OID->VAO[1], GL_TRIANGLES, 0, 36);

            glUniformMatrix4fv(location_shadowmap._M, 1, GL_FALSE, HierarchyPointer_ground->Transform._mat4);
            glad.BindVAOAndDrawArrays(OID->VAO[2], GL_TRIANGLES, 0, 6);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // ----------------------------------------------------------------

            // ----------------------------------------------------------------
            // Camera--------------------------------------
            GlobalCameraRotate(glfw.window, HierarchyPointer_MainCamera, speed * 10.0f);
            GlobalCameraTranslate(glfw.window, HierarchyPointer_MainCamera, speed * 10.0f);
            glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[0]);
            glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 2, MAT4_SIZE, HierarchyPointer_MainCamera->RotationMatrix);
            glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 3, MAT4_SIZE, HierarchyPointer_MainCamera->TranslateMatrix);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            // ----------------------------------------------------------------

            // ----------------------------------------------------------------
            // Draw the cube map box-----------------------
            glViewport(0, 0, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT);
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


            glad.BindVAOAndDrawArrays(OID->VAO[0], GL_TRIANGLES, 0, 36);

            // ----------------------------------------------------------------

            // ----------------------------------------------------------------
            // Draw a container----------------------------
            glEnable(GL_DEPTH_TEST);
            glCullFace(GL_BACK);
            glDepthMask(GL_TRUE);

            glUseProgram(Shader_container.Program);
            glBindTexture(GL_TEXTURE_2D, HierarchyPointer_sunlight->GetShadowMap()[0].textureId);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(location_container._ShadowMapTexture, 0);
            // Key Board Event-----------------------------
            KeyRotate(glfw.window, *HierarchyPointer_cube, speed * 35.0f, HierarchyPointer_cube->Rotation._mat4);
            KeyTranslate(glfw.window, *HierarchyPointer_cube, speed * 10.0f, HierarchyPointer_cube->Translate._mat4);

            if (Shader_container.init == false)
            {
                Shader_container.init = true;
            }

            HierarchyPointer_cube->UpdataModelMatrix();
            glUniformMatrix4fv(location_container._M, 1, GL_FALSE, HierarchyPointer_cube->Transform._mat4);

            glad.BindVAOAndDrawArrays(OID->VAO[1], GL_TRIANGLES, 0, 36);

            // ----------------------------------------------------------------

            // ----------------------------------------------------------------
            // Ground----------------------------------------------------------
            glUseProgram(Shader_ground.Program);
            glBindTexture(GL_TEXTURE_2D, HierarchyPointer_sunlight->GetShadowMap()[0].textureId);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(location_ground._ShadowMapTexture, 0);

            if (Shader_ground.init == false)
            {
                Transform::Scale(15.0f, 1.0f, 15.0f, HierarchyPointer_ground->Scale._mat4);
                Transform::Rotation(0.0f, 0.0f, 0.0f, HierarchyPointer_ground->Rotation._mat4);
                Transform::Translate(0.0f, -5.0f, 35.0f, HierarchyPointer_ground->Translate._mat4);

                HierarchyPointer_ground->UpdataModelMatrix();
                glUniformMatrix4fv(location_ground._M, 1, GL_FALSE, HierarchyPointer_ground->Transform._mat4);
                Shader_ground.init = true;
            }
            glad.BindVAOAndDrawArrays(OID->VAO[2], GL_TRIANGLES, 0, 6);

            // ----------------------------------------------------------------

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

void ObjectIndexInitiater(ObjectIndexManger *OID)
{
    OID->CreateAndGenObjectIndex(OID_VAO, 3);
    glGenVertexArrays(OID->sizeVAO, OID->VAO);

    OID->CreateAndGenObjectIndex(OID_VBO, 3);
    glGenBuffers(OID->sizeVBO, OID->VBO);

    OID->CreateAndGenObjectIndex(OID_TEX, 1);
    glGenTextures(OID->sizeTEX, OID->TEX);

    OID->CreateAndGenObjectIndex(OID_EBO, 1);
    glGenBuffers(OID->sizeEBO, OID->EBO);

    OID->CreateAndGenObjectIndex(OID_UBO, 3);
    glGenBuffers(OID->sizeUBO, OID->UBO);

    OID->CreateAndGenObjectIndex(OID_FBO, 1);
}


void DrawCallGlobalConfigs()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Draw Calls
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}
