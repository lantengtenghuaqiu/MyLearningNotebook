#include "../includes/Window.hpp"
#include "../includes/Event.hpp"

void glClearConfig();
void ObjectIndexManger(ObjectIndex *OID);

int main()
{
    GLFW glfw;
    GLAD<float> glad;
    if (Init_GLFW_GLAD(glfw, glad))
    {
        ObjectIndex *OID = ObjectIndex::GetIntance();
        ObjectIndexManger(OID);

        Picture::ImageManager *picture = Picture::ImageManager::GetInstance();
        ReadFile::TheFile *file = ReadFile::TheFile::GetInstance();

        Vertices vertex;

        Hierarchy hierarchy;

        // Container
        hierarchy.sceneObjects.emplace("Cube", SceneObject());
        SceneObject *Pointer_object_cube = &std::get<SceneObject>(hierarchy.sceneObjects["Cube"]);

        glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
        glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::mesh) + sizeof(Cube::normal) + sizeof(Cube::uv), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Cube::mesh), Cube::mesh);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cube::mesh), sizeof(Cube::normal), Cube::normal);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(Cube::mesh) + sizeof(Cube::normal), sizeof(Cube::uv), Cube::uv);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)sizeof(Cube::mesh));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)sizeof(Cube::mesh) + sizeof(Cube::normal));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Shader Shader_container;

        glad.GetShadersData(file, "/D_OpenGL/A_3_Shadow/box.vertex", Shader_container.ShaderData);
        glad.CompileAndAttachShader(Shader_container.Program, Shader_container.Vertex, Shader_container.ShaderData, "Vertex");

        glad.GetShadersData(file, "/D_OpenGL/A_3_Shadow/box.fragment", Shader_container.ShaderData);
        glad.CompileAndAttachShader(Shader_container.Program, Shader_container.Fragment, Shader_container.ShaderData, "Fragment");
        glad.LinkShaderProgram(Shader_container.Program);

        glad.DetachAndDeleteShaders(Shader_container);

        // --------------------------------------------------------------------
        // Clear config
        glClearConfig();

        // Frame flash
        while (!glfwWindowShouldClose(glfw.window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glUseProgram(Shader_container.Program);

            glBindVertexArray(OID->VAO[0]);
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
}