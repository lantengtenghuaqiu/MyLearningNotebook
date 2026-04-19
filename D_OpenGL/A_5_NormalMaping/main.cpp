#include <chrono>
#include "../includes/Window.hpp"
#include "../includes/Event.hpp"
#include "./LocationManger.hpp"
// mkdir -p builders && clang -c ../src/glad.c  -I ../includes -o ./builders/glad.o && clang++ -c main.cpp -I ../includes -std=c++17 -o ./builders/main.o && clang++ ./builders/main.o ./builders/glad.o -L ../libs/mac -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++17  -o main

void glClearConfig();
void ObjectIndexInitiater(ObjectIndexManger *OID);
float speed;

int main()
{
    GLFW glfw;
    GLAD glad;
    float SRC_RATIO = 16.0f / 12.0f;
    int SRC_WIDTH = 520;
    int SRC_HEIGHT = SRC_WIDTH / SRC_RATIO;
    int SRC_FRAMEBUFFER_WIDTH;
    int SRC_FRAMEBUFFER_HEIGHT;

    if (Init_GLFW_GLAD(glfw, SRC_WIDTH, SRC_HEIGHT, glad, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT))
    {
        // Mangers-------------------------------------------------------------
        ObjectIndexManger *OID = ObjectIndexManger::GetIntance();
        ObjectIndexInitiater(OID);
        Tools::ImageManager *Imager = Tools::ImageManager::GetInstance();
        Tools::TheFileManager *Filer = Tools::TheFileManager::GetInstance();
        Tools::AssimpLoderManger *ModelLoader = Tools::AssimpLoderManger::GetInstance();
        // --------------------------------------------------------------------

        // Hierarchy-----------------------------------------------------------
        Hierarchy hierarchy;
        // Global Scene Objects---------
        // Global Camera
        hierarchy.sceneObjects.emplace("GlobalCamera", Camera());
        Camera *Hpointer_GlobalCamera = &std::get<Camera>(hierarchy.sceneObjects["GlobalCamera"]);
        {
            SetMainCamera(Hpointer_GlobalCamera, 20, 20, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT);
            BindTransformUniformBufferObject(OID, Hpointer_GlobalCamera);
        }

        // Cube Map Box
        hierarchy.sceneObjects.emplace("CubeMapBox", SceneObject());
        SceneObject *Hpointer_CubeMapBox = &std::get<SceneObject>(hierarchy.sceneObjects["CubeMapBox"]);
        Shader Shader_CubeMap;
        GlobalLocation::CubeMap Location_CubeMap;
        {
            BindSceneObjectMeshOnly(OID, Cube::mesh);
            BindShader(glad, Filer, Shader_CubeMap, "/D_OpenGL/Assets/Materials/CubeMap.vertex", "/D_OpenGL/Assets/Materials/CubeMap.fragment");
            BindSixCubeMapTexture(OID, Imager);
            Location_CubeMap._skybox = glGetUniformLocation(Shader_CubeMap.Program, "skybox");
            BindUBOBlock(Shader_CubeMap.Program, "Transform", 0);
        }
        // ------------------------------------------------
        // Scene Objects-----------------------------------
        // Light Group
        // Direction Light
        hierarchy.sceneObjects.emplace("DirectionLight", DirectionalLight());
        DirectionalLight *Hpointer_DirectionLight = &std::get<DirectionalLight>(hierarchy.sceneObjects["DirectionLight"]);
        {
            Vec4 sunlight = Normalize(0.0f, -1.0f, 1.0f, 0.0f);
            BindLightBufferObject(OID, Hpointer_DirectionLight);
        }
        // Point Light
        hierarchy.sceneObjects.emplace("PointLight", PointLight());
        PointLight *Hpointer_PointLight = &std::get<PointLight>(hierarchy.sceneObjects["PointLight"]);
        Shader Shader_PointLight;
        LocationManger::PointLight Location_PointLight;
        {
            ModelLoader->SetModelPathAndProcessMode("/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/Assets/Models/planet.obj", aiProcess_Triangulate);
            ModelLoader->GetModelContents(FETCH::MESH | FETCH::EBO, Hpointer_PointLight->MeshCount);
            ModelLoader->ReturnCountInformation(FETCH::MESH, Hpointer_PointLight->VertexCount);
            ModelLoader->ReturnCountInformation(FETCH::EBO, Hpointer_PointLight->ElementObjectCount);

            Hpointer_PointLight->SetScale(0.1f, 0.1f, 0.1f);
            Hpointer_PointLight->SetPosition(-3.0f, 1.5f, 10.0f);

            // BindMeshVertexOnly(OID, ModelLoader);
            BindMesh(OID, ModelLoader, FETCH::MESH | FETCH::EBO);
            BindShader(glad, Filer, Shader_PointLight, "/D_OpenGL/Assets/Materials/SpecialShaders/VertexOnly.vertex", "/D_OpenGL/Assets/Materials/SpecialShaders/WhiteShader.fragment");
            Location_PointLight._M = glGetUniformLocation(Shader_PointLight.Program, "_M");
        }
        // ------------------------------------------------
        // 3D Mesh Objects---------------------------------
        // ContainerBox
        hierarchy.sceneObjects.emplace("ContainerBox", SceneObject());
        SceneObject *Hpointer_ContainerBox = &std::get<SceneObject>(hierarchy.sceneObjects["ContainerBox"]);
        Shader Shader_ContainerBox;
        LocationManger::Containerbox Location_ContainerBox;
        {
            BindSceneObject(OID, Cube::mesh, Cube::normal, Cube::uv);
            BindShader(glad, Filer, Shader_ContainerBox, "/D_OpenGL/A_5_NormalMaping/Shaders/ContainerBox.vertex", "/D_OpenGL/A_5_NormalMaping/Shaders/ContainerBox.fragment");
            Location_ContainerBox._M = glGetUniformLocation(Shader_ContainerBox.Program, "_M");
            BindUBOBlock(Shader_ContainerBox.Program, "Transform", 0);
        }
        // ------------------------------------------------
        // --------------------------------------------------------------------
        // Clear config
        glClearConfig();
        //---------------------------------------------------------------------
        auto first_frame_time = std::chrono::high_resolution_clock::now();
        auto last_frame_time = std::chrono::high_resolution_clock::now();
        auto current_frame_time = std::chrono::high_resolution_clock::now();
        float deltTime = 0;
        float time = 0;
        //---------------------------------------------------------------------
        // Main While----------------------------------------------------------
        while (!glfwWindowShouldClose(glfw.window))
        {
            // ----------------------------------------------------------------
            // 帧速率:
            current_frame_time = std::chrono::high_resolution_clock::now();
            deltTime = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - last_frame_time).count();
            last_frame_time = std::chrono::high_resolution_clock::now();
            time = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - first_frame_time).count();
            speed = deltTime;
            // ----------------------------------------------------------------
            glfwPollEvents();

            // Global Camera Fresh
            GlobalCameraRotate(glfw.window, Hpointer_GlobalCamera, speed * 20.0f);
            GlobalCameraTranslate(glfw.window, Hpointer_GlobalCamera, speed * 10.0f);
            Hpointer_GlobalCamera->GlobalCameraSpaceUpdata(OID->UBO[0]);

            // DrawCalls-----------------------------------
            // Global--------------------------------------
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CW);

            // --------------------------------------------
            glCullFace(GL_FRONT);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            // Cube Map Sky Box
            glUseProgram(Shader_CubeMap.Program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, OID->TEX[0]);
            glUniform1i(Location_CubeMap._skybox, 0);
            if (Shader_CubeMap.init == false)
            {
                Shader_CubeMap.init = true;
            }
            glad.BindVAOAndDrawArrays(OID->VAO[0], GL_TRIANGLES, 0, 36);

            // Draw Calls --- Scene Objects
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDepthMask(GL_TRUE);
            glCullFace(GL_BACK);
            glEnable(GL_DEPTH_TEST);

            // Point Light Sphere
            glUseProgram(Shader_PointLight.Program);
            if (Shader_PointLight.init == false)
            {
                Transform::Scale(Hpointer_PointLight->GetScale(), Hpointer_PointLight->Scale._mat4);
                Transform::Translate(Hpointer_PointLight->GetPosition(), Hpointer_PointLight->Translate._mat4);
                Hpointer_PointLight->UpdataModelMatrix();
                glUniformMatrix4fv(Location_PointLight._M, 1, GL_FALSE, Hpointer_PointLight->Transform._mat4);
                Shader_PointLight.init = true;
            }
            // glad.BindVAOAndDrawArrays(OID->VAO[1], GL_TRIANGLES, Hpointer_PointLight);
            // glad.BindVAOAndDrawElement(OID.EBO[0],)
            glBindVertexArray(OID->VAO[1]);
            glDrawElements(GL_TRIANGLES,Hpointer_PointLight->ElementObjectCount*3,GL_UNSIGNED_INT,0);
            glBindVertexArray(0);


            // Container Box
            glUseProgram(Shader_ContainerBox.Program);
            if (Shader_ContainerBox.init == false)
            {
                Transform::Scale(2.0f, 2.0f, 2.0f, Hpointer_ContainerBox->Scale._mat4);
                Transform::Rotation(0.0f, -10.0f, 0.0f, Hpointer_ContainerBox->Rotation._mat4);
                Transform::Translate(1.5f, 0.0f, 10.0f, Hpointer_ContainerBox->Translate._mat4);
                Hpointer_ContainerBox->UpdataModelMatrix();
                glUniformMatrix4fv(Location_ContainerBox._M, 1, GL_FALSE, Hpointer_ContainerBox->Transform._mat4);
                Shader_ContainerBox.init = true;
            }
            glad.BindVAOAndDrawArrays(OID->VAO[2], GL_TRIANGLES, 0, 36);
            // ----------------------------------------------------------------
            glfwSwapBuffers(glfw.window);
            // ----------------------------------------------------------------
        }
        glfwTerminate();
    }
    else
    {
        printf("Init GLFW And GLAD With Some Thing Wrong !!!\n");
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