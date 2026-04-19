#include <stdio.h>

enum MODES
{
    mode1 = 1<<0, // 0001
    mode2 = 1<<2, // 0010
    mode3 = 1<<3, // 0011
    mode4 = 1<<4, // 0100
    mode5 = 1<<5  // 0101
};

int main()
{
    unsigned int mode = MODES::mode4 | MODES::mode3 | MODES::mode2 | MODES::mode5;

    //
    printf("%d\n" , 1<<5);


    // 0011 & 0001 = 0001
    if (mode & MODES::mode1)
    {
        printf("Mode 1\n");
    }

    if (mode & MODES::mode2)
    {
        printf("Mode 2\n");
    }

    if (mode & MODES::mode3)
    {
        printf("Mode 3\n");
    }

    if (mode & MODES::mode4)
    {
        printf("Mode 4\n");
    }

    if (mode & MODES::mode5)
    {
        printf("Mode 5\n");
    }

    return 0;
}


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
    unsigned int CurrentEBOCount = 0;
    unsigned int CurrentVertexCount = 0;
    unsigned int CurrentUVCount = 0;

    float **ElementBufferObjectContainer;
    float **VertexContainer;
    float **UVContainer;
    // ----------------

    std::string modelFilePath;
    unsigned int modelProcessMode;

    void ClearUp()
    {
        if (this->CurrentMeshCount != 0)
        {
            if (this->CurrentVertexCount != 0)
            {
                for (int i = 0; i < this->CurrentMeshCount; i++)
                {
                    for (int j = 0; j < this->CurrentVertexCount; j++)
                    {
                        delete[] this->VertexContainer[i];
                        this->VertexContainer[i] = nullptr;
                    }
                }
                delete[] this->VertexContainer;
                this->VertexContainer = nullptr;
            }

            if (this->CurrentEBOCount != 0)
            {
                for (int i = 0; i < this->CurrentMeshCount; i++)
                {
                    for (int j = 0; j < this->CurrentEBOCount; j++)
                    {
                        delete[] ElementBufferObjectContainer[i];
                        ElementBufferObjectContainer[i] = nullptr;
                    }
                }
                delete[] ElementBufferObjectContainer;
                ElementBufferObjectContainer = nullptr;
            }
        }
    }

    void GetModelContents(std::string filePath, unsigned int &processMode, unsigned int infoMode)
    {
        this->ClearUp();
        const char* p="/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/Assets/Models/planet.obj";
        const aiScene *scene = Importer.ReadFile(p, processMode);

        if (scene && this->CurrentMeshCount > 0)
        {
            this->CurrentMeshCount = scene->mNumMeshes;
            aiMesh **mesh = new aiMesh *[scene->mNumMeshes];
            for (int i = 0; i < this->CurrentMeshCount; i++)
            {
                mesh[i] = (*scene).mMeshes[i];

                if (infoMode & FETCH::MESH)
                {
                    this->VertexContainer = new float *[this->CurrentMeshCount];

                    this->CurrentVertexCount = mesh[i]->mNumVertices;
                    printf("this->CurrentVertexCount:%d\n", this->CurrentVertexCount);
                    this->VertexContainer[i] = new float[this->CurrentVertexCount * 3];

                    for (int j = 0; j < this->CurrentVertexCount; j++)
                    {
                        this->VertexContainer[i][j] = mesh[i]->mVertices[j].x;
                        this->VertexContainer[i][j + 1] = mesh[i]->mVertices[j].y;
                        this->VertexContainer[i][j + 2] = mesh[i]->mVertices[j].z;
                        // printf("%f %f %f\n", this->VertexContainer[i][j], this->VertexContainer[i][j + 1], this->VertexContainer[i][j + 2]);
                    }
                }

                if (infoMode & FETCH::EBO)
                {
                    // if (eboCount != nullptr)
                    // {
                    //     delete[] eboCount;
                    //     eboCount = nullptr;
                    // }
                    // this->ElementBufferObjectContainer = new float *[this->CurrentMeshCount];
                    // eboCount = new unsigned int[this->CurrentMeshCount];
                    // for (int i = 0; i < this->CurrentMeshCount; i++)
                    // {
                    //     this->CurrentEBOCount = mesh[i]->mNumFaces * 3;
                    //     eboCount[i] = this->CurrentEBOCount;
                    // }
                    // ElementBufferObjectContainer = new float *[this->CurrentMeshCount];
                }

                // if()
            }
        }
        else
        {
            printf("Wrong aiScene Load\n");
        }
    }

    void SetModelPathAndProcessMode()
    {
    }
};


ModelLoader->modelFilePath = "/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/Assets/Models/planet.obj";
std::cout<< ModelLoader->modelFilePath<<std::endl;
ModelLoader->modelProcessMode = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
ModelLoader->GetModelContents(ModelLoader->modelFilePath, ModelLoader->modelProcessMode, FETCH::MESH);
Hpointer_PointLight->VertexCount[0] = ModelLoader->CurrentVertexCount;