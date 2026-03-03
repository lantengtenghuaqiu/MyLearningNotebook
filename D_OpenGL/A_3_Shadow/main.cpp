// clang -c ../src/glad.c  -I ../includes -o ./builder/glad.o &&
// clang++ -c main.cpp -I ../includes -std=c++17 -o ./builder/main.o &&
// clang++ ./builder/main.o ./builder/glad.o -L ../libs/mac -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++17  -o main

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
        // Vertices vertex;

        Hierarchy hierarchy;

        // Camera--------------------------------------------------------------
        hierarchy.sceneObjects.emplace("Camera", Camera());
        Camera *HierarchyPointer_camera = &std::get<Camera>(hierarchy.sceneObjects["Camera"]);
        {
            SetCamera(HierarchyPointer_camera, 10.0f, 10.0f);
        }
        BindTransformUniformBufferObject(OID, HierarchyPointer_camera);

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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // Draw Calls
            glEnable(GL_CULL_FACE);
            // Draw the cube map box
            glCullFace(GL_FRONT);
            glDepthMask(GL_FALSE);
            glUseProgram(Shader_cubeMap.Program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, OID->TEX[0]);
            glUniform1i(location_cubmap._skybox, 0);

            if (Shader_cubeMap.init == false)
            {

                Shader_cubeMap.init = true;
            }

            glBindVertexArray(OID->VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);
            glCullFace(GL_BACK);

            // Draw a container------------------------------------------------
            glUseProgram(Shader_container.Program);

            // Key Board Event-----------------------------
            // KeyRotate(glfw.window, *HierarchyPointer_cube, speed * 35.0f, HierarchyPointer_cube->Rotation._mat4);
            // KeyTranslate(glfw.window, *HierarchyPointer_cube, speed * 20.0f, HierarchyPointer_cube->Translate._mat4);

            if (Shader_container.init == false)
            {
                Transform::TranslateZ(5.0f, HierarchyPointer_cube->Translate._mat4);
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
