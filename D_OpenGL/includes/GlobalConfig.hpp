#ifndef GLOBAL_DATAS
#define GLOBAL_DATAS

// #include "../includes/SceneObject.hpp"

// 全局数据------------------------------------------------

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

typedef struct ObjectCurrentIndex
{
    int VAO = 0;
    int VBO = 0;
    int Vertex = 0;
    int EBO = 0;
    int TEX = 0;
    int FBO = 0;
    int UBO = 0;
    ObjectCurrentIndex() {}

} ObjectCurrentID;

#define OID_VAO 0x900
#define OID_VBO 0x901
#define OID_EBO 0x902
#define OID_TEX 0x903
#define OID_UBO 0x904
#define OID_FBO 0x905

typedef struct ObjectIndex
{
    static ObjectIndex *GetIntance()
    {
        static ObjectIndex instance;
        return &instance;
    }

    ObjectCurrentIndex currentId;

    int CID_VAO = 0;
    int CID_VBO = 0;
    int CID_Vertex = 0;
    int CID_EBO = 0;
    int CID_TEX = 0;
    int CID_FBO = 0;
    int CID_UBO = 0;

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
    void CreateAndGenObjectIndex(unsigned int OID_Type, const unsigned int &size)
    {
        switch (OID_Type)
        {
        case OID_VAO:
        {
            if (this->VAO == nullptr)
            {
                printf("Create a new VAO\n");
                this->VAO = new unsigned int[size];
                this->sizeVAO = size;
            }
        }
        break;

        case OID_VBO:
        {
            if (this->VBO == nullptr)
            {
                printf("Create a new VAO\n");
                this->VBO = new unsigned int[size];
                this->sizeVBO = size;
            }
        }
        break;

        case OID_EBO:
        {
            if (this->EBO == nullptr)
            {
                printf("Create a new VAO\n");
                this->EBO = new unsigned int[size];
                this->sizeEBO = size;
            }
        }
        break;

        case OID_TEX:
        {
            if (this->TEX == nullptr)
            {
                printf("Create a new VAO\n");
                this->TEX = new unsigned int[size];
                this->sizeTEX = size;
            }
        }
        break;

        case OID_UBO:
        {
            if (this->UBO == nullptr)
            {
                printf("Create a new VAO\n");
                this->UBO = new unsigned int[size];
                this->sizeUBO = size;
            }
        }
        break;

        case OID_FBO:
        {
            if (this->FBO == nullptr)
            {
                printf("Create a new VAO\n");
                this->FBO = new unsigned int[size];
                this->sizeFBO = size;
                this->CID_FBO++;
            }
        }
        break;

        default:
            break;
        }
    }

private:
    ObjectIndex() : VAO(nullptr), VBO(nullptr), EBO(nullptr), TEX(nullptr), FBO(nullptr), UBO(nullptr) {}

    ~ObjectIndex()
    {
        if (this->VBO != nullptr)
        {
            delete[] (this->VBO);
            this->VBO = nullptr;
        }
        if (this->EBO != nullptr)
        {
            delete[] (this->EBO);
            this->EBO = nullptr;
        }

        if (this->TEX != nullptr)
        {
            delete[] (this->TEX);
            this->TEX = nullptr;
        }

        if (this->VAO != nullptr)
        {
            delete[] (this->VAO);
            this->VAO = nullptr;
        }

        if (this->FBO != nullptr)
        {
            delete[] (this->FBO);
            this->FBO = nullptr;
        }

        if (this->UBO != nullptr)
        {
            delete[] (this->UBO);
            this->UBO = nullptr;
        }
    }
} ObjectID;

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

void SetCamera(Camera &camera, const float &frameW, const float &frameH)
{
    camera.SetPosition(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    camera.fov = 30.0f;
    camera.w = frameW;
    camera.h = frameH;
    camera.n = 0.01f;
    camera.f = 30.0f;
    camera.aspect = frameW / frameH;
    camera.ViewMatrix();
}
void ClearGLAttribute()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClearStencil(0.0f);
}
#endif