#ifndef COMPONENTS
#define COMPONENTS
#include "../Data.hpp"
#include "../BasicIncludes.hpp"
#include "./BasicComponent.hpp"
#include "../XMath.hpp"
// Components part
struct ImageComponent
{
    ColorComponent Color;
};

struct TransformComponent
{
    /* data */
private:
    Vec4 scale;
    Vec4 rotation;
    Vec4 position;

public:
    mat4 Scale;
    mat4 Rotation;
    mat4 Translate;

    mat4 Transform;

    bool isMoveing = false;
    void UpdataModelMatrix()
    {
#ifdef TRANSFORM_DEBUG
        for (int i = 0; i < 16; i++)
        {
            printf("%f ", this->Transform._mat4[i]);
            if ((i + 1) % 4 == 0)
                printf("\n");
        }
        printf("\n");
#endif
        this->Transform = this->Translate * this->Rotation * this->Scale;
    }

    TransformComponent()
    {
        this->scale = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        this->rotation = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        this->position = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    TransformComponent(Vec4 pos, Vec4 rot, Vec4 scl)
    {
        this->scale = scl;
        this->rotation = rot;
        this->position = pos;
    }

    // Scale
    Vec4 GetScale() { return this->scale; }
    float GetScaleX() const { return this->scale.x; }
    float GetScaleY() const { return this->scale.y; }
    float GetScaleZ() const { return this->scale.z; }

    void SetScale(Vec4 scl) { this->scale += scl; }
    void SetScale(float x, float y, float z)
    {
        this->scale.x += x;
        this->scale.y += y;
        this->scale.z += z;
    }
    void SetScaleX(float x) { this->scale.x += x; }
    void SetScaleY(float y) { this->scale.x += y; }
    void SetScaleZ(float z) { this->scale.x += z; }

    // Rotation
    Vec4 GetRotation() { return this->rotation; }
    float GetRotationX() const { return this->rotation.x; }
    float GetRotationY() const { return this->rotation.y; }
    float GetRotationZ() const { return this->rotation.z; }

    void SetRotation(Vec4 rot) { this->rotation += rot; }
    void SetRotation(float x, float y, float z)
    {
        this->rotation.x += x;
        this->rotation.y += y;
        this->rotation.z += z;
    }
    void SetRotationX(float x) { this->rotation.x += x; }
    void SetRotationY(float y) { this->rotation.y += y; }
    void SetRotationZ(float z) { this->rotation.z += z; }

    // Position
    Vec4 GetPosition() const { return this->position; }
    float GetPositionX() const { return this->position.x; }
    float GetPositionY() const { return this->position.y; }
    float GetPositionZ() const { return this->position.z; }

    void SetPosition(Vec4 pos) { this->position += pos; }
    void SetPosition(float x, float y, float z)
    {
        this->position.x += x;
        this->position.y += y;
        this->position.z += z;
    }
    void SetPositionX(float x) { this->position.x += x; }
    void SetPositionY(float y) { this->position.y += y; }
    void SetPositionZ(float z) { this->position.z += z; }
};

struct MeshRenderComponents
{
    unsigned int MeshCount;
    unsigned int ElementObjectCount;
    unsigned int VertexCount;
};

struct ShadowMap
{
    unsigned int framebufferId;
    unsigned int textureId;
    unsigned int location;

    int textureSize = 1024;

    Vec4 right;
    Vec4 up;
    Vec4 forward;

    mat4 shadowMatrix;

    float size = 20.0f;
    float near = 0.01f;
    float far = 10.0f;

    void InitShadowMapMatrix(int type)
    {
        mat4 projection;
        mat4 space;
        if (type == Ortho)
        {
            projection._mat4[0] = 2.0f / (this->size);

            projection._mat4[5] = 2.0f / (this->size);

            projection._mat4[10] = -2.0f / (this->far - this->near);

            projection._mat4[14] = -(this->far + this->near) / (this->far - this->near);
        }

        this->right = Math::Cross(this->forward, 0.0f, 1.0f, 0.0f);
        this->up = Math::Cross(this->right, this->forward);

        space._mat4[0] = (this->right).x;
        space._mat4[1] = (this->up).x;
        space._mat4[2] = (this->forward).x;

        space._mat4[4] = (this->right).y;
        space._mat4[5] = (this->up).y;
        space._mat4[6] = (this->forward).y;

        space._mat4[8] = (this->right).z;
        space._mat4[9] = (this->up).z;
        space._mat4[10] = (this->forward).z;

        this->shadowMatrix = projection * space;
    }
};

using ComponentTypes = std::variant<TransformComponent, ImageComponent, MeshRenderComponents>;

#endif