#ifndef CREATEWINDOW
#define CREATEWINDOW

#include "../BasicIncludes.hpp"
#include "./Glad/glad/glad.h"
#include "./GLFW/glfw3.h"

#include "../Xtools.hpp"
// #include "../../SceneObject/SceneObject.hpp"
// #include "../GlobalConfig.hpp"

#define ShaderChecker                               \
    printf("Wrong Compile (%d)\n", checker);        \
    char info[512];                                 \
    glGetShaderInfoLog(shader_id, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                   \
    {                                               \
        printf("%c", info[i]);                      \
    }                                               \
    printf("\n");

#define LinkChecker                                      \
    printf("Wrong Link (%d):\n", checker);               \
    char info[512];                                      \
    glGetShaderInfoLog(shader_program, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                        \
    {                                                    \
        printf("%c", info[i]);                           \
    }                                                    \
    printf("\n");

class GLFW
{
private:
    GLFW() = default;
    ~GLFW();

public:
    static GLFW *GetInstance();
    GLFWwindow *window = nullptr;
    int InitGlfw(GLFWwindow *&window, const int &width, const int &height, const char *title, GLFWmonitor *glfwmonitor = nullptr, GLFWwindow *share = nullptr);
};

class GLAD
{
private:
    GLAD() = default;
    ~GLAD() {}

public:
    static GLAD *GetInstance();

    int InitGlad(GLFWwindow *window, int &frameWidth, int &frameHeight);

    void BindEBO(unsigned int &EBO, const int data_size, const unsigned int *index, int ebo_draw_type);

    //     void BindVAOAndDrawArrays(unsigned int &VAO, const int &Type, const int &start, const int &end)
    //     {
    //         glBindVertexArray(VAO);
    //         glDrawArrays(Type, start, end);
    //         glBindVertexArray(0);
    //     }

    // int GetUniformLoaction(const unsigned int sahder_program, const char *keyword)
    // {
    //     return glGetUniformLocation(sahder_program, keyword);
    // }
};

typedef struct ShadersProgram
{
    unsigned int Program = 0;
    unsigned int Vertex = 0;
    unsigned int Fragment = 0;
    char *ShaderData;
    bool init = false;
    ShadersProgram();

} Shader;
void CompileAndAttachShader(unsigned int shader_program, unsigned int shader_id, char *&shader_data, const char *shader_type);
void LinkShader(unsigned int &shader_program);
void DetachAndDeleteShaders(Shader shader);
void UseShaderProgram(unsigned int shader_program);

// void BindUBOBlock(unsigned int shader_program, const char *name, int bling_id)
// {
//     unsigned int block_id = glGetUniformBlockIndex(shader_program, name);
//     glUniformBlockBinding(shader_program, block_id, bling_id);
// }

// void BindTransformUniformBufferObject(ObjectID *OID, Camera *camera)
// {
//     glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[OID->GetCID_UBO('r')]);
//     glBufferData(GL_UNIFORM_BUFFER, MAT4_SIZE * 4, NULL, GL_STATIC_DRAW);

//     glBufferSubData(GL_UNIFORM_BUFFER, 0, MAT4_SIZE, camera->projection);
//     glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE, MAT4_SIZE, camera->viewSpace);
//     glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 2, MAT4_SIZE, camera->RotationMatrix);
//     glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 3, MAT4_SIZE, camera->TranslateMatrix);
//     glBindBufferBase(GL_UNIFORM_BUFFER, 0, OID->UBO[OID->GetCID_UBO('w')]);

//     glBindBuffer(GL_UNIFORM_BUFFER, 0);
// }

// void BindLightBufferObject(ObjectID *OID, const DirectionalLight *Sunlight)
// {
//     glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[OID->GetCID_UBO('r')]);
//     glBufferData(GL_UNIFORM_BUFFER, VEC3_SIZE, NULL, GL_STATIC_DRAW);

//     glBufferSubData(GL_UNIFORM_BUFFER, 0, VEC3_SIZE, Sunlight->Direction.v3);

//     glBindBufferBase(GL_UNIFORM_BUFFER, 1, OID->UBO[OID->GetCID_UBO('w')]);
//     glBindBuffer(GL_UNIFORM_BUFFER, 0);
// }

// void BindShadowMapUniformBufferObject(ObjectID *OID, const ShadowMap &showMap)
// {
//     glBindBuffer(GL_UNIFORM_BUFFER, OID->UBO[OID->GetCID_UBO('r')]);
//     glBufferData(GL_UNIFORM_BUFFER, MAT4_SIZE, NULL, GL_STATIC_DRAW);

//     for (int i = 0; i < 16; i++)
//     {
//         printf("%f ", showMap.shadowMatrix._mat4[i]);
//         if ((i + 1) % 4 == 0)
//         {
//             printf("\n");
//         }
//     }
//     glBufferSubData(GL_UNIFORM_BUFFER, 0, MAT4_SIZE, showMap.shadowMatrix._mat4);
//     glBindBufferBase(GL_UNIFORM_BUFFER, 2, OID->UBO[OID->GetCID_UBO('w')]);

//     glBindBuffer(GL_UNIFORM_BUFFER, 0);
// }

// void BindShadowMapTexture(ShadowMap &shadow)
// {
//     glGenTextures(1, &shadow.textureId);
//     // printf("%d\n",shadow.textureId);
//     glBindTexture(GL_TEXTURE_2D, shadow.textureId);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow.textureSize, shadow.textureSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//     glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Color_white);

//     glGenFramebuffers(1, &shadow.framebufferId);
//     // printf("%d\n",shadow.framebufferId);

//     glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow.framebufferId);
//     glDrawBuffer(GL_NONE);
//     glReadBuffer(GL_NONE);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow.textureId, 0);

//     if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//     {
//         printf("Shadow frame buffer not complete！\n");
//     }

//     glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
// }

// // Bind the datas of 3D scene object mesh, normal and uv.
template <typename T, size_t N1, size_t N2, size_t N3>
void BindSceneObject(ObjectID *OID, T (&mesh)[N1], T (&normal)[N2] = nullptr, T (&uv)[N3] = nullptr)
{
    if (mesh == nullptr)
    {
        printf("Wrong Mesh Data");
        return;
    }
    size_t TotalMeshSize = N1;
    if (normal != nullptr)
        TotalMeshSize += N2;
    if (uv != nullptr)
        TotalMeshSize += N3;

    glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
    glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
    glBufferData(GL_ARRAY_BUFFER, TotalMeshSize * FLOAT_SIZE, NULL, GL_STATIC_DRAW);

    // Mesh Data
    glBufferSubData(GL_ARRAY_BUFFER, 0, N1 * FLOAT_SIZE, mesh);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    // Normal Data
    if (normal != nullptr)
    {
        glBufferSubData(GL_ARRAY_BUFFER, N1 * FLOAT_SIZE, N2 * FLOAT_SIZE, normal);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)(N1 * FLOAT_SIZE));
        glEnableVertexAttribArray(1);
    }
    // UV Data
    if (uv != nullptr)
    {
        glBufferSubData(GL_ARRAY_BUFFER, (N1 + N2) * FLOAT_SIZE, N3 * FLOAT_SIZE, uv);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)((N1 + N2) * FLOAT_SIZE));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// template <typename T, size_t N1, size_t N2>
// void BindSceneObject2D(ObjectID *OID, T (&mesh)[N1], T (&uv)[N2])
// {
//     glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
//     glBufferData(GL_ARRAY_BUFFER, (N1 + N2) * FLOAT_SIZE, NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, N1 * FLOAT_SIZE, mesh);
//     glBufferSubData(GL_ARRAY_BUFFER, N1 * FLOAT_SIZE, N2 * FLOAT_SIZE, uv);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
//     glEnableVertexAttribArray(0);

//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)((N1)*FLOAT_SIZE));
//     glEnableVertexAttribArray(1);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

// template <typename T, size_t N1>
// void BindSceneObjectMeshOnly(ObjectID *OID, T (&mesh)[N1])
// {
//     glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
//     glBufferData(GL_ARRAY_BUFFER, (N1)*FLOAT_SIZE, NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, N1 * FLOAT_SIZE, mesh);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

// void BindSceneObjectMeshOnly(ObjectID *OID, float *mesh, int size)
// {
//     glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
//     glBufferData(GL_ARRAY_BUFFER, (size)*FLOAT_SIZE, NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, size * FLOAT_SIZE, mesh);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

// void BindMeshVertexOnly(ObjectID *OID, Tools::AssimpLoderManger *modelManger)
// {
//     glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);

//     glBufferData(GL_ARRAY_BUFFER, (modelManger->CurrentVertexCount) * FLOAT_SIZE, NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, modelManger->CurrentVertexCount * FLOAT_SIZE, modelManger->VertexContainer);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

// void BindMesh(ObjectID *OID, Tools::AssimpLoderManger *modelManger, unsigned int mode)
// {
// glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);

// Tools::AssimpLoderManger ModelManager =

// if (mode & FETCH::EBO)
// {
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OID->EBO[OID->GetCID_EBO('w')]);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelManger->CurrentEBOCount * FLOAT_SIZE * 3, modelManger->ElementObjectContainer, GL_STATIC_DRAW);
//     // printf("%d\n",modelManger->CurrentEBOCount);
// }

// unsigned int totalCount = 0;
// if (mode & FETCH::MESH)
// {
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);
//     totalCount += modelManger->CurrentVertexCount;
//     // printf("%d\n",modelManger->CurrentVertexCount);
// }

// glBufferData(GL_ARRAY_BUFFER, (totalCount)*FLOAT_SIZE, NULL, GL_STATIC_DRAW);

// unsigned int offset = 0;
// unsigned int location = 0;

// if (mode & FETCH::MESH)
// {
//     glBufferSubData(GL_ARRAY_BUFFER, location, modelManger->CurrentVertexCount * FLOAT_SIZE, modelManger->VertexContainer);
//     glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)(offset));
//     glEnableVertexAttribArray(location);
//     offset += modelManger->CurrentVertexCount;
//     location++;
// }

// glBindBuffer(GL_ARRAY_BUFFER, 0);
// glBindVertexArray(0);
// }

// void BindSceneObjectMeshOnly(ObjectID *OID, float *vertexData, const unsigned int countSize)
// {
//     glBindVertexArray(OID->VAO[OID->GetCID_VAO('w')]);
//     glBindBuffer(GL_ARRAY_BUFFER, OID->VBO[OID->GetCID_VBO('w')]);

//     glBufferData(GL_ARRAY_BUFFER, (countSize)*FLOAT_SIZE, NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, countSize * FLOAT_SIZE, vertexData);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

void BindShader(Shader &shader, const char *path_vertex, const char *path_fragment);

// void BindSixCubeMapTexture(ObjectID *OID, Tools::ImageManager *&image)
// {
//     if (image->path != NULL)
//     {
//         delete[] image->path;
//         image->path = NULL;
//     }
// #ifdef __APPLE__
//     const char *base_path = "/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/Assets/Textures/skybox";
//     size_t base_path_size = strlen(base_path);
//     image->path = new char[base_path_size + 1];
//     strcpy(image->path, base_path);
// #else
//     const char *base_path = "G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Textures\\skybox";
//     size_t base_path_size = strlen(base_path);
//     image->path = new char[base_path_size + 1];
//     strcpy(image->path, base_path);
// #endif

//     const char *faces[] =
//         {
//             "right.jpg",
//             "left.jpg",
//             "top.jpg",
//             "bottom.jpg",
//             "front.jpg",
//             "back.jpg"};

//     glBindTexture(GL_TEXTURE_CUBE_MAP, OID->TEX[OID->GetCID_TEX('w')]);
//     // printf(" Tex : %d\n", OID->TEX[OID->GetCID_TEX('r')]);
//     for (int i = 0; i < 6; i++)
//     {
//         size_t reval_path = strlen(faces[i]);
//         size_t whole_path_size = base_path_size + reval_path + 2;
//         char *path = new char[whole_path_size];
//         for (size_t j = 0; j < whole_path_size; j++)
//         {
//             if (j < base_path_size)
//                 path[j] = image->path[j];
//             else if (j == base_path_size)
//                 path[j] = '/';
//             else
//                 path[j] = faces[i][(j - 1) - base_path_size];
// #ifdef FILE_DEBUG
//             printf("%c", path[j]);
// #endif
//         }
// #ifdef FILE_DEBUG
//         printf("\n");
// #endif

//         path[whole_path_size] = '\0';

//         // stbi_set_flip_vertically_on_load(1);
//         image->data = stbi_load(path, &image->width, &image->height, &image->channels, 0);
//         if (image->data != NULL)
//         {
//             glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
//             stbi_image_free(image->data);
//         }
//         else
//         {
//             printf("Wrong image data\n");
//         }
//         glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//         // stbi_set_flip_vertically_on_load(0);
//     }
//     glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
// }

// void BindHDRCubeMapTexture(ObjectID *OID, Tools::ImageManager *&image, const char *revealPath)
// {
// }

#endif
