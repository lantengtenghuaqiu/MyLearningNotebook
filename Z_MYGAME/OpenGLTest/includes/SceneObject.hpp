#ifndef SCENEOBJECT
#define SCENEOBJECT
#include "Data.hpp"
#include "./XMath.hpp"
#include "./Components.hpp"
#include "../includes/BasicIncludes.hpp"
#include "./Transformation.hpp"
// #include <iostream>
#include <variant>
#include <unordered_map>

// Components part
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

    mat4 TransformMat4;

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
        this->TransformMat4 = this->Translate * this->Rotation * this->Scale;
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

using ComponentTypes = std::variant<ColorComponent>;

struct SceneObject : public TransformComponent
{
    unsigned int MeshCount;
    unsigned int ElementObjectCount;
    unsigned int VertexCount;
    std::unordered_map<std::string, ComponentTypes> components;
};

struct Camera : public SceneObject
{
    float fov = 30.0f;
    float w = 2.0f;
    float h = 2.0f;
    float f = 50.0f;
    float n = 0.001f;
    float aspect = 1.0f;

    /* data */
    Vec4 target;

    Vec4 Forward;
    Vec4 Up;
    Vec4 Right;

    float RotationMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    float TranslateMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    float viewSpace[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    float projection[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    Camera()
    {
        this->Right.Set(1.0f, 0.0f, 0.0f, 0.0f);
        this->Up.Set(0.0f, 1.0f, 0.0f, 0.0f);
        this->Forward.Set(0.0f, 0.0f, -1.0f, 0.0f);
    }

    Camera(float f, float fov, float w, float h, float n, float aspect)
    {
        this->f = f;
        this->n = n;
        this->h = h;
        this->w = w;
        this->fov = fov;
        this->aspect = aspect;
    }

    void UpdateCameraSpaceMatrix()
    {

        // float CameraSpace[16] = {
        //     Right.x, Up.x, Forward.x, 0.0f,
        //     Right.y, Up.y, Forward.y, 0.0f,
        //     Right.z, Up.z, Forward.z, 0.0f,
        //     camera.x, camera.y, camera.z, 1.0f
        //};

        // Transform::Rotation(this->Forward, this->RotationMatrix);
        // Transform::Translate(this->GetPosition(), this->TranslateMatrix);

        this->viewSpace[0] = (this->Right).x;
        this->viewSpace[1] = (this->Up).x;
        this->viewSpace[2] = (this->Forward).x;

        this->viewSpace[4] = (this->Right).y;
        this->viewSpace[5] = (this->Up).y;
        this->viewSpace[6] = (this->Forward).y;

        this->viewSpace[8] = (this->Right).z;
        this->viewSpace[9] = (this->Up).z;
        this->viewSpace[10] = (this->Forward).z;

        // this->CameraSpace[12] = -Math::Dot(this->Right, this->GetPosition());
        // this->CameraSpace[13] = -Math::Dot(this->Up, this->GetPosition());
        // this->CameraSpace[14] = -Math::Dot(this->Forward, this->GetPosition());
    }

    void UpdateCameraProjectionMatrix(int Type)
    {
        // float CameraProjection[16] = {
        //     1.0f / (this->aspect * tan_fov)  ,   0.0f,               0.0f,                                               0.0f,
        //     0.0f,                                1.0f / (tan_fov),   0.0f,                                               0.0f,
        //     0.0f,                                0.0f,               (this->n + this->f) / (this->n - this->f),          -1.0f,
        //     0.0f,                                0.0f,               (2.0f * this->n * this->f) / (this->n - this->f),   0.0f
        //};
        if (Type == Ortho)
        {
            this->projection[0] = 2.0f / (this->w);

            this->projection[5] = 2.0f / (this->h);

            this->projection[10] = -2.0f / (this->f - this->n);

            this->projection[14] = -(this->f + this->n) / (this->f - this->n);
        }
        else
        {
            float tan_fov = tan((this->fov / 2.0f) * rad);

            this->projection[0] = 1.0f / (this->aspect * tan_fov);

            this->projection[5] = 1.0f / (tan_fov);

            this->projection[10] = (this->n + this->f) / (this->n - this->f);

            this->projection[11] = -1.0f;

            this->projection[14] = (2.0f * this->n * this->f) / (this->n - this->f);

            this->projection[15] = 0.0f;
        }
    }
    void GlobalCameraSpaceUpdata(unsigned int locationIndex)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, locationIndex);
        glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 2, MAT4_SIZE, this->RotationMatrix);
        glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE * 3, MAT4_SIZE, this->TranslateMatrix);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
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

struct Light : public SceneObject
{
protected:
    ShadowMap *shadow;
    unsigned int shadowCount;
    bool shadowInited = false;

public:
    Vec4 Color;
    float Intensity = 0.0f;

    Light()
    {
        this->Color = Vec4(1.0, 1.0, 1.0, 1.0);
        this->Intensity = 1.0f;
    }
    // Light(Vec4 color, float instensity)
    // {
    //     this->Color = color * instensity;
    //     this->Intensity = instensity;
    // }

    Vec4 GetColorMultipliedByInstensity()
    {
        return this->Color * this->Intensity;
    }

    Vec4 GetColor()
    {
        return this->Color;
    }

    ShadowMap *GetShadowMap()
    {
        if (this->shadowInited)
        {
            return this->shadow;
        }
        else
        {
            this->InitShadowMap(1);
            this->shadowInited = true;
            printf("********\n");
            printf("****Auto Init The Shadow Map !!!! count is : %d****\n", this->shadowCount);
            printf("********\n");
            return this->shadow;
        }
    }

    void InitShadowMap(const unsigned int count)
    {
        if (this->shadowInited)
        {
            printf("Shadow map can init once ,and is inited with count : %d\n", this->shadowCount);
        }
        else
        {
            this->shadow = new ShadowMap[count];
            this->shadowInited = true;
        }
    }
};

struct DirectionalLight : public Light
{
public:
    Vec4 Direction;
    DirectionalLight() : Light()
    {
        this->Direction = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public Light
{
    /* data */
    PointLight() : Light()
    {
    }
};

// Container for contain and get the scene object easly.
using VariantSceneObject = std::variant<SceneObject, DirectionalLight, PointLight, Camera>;

typedef class SceneObjectRoot
{
public:
    std::unordered_map<std::string, VariantSceneObject> sceneObjects;

} Hierarchy;

// Normal Scene Object Examples
// Includes the scene object of mesh normal and uv datas.
namespace Cube
{
    float mesh[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        // Left face
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        // Right face
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        // Top face
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f};
    float uv[] = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f};
    float normal[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
} // namespace Cube
namespace Plane
{
    float mesh[] = {
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f};
    float normal[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    float uv[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};
} // namespace Plane

namespace Sphere
{

    float mesh[] = {
        // 三角形1
        0.000000f, -1.000000f, 0.000000f,
        0.707107f, -0.707107f, 0.000000f,
        0.612372f, -0.707107f, 0.353553f,

        // 三角形2
        0.000000f, -1.000000f, 0.000000f,
        0.612372f, -0.707107f, 0.353553f,
        0.353553f, -0.707107f, 0.612372f,

        // 三角形3
        0.000000f, -1.000000f, 0.000000f,
        0.353553f, -0.707107f, 0.612372f,
        0.000000f, -0.707107f, 0.707107f,

        // 三角形4
        0.000000f, -1.000000f, 0.000000f,
        0.000000f, -0.707107f, 0.707107f,
        -0.353553f, -0.707107f, 0.612372f,

        // 三角形5
        0.000000f, -1.000000f, 0.000000f,
        -0.353553f, -0.707107f, 0.612372f,
        -0.612372f, -0.707107f, 0.353553f,

        // 三角形6
        0.000000f, -1.000000f, 0.000000f,
        -0.612372f, -0.707107f, 0.353553f,
        -0.707107f, -0.707107f, 0.000000f,

        // 三角形7
        0.000000f, -1.000000f, 0.000000f,
        -0.707107f, -0.707107f, 0.000000f,
        -0.612372f, -0.707107f, -0.353553f,

        // 三角形8
        0.000000f, -1.000000f, 0.000000f,
        -0.612372f, -0.707107f, -0.353553f,
        -0.353553f, -0.707107f, -0.612372f,

        // 三角形9
        0.000000f, -1.000000f, 0.000000f,
        -0.353553f, -0.707107f, -0.612372f,
        0.000000f, -0.707107f, -0.707107f,

        // 三角形10
        0.000000f, -1.000000f, 0.000000f,
        0.000000f, -0.707107f, -0.707107f,
        0.353553f, -0.707107f, -0.612372f,

        // 三角形11
        0.000000f, -1.000000f, 0.000000f,
        0.353553f, -0.707107f, -0.612372f,
        0.612372f, -0.707107f, -0.353553f,

        // 三角形12
        0.000000f, -1.000000f, 0.000000f,
        0.612372f, -0.707107f, -0.353553f,
        0.707107f, -0.707107f, 0.000000f,

        // ==================== 中纬带（南纬45°→赤道，12个三角形，36顶点）====================
        // 三角形13
        0.707107f, -0.707107f, 0.000000f,
        1.000000f, 0.000000f, 0.000000f,
        0.866025f, 0.000000f, 0.500000f,

        // 三角形14
        0.707107f, -0.707107f, 0.000000f,
        0.866025f, 0.000000f, 0.500000f,
        0.612372f, -0.707107f, 0.353553f,

        // 三角形15
        0.612372f, -0.707107f, 0.353553f,
        0.866025f, 0.000000f, 0.500000f,
        0.500000f, 0.000000f, 0.866025f,

        // 三角形16
        0.612372f, -0.707107f, 0.353553f,
        0.500000f, 0.000000f, 0.866025f,
        0.353553f, -0.707107f, 0.612372f,

        // 三角形17
        0.353553f, -0.707107f, 0.612372f,
        0.500000f, 0.000000f, 0.866025f,
        0.000000f, 0.000000f, 1.000000f,

        // 三角形18
        0.353553f, -0.707107f, 0.612372f,
        0.000000f, 0.000000f, 1.000000f,
        0.000000f, -0.707107f, 0.707107f,

        // 三角形19
        0.000000f, -0.707107f, 0.707107f,
        0.000000f, 0.000000f, 1.000000f,
        -0.500000f, 0.000000f, 0.866025f,

        // 三角形20
        0.000000f, -0.707107f, 0.707107f,
        -0.500000f, 0.000000f, 0.866025f,
        -0.353553f, -0.707107f, 0.612372f,

        // 三角形21
        -0.353553f, -0.707107f, 0.612372f,
        -0.500000f, 0.000000f, 0.866025f,
        -0.866025f, 0.000000f, 0.500000f,

        // 三角形22
        -0.353553f, -0.707107f, 0.612372f,
        -0.866025f, 0.000000f, 0.500000f,
        -0.612372f, -0.707107f, 0.353553f,

        // 三角形23
        -0.612372f, -0.707107f, 0.353553f,
        -0.866025f, 0.000000f, 0.500000f,
        -1.000000f, 0.000000f, 0.000000f,

        // 三角形24
        -0.612372f, -0.707107f, 0.353553f,
        -1.000000f, 0.000000f, 0.000000f,
        -0.707107f, -0.707107f, 0.000000f,

        // ==================== 中纬带（赤道→北纬45°，12个三角形，36顶点）====================
        // 三角形25
        -0.707107f, -0.707107f, 0.000000f,
        -1.000000f, 0.000000f, 0.000000f,
        -0.866025f, 0.000000f, -0.500000f,

        // 三角形26
        -0.707107f, -0.707107f, 0.000000f,
        -0.866025f, 0.000000f, -0.500000f,
        -0.612372f, -0.707107f, -0.353553f,

        // 三角形27
        -0.612372f, -0.707107f, -0.353553f,
        -0.866025f, 0.000000f, -0.500000f,
        -0.500000f, 0.000000f, -0.866025f,

        // 三角形28
        -0.612372f, -0.707107f, -0.353553f,
        -0.500000f, 0.000000f, -0.866025f,
        -0.353553f, -0.707107f, -0.612372f,

        // 三角形29
        -0.353553f, -0.707107f, -0.612372f,
        -0.500000f, 0.000000f, -0.866025f,
        0.000000f, 0.000000f, -1.000000f,

        // 三角形30
        -0.353553f, -0.707107f, -0.612372f,
        0.000000f, 0.000000f, -1.000000f,
        0.000000f, -0.707107f, -0.707107f,

        // 三角形31
        0.000000f, -0.707107f, -0.707107f,
        0.000000f, 0.000000f, -1.000000f,
        0.500000f, 0.000000f, -0.866025f,

        // 三角形32
        0.000000f, -0.707107f, -0.707107f,
        0.500000f, 0.000000f, -0.866025f,
        0.353553f, -0.707107f, -0.612372f,

        // 三角形33
        0.353553f, -0.707107f, -0.612372f,
        0.500000f, 0.000000f, -0.866025f,
        0.866025f, 0.000000f, -0.500000f,

        // 三角形34
        0.353553f, -0.707107f, -0.612372f,
        0.866025f, 0.000000f, -0.500000f,
        0.612372f, -0.707107f, -0.353553f,

        // 三角形35
        0.612372f, -0.707107f, -0.353553f,
        0.866025f, 0.000000f, -0.500000f,
        1.000000f, 0.000000f, 0.000000f,

        // 三角形36
        0.612372f, -0.707107f, -0.353553f,
        1.000000f, 0.000000f, 0.000000f,
        0.707107f, -0.707107f, 0.000000f,

        // ==================== 北半球（北纬45°→北极，12个三角形，36顶点）====================
        // 三角形37
        1.000000f, 0.000000f, 0.000000f,
        0.707107f, 0.707107f, 0.000000f,
        0.866025f, 0.000000f, 0.500000f,

        // 三角形38
        0.866025f, 0.000000f, 0.500000f,
        0.707107f, 0.707107f, 0.000000f,
        0.612372f, 0.707107f, 0.353553f,

        // 三角形39
        0.866025f, 0.000000f, 0.500000f,
        0.612372f, 0.707107f, 0.353553f,
        0.500000f, 0.000000f, 0.866025f,

        // 三角形40
        0.500000f, 0.000000f, 0.866025f,
        0.612372f, 0.707107f, 0.353553f,
        0.353553f, 0.707107f, 0.612372f,

        // 三角形41
        0.500000f, 0.000000f, 0.866025f,
        0.353553f, 0.707107f, 0.612372f,
        0.000000f, 0.000000f, 1.000000f,

        // 三角形42
        0.000000f, 0.000000f, 1.000000f,
        0.353553f, 0.707107f, 0.612372f,
        0.000000f, 0.707107f, 0.707107f,

        // 三角形43
        0.000000f, 0.000000f, 1.000000f,
        0.000000f, 0.707107f, 0.707107f,
        -0.500000f, 0.000000f, 0.866025f,

        // 三角形44
        -0.500000f, 0.000000f, 0.866025f,
        0.000000f, 0.707107f, 0.707107f,
        -0.353553f, 0.707107f, 0.612372f,

        // 三角形45
        -0.500000f, 0.000000f, 0.866025f,
        -0.353553f, 0.707107f, 0.612372f,
        -0.866025f, 0.000000f, 0.500000f,

        // 三角形46
        -0.866025f, 0.000000f, 0.500000f,
        -0.353553f, 0.707107f, 0.612372f,
        -0.612372f, 0.707107f, 0.353553f,

        // 三角形47
        -0.866025f, 0.000000f, 0.500000f,
        -0.612372f, 0.707107f, 0.353553f,
        -1.000000f, 0.000000f, 0.000000f,

        // 三角形48
        -1.000000f, 0.000000f, 0.000000f,
        -0.612372f, 0.707107f, 0.353553f,
        -0.707107f, 0.707107f, 0.000000f,

        // 剩余北半球三角形（完整闭合）
        // 三角形49
        -1.000000f, 0.000000f, 0.000000f,
        -0.707107f, 0.707107f, 0.000000f,
        -0.866025f, 0.000000f, -0.500000f,

        // 三角形50
        -0.866025f, 0.000000f, -0.500000f,
        -0.707107f, 0.707107f, 0.000000f,
        -0.612372f, 0.707107f, -0.353553f,

        // 三角形51
        -0.866025f, 0.000000f, -0.500000f,
        -0.612372f, 0.707107f, -0.353553f,
        -0.500000f, 0.000000f, -0.866025f,

        // 三角形52
        -0.500000f, 0.000000f, -0.866025f,
        -0.612372f, 0.707107f, -0.353553f,
        -0.353553f, 0.707107f, -0.612372f,

        // 三角形53
        -0.500000f, 0.000000f, -0.866025f,
        -0.353553f, 0.707107f, -0.612372f,
        0.000000f, 0.000000f, -1.000000f,

        // 三角形54
        0.000000f, 0.000000f, -1.000000f,
        -0.353553f, 0.707107f, -0.612372f,
        0.000000f, 0.707107f, -0.707107f,

        // 三角形55
        0.000000f, 0.000000f, -1.000000f,
        0.000000f, 0.707107f, -0.707107f,
        0.500000f, 0.000000f, -0.866025f,

        // 三角形56
        0.500000f, 0.000000f, -0.866025f,
        0.000000f, 0.707107f, -0.707107f,
        0.353553f, 0.707107f, -0.612372f,

        // 三角形57
        0.500000f, 0.000000f, -0.866025f,
        0.353553f, 0.707107f, -0.612372f,
        0.866025f, 0.000000f, -0.500000f,

        // 三角形58
        0.866025f, 0.000000f, -0.500000f,
        0.353553f, 0.707107f, -0.612372f,
        0.612372f, 0.707107f, -0.353553f,

        // 三角形59
        0.866025f, 0.000000f, -0.500000f,
        0.612372f, 0.707107f, -0.353553f,
        1.000000f, 0.000000f, 0.000000f,

        // 三角形60
        1.000000f, 0.000000f, 0.000000f,
        0.612372f, 0.707107f, -0.353553f,
        0.707107f, 0.707107f, 0.000000f,

        // ==================== 北极闭合（12个三角形，36顶点，总顶点144）====================
        // 三角形61
        0.000000f, 1.000000f, 0.000000f,
        0.000000f, 0.707107f, 0.707107f,
        0.353553f, 0.707107f, 0.612372f,

        0.000000f, 1.000000f, 0.000000f,
        0.353553f, 0.707107f, 0.612372f,
        0.612372f, 0.707107f, 0.353553f,

        0.000000f, 1.000000f, 0.000000f,
        0.612372f, 0.707107f, 0.353553f,
        0.707107f, 0.707107f, 0.000000f,

        0.000000f, 1.000000f, 0.000000f,
        0.707107f, 0.707107f, 0.000000f,
        0.612372f, 0.707107f, -0.353553f,

        0.000000f, 1.000000f, 0.000000f,
        0.612372f, 0.707107f, -0.353553f,
        0.353553f, 0.707107f, -0.612372f,

        0.000000f, 1.000000f, 0.000000f,
        0.353553f, 0.707107f, -0.612372f,
        0.000000f, 0.707107f, -0.707107f,

        0.000000f, 1.000000f, 0.000000f,
        0.000000f, 0.707107f, -0.707107f,
        -0.353553f, 0.707107f, -0.612372f,

        0.000000f, 1.000000f, 0.000000f,
        -0.353553f, 0.707107f, -0.612372f,
        -0.612372f, 0.707107f, -0.353553f,

        0.000000f, 1.000000f, 0.000000f,
        -0.612372f, 0.707107f, -0.353553f,
        -0.707107f, 0.707107f, 0.000000f,

        0.000000f, 1.000000f, 0.000000f,
        -0.707107f, 0.707107f, 0.000000f,
        -0.612372f, 0.707107f, 0.353553f,

        0.000000f, 1.000000f, 0.000000f,
        -0.612372f, 0.707107f, 0.353553f,
        -0.353553f, 0.707107f, 0.612372f,

        0.000000f, 1.000000f, 0.000000f,
        -0.353553f, 0.707107f, 0.612372f,
        0.000000f, 0.707107f, 0.707107f};
}
#endif