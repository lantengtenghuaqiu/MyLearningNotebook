#include "../includes/Window.hpp"
#include "../includes/Event.hpp"

int main()
{
    GLFW glfw;
    GLAD<float> glad;
    if (Init_GLFW_GLAD(glfw, glad))
    {
        ObjectID OID;
        OID.CreateNewIndex(OID.VAO, 1);
        OID.CreateNewIndex(OID.VBO, 1);
        OID.CreateNewIndex(OID.EBO, 1);
        OID.CreateNewIndex(OID.FBO, 1);
        OID.CreateNewIndex(OID.UBO, 1);

        Picture::ImageManager *picture = &Picture::ImageManager::GetInstance();

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