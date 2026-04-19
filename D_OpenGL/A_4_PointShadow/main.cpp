#include "../includes/Window.hpp"

void glClearConfig();
void ObjectIndexInitiater(ObjectIndexManger *OID);
float speed;

int main()
{
    GLFW glfw;
    GLAD glad;
    float SRC_RATIO = 16.0f / 9.0f;
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
        // --------------------------------------------------------------------

        // Hierarchy-----------------------------------------------------------
        Hierarchy hierarchy;
        // Global Scene Objects---------
        // Global Camera
        hierarchy.sceneObjects.emplace("GlobalCamera", Camera());
        Camera *Hpointer_GlobalCamera = &std::get<Camera>(hierarchy.sceneObjects["GlobalCamera"]);
        {
            SetMainCamera(Hpointer_GlobalCamera, 20, 20, SRC_FRAMEBUFFER_WIDTH, SRC_FRAMEBUFFER_HEIGHT);
            BindTransformUniformBufferObject(OID,Hpointer_GlobalCamera);
        }
        // Direction Light
        hierarchy.sceneObjects.emplace("DirectionLight", DirectionalLight());
        DirectionalLight *Hpointer_DirectionLight = &std::get<DirectionalLight>(hierarchy.sceneObjects["DirectionLight"]);
        {
            Vec4 sunlight = Normalize(0.0f,-1.0f,1.0f,0.0f);
            BindLightBufferObject(OID,Hpointer_DirectionLight);
        }
        // Cube Map Box
        hierarchy.sceneObjects.emplace("CubeMapBox", SceneObject());
        {
        }
        // ----------------------------
        // Scene Objects---------------
        

        // ----------------------------
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
            // ----------------------------------------------------------------
            // 帧速率:
            current_frame_time = std::chrono::high_resolution_clock::now();
            deltTime = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - last_frame_time).count();
            last_frame_time = std::chrono::high_resolution_clock::now();
            time = std::chrono::duration_cast<std::chrono::duration<float>>(current_frame_time - first_frame_time).count();
            speed = deltTime;
            // ----------------------------------------------------------------
            glfwPollEvents();


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