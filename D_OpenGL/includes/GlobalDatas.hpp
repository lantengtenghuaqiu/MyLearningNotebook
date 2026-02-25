#ifndef GLOBAL_DATAS
#define GLOBAL_DATAS

#include "../includes/SceneObject.hpp"
struct Vertices
{
    /* data */
    float *vertices;
    unsigned long long size;
    void Set(float *vertices, unsigned long long size)
    {
        this->vertices = vertices;
        this->size = size;
    }
} Vertex;

typedef struct ObjectAttributes
{
    unsigned int *VAO;
    unsigned int *VBO;
    unsigned int *EBO;
    unsigned int *TEX;
    unsigned int *FBO; // FrameBufferObjet
    unsigned int *UBO;

    unsigned int sizeVAO;
    unsigned int sizeVBO;
    unsigned int sizeEBO;
    unsigned int sizeTEX;
    unsigned int sizeFBO;
    unsigned int sizeUBO;

    // unsigned int currentVAOIndex = 0;

    ObjectAttributes() {}

    ObjectAttributes(unsigned int sizeVAO, unsigned int sizeVBO, unsigned int sizeEBO, unsigned int sizeTEX, unsigned int sizeFBO, unsigned int sizeUBO)
    {
        this->sizeVAO = sizeVAO;
        this->sizeVBO = sizeVBO;
        this->sizeEBO = sizeEBO;
        this->sizeTEX = sizeTEX;
        this->sizeFBO = sizeFBO;
        this->sizeUBO = sizeUBO;

        this->VAO = new unsigned int[sizeVAO];
        this->VBO = new unsigned int[sizeVBO];
        this->EBO = new unsigned int[sizeEBO];
        this->TEX = new unsigned int[sizeTEX];
        this->FBO = new unsigned int[sizeFBO];
        this->UBO = new unsigned int[sizeUBO];
    }
    ~ObjectAttributes()
    {

        delete[] (this->VBO);
        this->VBO = nullptr;

        delete[] (this->EBO);
        this->EBO = nullptr;

        delete[] (this->TEX);
        this->TEX = nullptr;

        delete[] (this->VAO);
        this->VAO = nullptr;

        delete[] (this->FBO);
        this->FBO = nullptr;
    }
} Attributes;

typedef struct TextureAttribute
{
    int width, height, nChanel;
    unsigned int textureLocation;

} TexAttri;

typedef struct ShadersProgram
{
    unsigned int Program = 0;
    unsigned int Vertex = 0;
    unsigned int Fragment = 0;
    char *ShaderData;
    bool init = false;
    ShadersProgram()
    {
        ShaderData = new char;
        this->Program = glCreateProgram();
        this->Vertex = glCreateShader(GL_VERTEX_SHADER);
        this->Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    }

} Shader;

typedef struct CurrentIndex
{
    int VAO = 0;
    int VBO = 0;
    int Vertex = 0;
    int EBO = 0;
    int TEX = 0;
    int FBO = 0;
    int UBO = 0;
} CurrentID;

void SetCamera(Camera &camera)
{
    camera.SetPosition(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    camera.fov = 30.0f;
    camera.w = 860.0f;
    camera.h = 720.0f;
    camera.n = 0.01f;
    camera.f = 30.0f;
    camera.aspect = 860.0f / 720.0f;
}

#endif