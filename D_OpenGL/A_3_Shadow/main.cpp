#include "../includes/Window.hpp"
#include "../includes/Event.hpp"

int main()
{
    GLFW glfw;
    GLAD<float> glad;
    if (Init_GLFW_GLAD(glfw, glad))
    {
        ObjectIndex * OID = ObjectIndex::GetIntance();
        OID->CreateAndGenObjectIndex(OID_VAO, 1);
        glGenVertexArrays(OID->sizeVAO, OID->VAO);
        
        OID->CreateAndGenObjectIndex(OID_VBO, 1);
        glGenBuffers(OID->sizeVBO, OID->VBO);

        OID->CreateAndGenObjectIndex(OID_TEX, 1);
        glGenTextures(OID->sizeTEX, OID->TEX);

        OID->CreateAndGenObjectIndex(OID_EBO, 1);
        glGenBuffers(OID->sizeEBO, OID->EBO);

        OID->CreateAndGenObjectIndex(OID_UBO, 1);
        glGenBuffers(OID->sizeUBO, OID->UBO);

        OID->CreateAndGenObjectIndex(OID_FBO, 1);

        Picture::ImageManager *picture = Picture::ImageManager::GetInstance();

        Vertices vertex;
        // glad.BindVBO(OID.VBO);

        glClearColor(0.2f, 0.2f, 0.22f, 1.0f);
        glClearStencil(0.0f);
        glClearDepth(1.0f);

        while (!glfwWindowShouldClose(glfw.window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glfwSwapBuffers(glfw.window);
        }
        glfwTerminate();
    }
    return 0;
}