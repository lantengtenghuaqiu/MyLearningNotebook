#include "../Include/Xtools.hpp"
#include "Xtools.hpp"

// Tools::ImageManager *Tools::ImageManager::GetInstance()
// {
//     static ImageManager instance;
//     return &instance;
// }

// File Manager--------------------------------------------
Tools::TheFileManager *Tools::TheFileManager::GetInstance()
{
    static TheFileManager instance;
    return &instance;
}

void Tools::TheFileManager::GetContent(const char *path, const char *mode, char *&container)
{
    Tools::TheFileManager * FileManager = Tools::TheFileManager::GetInstance();

    FileManager->File = fopen(path, mode);
    if (FileManager->File)
    {
        fseek(FileManager->File, 0, SEEK_END);

        long file_size = ftell(FileManager->File);

        fseek(FileManager->File, 0, SEEK_SET);

        container = new char[file_size + 1]();

        fread(container, 1, file_size, FileManager->File);

// #ifdef DEBUG
//         for (long i = 0; i < file_size; i++)
//         {
//             printf("%c", container[i]);
//         }
//         printf("\n");
// #endif
    }
    else
    {
        printf("File open is wrong");
    }

    fclose(FileManager->File);
}

void Tools::TheFileManager::GetFileData(const char *revalpath, char *&data)
{
    if (revalpath == nullptr) {
        return;
    }
    Tools::TheFileManager * FileManager = Tools::TheFileManager::GetInstance();

    if (FileManager->path != NULL)
    {
        delete[] FileManager->path;
        FileManager->path = NULL;
    }

    if (data != NULL)
    {
        delete[] data;
        data = NULL;
    }

    const char *basePath = BASEPATH;

    FileManager->path = new char[strlen(basePath) + strlen(revalpath) + 1];
    snprintf(FileManager->path, strlen(basePath) + strlen(revalpath) + 1, "%s%s", basePath, revalpath);

    // #ifdef FILE_DEBUG
    //     printf("basePath size : %zu\n", strlen(basePath));
    //     printf("revalpath size : %zu\n", strlen(revalpath));
    //     printf("file->path size : %zu\n", strlen(file->path));
    //     for (size_t i = 0; i < strlen(basePath) + strlen(revalpath) + 1; i++)
    //     {
    //         printf("%c", file->path[i]);
    //     }
    //     printf("\n");
    // #endif

    FileManager->GetContent(FileManager->path, "rb", data);
}



//Object Index Manager-------------------------------------
ObjectIndexManger *ObjectIndexManger::GetIntance()
{
    static ObjectIndexManger instance;
    return &instance;
}

void ObjectIndexManger::CreateAndGenObjectIndex(unsigned int OID_Type, const unsigned int &size)
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
            printf("Create a new VBO\n");
            this->VBO = new unsigned int[size];
            this->sizeVBO = size;
        }
    }
    break;

    case OID_EBO:
    {
        if (this->EBO == nullptr)
        {
            printf("Create a new EBO\n");
            this->EBO = new unsigned int[size];
            this->sizeEBO = size;
        }
    }
    break;

    case OID_TEX:
    {
        if (this->TEX == nullptr)
        {
            printf("Create a new TEX\n");
            this->TEX = new unsigned int[size];
            this->sizeTEX = size;
        }
    }
    break;

    case OID_UBO:
    {
        if (this->UBO == nullptr)
        {
            printf("Create a new UBO\n");
            this->UBO = new unsigned int[size];
            this->sizeUBO = size;
        }
    }
    break;

    case OID_FBO:
    {
        if (this->FBO == nullptr)
        {
            printf("Create a new FBO\n");
            this->FBO = new unsigned int[size];
            this->sizeFBO = size;
        }
    }
    break;

    default:
        break;
    }
}