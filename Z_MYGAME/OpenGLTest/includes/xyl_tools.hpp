#ifndef XYLTOOLS
#define XYLTOOLS

#include "./BasicIncludes.hpp"

#include "./Assimp/AssimpManger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

// #ifdef DEBUG
// #define PRINT_VAR(x) printf("%s\n", #x);
// #else
// #define PRINT_VAR(x)
// #endif

namespace Tools
{
    // Image Manger
    struct ImageManager
    {
        static ImageManager *GetInstance()
        {
            static ImageManager instance;
            return &instance;
        }
        unsigned char *data;
        char *path;

        int width;
        int height;
        int channels;

        ImageManager(const ImageManager &) = delete;
        ImageManager &operator=(const ImageManager &) = delete;

    private:
        ImageManager() = default;
        ~ImageManager() {};
    };

    // File Manager
    class TheFileManager
    {
    private:
        FILE *_file;
        TheFileManager() = default;
        ~TheFileManager() {};

    public:
        static TheFileManager *GetInstance()
        {
            static TheFileManager instance;
            return &instance;
        }
        char *path;

        void GetContent(const char *path, const char *mode, char *&container)
        {
            this->_file = fopen(path, mode);
            if (this->_file)
            {
                fseek(this->_file, 0, SEEK_END);

                long file_size = ftell(this->_file);

                fseek(this->_file, 0, SEEK_SET);

                container = new char[file_size + 1]();

                fread(container, 1, file_size, this->_file);

#ifdef DEBUG
                for (long i = 0; i < file_size; i++)
                {
                    printf("%c", container[i]);
                }
                printf("\n");
#endif
            }
            else
            {
                printf("File open is wrong");
            }

            fclose(this->_file);
        }

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
                printf("CurrentMeshCount : %d\n",scene->mNumMeshes);
            }

            if (this->CurrentMeshCount > 0)
            {
                aiMesh **mesh = new aiMesh *[scene->mNumMeshes];

                for (int i = 0; i < this->CurrentMeshCount; i++)
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
                        int index=0;
                        for (int k = 0; k < mesh[i]->mNumVertices;k++)
                        {
                            this->VertexContainer[index + tempVertexCurrentCount] = mesh[i]->mVertices[k].x;
                            this->VertexContainer[index + tempVertexCurrentCount + 1] = mesh[i]->mVertices[k].y;
                            this->VertexContainer[index + tempVertexCurrentCount + 2] = mesh[i]->mVertices[k].z;
                            index+=3;
                            printf("%f %f %f\n", this->VertexContainer[k], this->VertexContainer[k + 1], this->VertexContainer[k + 2]);
                        }
                        tempVertexCurrentCount += mesh[i]->mNumVertices;
                    }

                    if (infoMode & FETCH::EBO)
                    {
                        this->ElementObjectContainer = new unsigned int[this->CurrentEBOCount*3];
                        int index=0;
                        for (int k = 0; k < mesh[i]->mNumFaces;k++)
                        {
                            
                            this->ElementObjectContainer[index + tempEBOCurrentCount] = mesh[i]->mFaces[k].mIndices[0];
                            this->ElementObjectContainer[index + tempEBOCurrentCount + 1] = mesh[i]->mFaces[k].mIndices[1];
                            this->ElementObjectContainer[index + tempEBOCurrentCount + 2] = mesh[i]->mFaces[k].mIndices[2];
                            // printf("%d %d %d\n", mesh[i]->mFaces[k].mIndices[0], mesh[i]->mFaces[k].mIndices[1], mesh[i]->mFaces[k].mIndices[2]);
                            printf("%d %d %d\n", this->ElementObjectContainer[index], this->ElementObjectContainer[index + 1], this->ElementObjectContainer[index + 2]);
                            index+=3;
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

    template <typename T, size_t N>
    size_t GetSizeofArray(T (&Array)[N])
    {
        return N;
    }
} // namespace Tools

#endif