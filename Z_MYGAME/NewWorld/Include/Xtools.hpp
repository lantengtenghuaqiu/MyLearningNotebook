#ifndef XYLTOOLS
#define XYLTOOLS

#include "./BasicIncludes.hpp"

#include "./Assimp/AssimpManger.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "./STBI/stb_image.h"

#include <cstring>

namespace Tools
{
    // Image Manger
    // struct ImageManager
    // {
    //     static ImageManager *GetInstance();
    //     unsigned char *data;
    //     char *path;

    //     int width;
    //     int height;
    //     int channels;

    //     ImageManager(const ImageManager &) = delete;
    //     ImageManager &operator=(const ImageManager &) = delete;

    // private:
    //     ImageManager() = default;
    //     ~ImageManager() {};
    // };

    // File Manager
    class TheFileManager
    {
    private:
        FILE *File;
        TheFileManager() = default;
        ~TheFileManager() {};

    public:
        static TheFileManager *GetInstance();

        char *path;

        void GetContent(const char *path, const char *mode, char *&container);

        void GetFileData(const char *revalpath, char *&data);

        TheFileManager(const TheFileManager &) = delete;
        TheFileManager operator=(TheFileManager &) = delete;
    };

    class AssimpLoderManger
    {
        /* data */
    private:
        AssimpLoderManger() {}
        AssimpLoderManger &operator=(AssimpLoderManger) = delete;
        AssimpLoderManger(AssimpLoderManger &) = delete;
        ~AssimpLoderManger() {}

    public:
        static AssimpLoderManger *GetInstance()
        {
            static AssimpLoderManger instance;
            return &instance;
        }

        // Assimp manging--
        Assimp::Importer Importer;
        unsigned int CurrentMeshCount = 0;
        // ----------------

        // Self manging----
        unsigned int CurrentEBOCount;
        unsigned int CurrentVertexCount;
        unsigned int CurrentUVCount;
        unsigned int CurrentNormalCount;

        unsigned int *ElementObjectContainer = nullptr;
        float *VertexContainer = nullptr;
        float *UVContainer = nullptr;
        float *NormalContainer = nullptr;
        // ----------------

        const char *modelFilePath;
        unsigned int modelProcessMode;

        void ClearUp()
        {
            if (this->CurrentMeshCount != 0)
            {
                if (this->VertexContainer != nullptr)
                {
                    delete[] this->VertexContainer;
                    this->VertexContainer = nullptr;
                }
                if (this->ElementObjectContainer != nullptr)
                {
                    delete[] ElementObjectContainer;
                    ElementObjectContainer = nullptr;
                }
            }
        }

        void GetModelContents(unsigned int infoMode, unsigned int &meshCount)
        {
            this->ClearUp();
            const aiScene *scene = Importer.ReadFile(this->modelFilePath, this->modelProcessMode);
            if (scene)
            {
                this->CurrentMeshCount = scene->mNumMeshes;
                meshCount = this->CurrentMeshCount;
                printf("CurrentMeshCount : %d\n", scene->mNumMeshes);
            }

            if (this->CurrentMeshCount > 0)
            {
                aiMesh **mesh = new aiMesh *[scene->mNumMeshes];

                for (unsigned int i = 0; i < this->CurrentMeshCount; i++)
                {
                    mesh[i] = (*scene).mMeshes[i];

                    if (infoMode & FETCH::MESH)
                    {
                        this->CurrentVertexCount += mesh[i]->mNumVertices * 3;
                    }
                    if (infoMode & FETCH::EBO)
                    {
                        this->CurrentEBOCount += mesh[i]->mNumFaces;
                    }
                }

                int tempVertexCurrentCount = 0;
                int tempEBOCurrentCount = 0;
                for (int i = 0; i < 1; i++)
                {

                    if (infoMode & FETCH::MESH)
                    {
                        this->VertexContainer = new float[this->CurrentVertexCount];
                        int index = 0;
                        for (unsigned int k = 0; k < mesh[i]->mNumVertices; k++)
                        {
                            this->VertexContainer[index + tempVertexCurrentCount] = mesh[i]->mVertices[k].x;
                            this->VertexContainer[index + tempVertexCurrentCount + 1] = mesh[i]->mVertices[k].y;
                            this->VertexContainer[index + tempVertexCurrentCount + 2] = mesh[i]->mVertices[k].z;
                            index += 3;
                            printf("%f %f %f\n", this->VertexContainer[k], this->VertexContainer[k + 1], this->VertexContainer[k + 2]);
                        }
                        tempVertexCurrentCount += mesh[i]->mNumVertices;
                    }

                    if (infoMode & FETCH::EBO)
                    {
                        this->ElementObjectContainer = new unsigned int[this->CurrentEBOCount * 3];
                        int index = 0;
                        for (unsigned int k = 0; k < mesh[i]->mNumFaces; k++)
                        {

                            this->ElementObjectContainer[index + tempEBOCurrentCount] = mesh[i]->mFaces[k].mIndices[0];
                            this->ElementObjectContainer[index + tempEBOCurrentCount + 1] = mesh[i]->mFaces[k].mIndices[1];
                            this->ElementObjectContainer[index + tempEBOCurrentCount + 2] = mesh[i]->mFaces[k].mIndices[2];
                            // printf("%d %d %d\n", mesh[i]->mFaces[k].mIndices[0], mesh[i]->mFaces[k].mIndices[1], mesh[i]->mFaces[k].mIndices[2]);
                            printf("%d %d %d\n", this->ElementObjectContainer[index], this->ElementObjectContainer[index + 1], this->ElementObjectContainer[index + 2]);
                            index += 3;
                        }
                        tempEBOCurrentCount += mesh[i]->mNumFaces;
                    }
                }
            }
            else
            {
                printf("Wrong aiScene Load\n");
            }
        }

        void ReturnCountInformation(unsigned int mode, unsigned int &objectCount)
        {
            if (this->CurrentMeshCount != 0)
            {

                if (mode == FETCH::MESH)
                {
                    objectCount = this->CurrentVertexCount;
                }
                else if (mode == FETCH::EBO)
                {
                    objectCount = this->CurrentEBOCount;
                }
                else if (mode == FETCH::UV)
                {
                    objectCount = this->CurrentUVCount;
                }
                else if (mode == FETCH::NORMAL)
                {
                    objectCount = this->CurrentNormalCount;
                }
            }
        }
        void SetModelPathAndProcessMode(const char *path, unsigned int mode)
        {
            this->modelFilePath = path;
            this->modelProcessMode = mode;
        }
    };

    // template <typename T, size_t N>
    // size_t GetSizeofArray(T (&Array)[N])
    // {
    //     return N;
    // }

} // namespace Tools

typedef struct ObjectIndexManger
{
    static ObjectIndexManger *GetIntance();
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

    ObjectIndexManger(const ObjectIndexManger &) = delete;
    ObjectIndexManger operator=(ObjectIndexManger &) = delete;

    // unsigned int currentVAOIndex = 0;
    void CreateAndGenObjectIndex(unsigned int OID_Type, const unsigned int &size);

    int GetCID_VAO(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_VAO;
            this->CID_VAO++;
            return temp;
        }
        else
        {
            return this->CID_VAO;
        }
    }

    int GetCID_VBO(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_VBO;
            this->CID_VBO++;
            return temp;
        }
        else
        {
            return this->CID_VBO;
        }
    }

    int GetCID_EBO(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_EBO;
            this->CID_EBO++;
            return temp;
        }
        else
        {
            return this->CID_EBO;
        }
    }
    int GetCID_FBO(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_FBO;
            this->CID_FBO++;
            return temp;
        }
        else
        {
            return this->CID_FBO;
        }
    }
    int GetCID_TEX(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_TEX;
            this->CID_TEX++;
            return temp;
        }
        else
        {
            return this->CID_TEX;
        }
    }
    int GetCID_UBO(const char mode)
    {
        if (mode == 'w')
        {
            int temp = this->CID_UBO;
            this->CID_UBO++;
            return temp;
        }
        else
        {
            return this->CID_UBO;
        }
    }

private:
    int CID_VAO = 0;
    int CID_VBO = 0;
    int CID_EBO = 0;
    int CID_TEX = 0;
    int CID_FBO = 0;
    int CID_UBO = 0;

    ObjectIndexManger() : VAO(nullptr), VBO(nullptr), EBO(nullptr), TEX(nullptr), FBO(nullptr), UBO(nullptr)
    {
        this->CID_VAO = 0;
        this->CID_VBO = 0;
        this->CID_EBO = 0;
        this->CID_TEX = 0;
        this->CID_FBO = 0;
        this->CID_UBO = 0;
    }

    ~ObjectIndexManger()
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
#endif