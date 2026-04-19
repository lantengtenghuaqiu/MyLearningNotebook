#include "IncludeManager.hpp"

// --------------------------------------------------------------------------------------
// Definitions:
#define InitGLFW                                                                          \
    if (GLFW::GetInstance()->InitGlfw(GLFW::GetInstance()->window, SCREEN_width, SCREEN_height, "NEWWORLD", nullptr, nullptr) == FAIL) \
    {                                                                                     \
        GLFWINITSTATUS;                                                                   \
    }

#define InitGLAD                                                                               \
    if (GLAD::GetInstance()->InitGlad(GLFW::GetInstance()->window, SCREEN_frameBufferWidth, SCREEN_frameBufferHeight) == FAIL) \
    {                                                                                          \
        GLADINITSTATUS;                                                                        \
    }

#define GladCleaningScreen                     \
    {                                          \
        glClearColor(0.2f, 0.2f, 0.22f, 1.0f); \
        glClearStencil(0.0f);                  \
        glClearDepth(1.0f);                    \
    }

// ------------------------------------
// ------------------------------------
// Overall Parameters
static int SCREEN_width = 1080;
static int SCREEN_height = 720;
static int SCREEN_frameBufferWidth = 0;
static int SCREEN_frameBufferHeight = 0;
// --------------------------------------------------------------------------------------
void ObjectIndexInitiater(ObjectIndexManger *OID);

int main()
{
    HELLOWORLD;
    //-----------------------
    // Init Logic
    InitGLFW;
    InitGLAD;
    // -
    // Managers
    ObjectIndexManger *OIDManager = ObjectIndexManger::GetIntance();
    ObjectIndexInitiater(OIDManager);

    // -
    // Hierarchy
    HIERARCHY Hierarchy;
    Hierarchy.Root.emplace("Plane", SceneObject());
    SceneObject *Plane = &std::get<SceneObject>(Hierarchy.Root["Planes"]);
    BindSceneObject(OIDManager,Plane::mesh);

    Shader ShaderPlane; 
    
    BindShader(ShaderPlane,"/C_Assets/Shaders/VertexOnly.vertex", "/C_Assets/Shaders/WhiteShader.fragment");
    
    //-----------------------

    GladCleaningScreen;
    while (!glfwWindowShouldClose(GLFW::GetInstance()->window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwSwapBuffers(GLFW::GetInstance()->window);
    }
    glfwTerminate();
    printf("%p\n",GLFW::GetInstance()->window);
    return 0;
}

void ObjectIndexInitiater(ObjectIndexManger *OID)
{
    OID->CreateAndGenObjectIndex(OID_VAO, 1);
    glGenVertexArrays(OID->sizeVAO, OID->VAO);

    OID->CreateAndGenObjectIndex(OID_VBO, 1);
    glGenBuffers(OID->sizeVBO, OID->VBO);

    // OID->CreateAndGenObjectIndex(OID_TEX, 1);
    // glGenTextures(OID->sizeTEX, OID->TEX);

    // OID->CreateAndGenObjectIndex(OID_EBO, 1);
    // glGenBuffers(OID->sizeEBO, OID->EBO);

    // OID->CreateAndGenObjectIndex(OID_UBO, 1);
    // glGenBuffers(OID->sizeUBO, OID->UBO);

    // OID->CreateAndGenObjectIndex(OID_FBO, 1);
}