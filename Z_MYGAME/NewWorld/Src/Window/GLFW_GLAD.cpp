#include "../../Include/Window/CreateWindow.hpp"

void CompileAndAttachShader(unsigned int shader_program, unsigned int shader_id, char *&shader_data, const char *shader_type)
{
    printf("Compile Shader %s : %d\n", shader_type, shader_id);

    glShaderSource(shader_id, 1, &shader_data, NULL);
    glCompileShader(shader_id);

    int checker;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &checker);

    if (checker == FAIL)
    {
        ShaderChecker
    }
    else
    {
        printf("Attached shader program(%d -> %d).\n", shader_id, shader_program);
        glAttachShader(shader_program, shader_id);
    }
    printf("\n");
    if (shader_data != NULL)
    {
        delete[] shader_data;
        shader_data = NULL;
    }
}
void LinkShader(unsigned int &shader_program)
{
    glLinkProgram(shader_program);
    int checker;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &checker);
    if (checker == FAIL)
    {
        LinkChecker
    }
}
void DetachAndDeleteShaders(Shader shader)
{
    // Detch And Delete Shaders
    glDetachShader(shader.Program, shader.Vertex);
    glDetachShader(shader.Program, shader.Fragment);
    glDeleteShader(shader.Vertex);
    glDeleteShader(shader.Fragment);
}

// Contains 3 methord:
// 1.Compile and Attach the Vertex and Fragment  Shader
// 2.LinkShader
// 3.DetachAndDeleteShaders
void BindShader(Shader &shader, const char *path_vertex, const char *path_fragment)
{
    Tools::TheFileManager::GetInstance()->GetFileData(path_vertex, shader.ShaderData);
    CompileAndAttachShader(shader.Program, shader.Vertex, shader.ShaderData, "Vertex");
   
    Tools::TheFileManager::GetInstance()->GetFileData(path_fragment, shader.ShaderData);
    CompileAndAttachShader(shader.Program, shader.Fragment, shader.ShaderData, "Fragment");

    LinkShader(shader.Program);

    DetachAndDeleteShaders(shader);
}

void UseShaderProgram(unsigned int shader_program)
{
    if (shader_program != FAIL)
    {
        printf("Use the shader program : %d\n", shader_program);
        glUseProgram(shader_program);
    }
    else
        printf("Shader program has something wrong\n");
}