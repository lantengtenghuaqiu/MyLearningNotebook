#include <stdio.h>
#include "../includes/assimp/Importer.hpp"
#include "../includes/assimp/scene.h"
#include "../includes/assimp/postprocess.h"

struct Model
{
    unsigned int size;
    float *vertex;
    float *uv;
    float *normal;
};

int main()
{
    Assimp::Importer imoprter;
    const aiScene *scene = imoprter.ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\Assets\\Models\\Sphere.obj", aiProcess_Triangulate);
    Model model;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("Load error");
        return -1;
    }

    printf("Meshs total count : %d\n", scene->mNumMeshes);
    printf("Materials total count : %d\n", scene->mNumMaterials);

    model.size = scene->mMeshes[0]->mNumVertices * 3;
    printf("Mesh vertex count : %d\n", model.size);

    printf("\n=== All material names ===\n");
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial *mat = scene->mMaterials[i];
        aiString matName;
        // 获取材质名称（即使无MTL，也能拿到usemtl声明的名称）
        mat->Get(AI_MATKEY_NAME, matName);

        // 处理默认材质的空名称
        printf("Material %d name: %s\n", i + 1, matName.C_Str());
    }
    // 打印唯一Mesh实际使用的材质（关键：这个才是“真正用到的材质”）
    printf("\n=== Mesh actrul name ===\n");
    aiMesh *mesh = scene->mMeshes[0]; // 只有1个Mesh
    aiMaterial *usedMat = scene->mMaterials[mesh->mMaterialIndex];
    aiString usedMatName;
    usedMat->Get(AI_MATKEY_NAME, usedMatName);
    const char *usedName = usedMatName.length > 0 ? usedMatName.C_Str() : "$DefaultMaterial";
    printf("Mesh 1 use material: %s\n", usedName);
    return 0;
}