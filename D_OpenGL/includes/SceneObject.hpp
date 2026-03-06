#ifndef SCENEOBJECT
#define SCENEOBJECT
#include "Data.hpp"
#include "../includes/BasicIncludes.hpp"
#include "./Transformation.hpp"
#include <iostream>
#include <variant>
#include <unordered_map>

// Components part
struct TransformAttribute
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
    void UpdataModelMatrix()
    {
        this->Transform = this->Translate * this->Rotation * this->Scale;
#ifdef TRANSFORM_DEBUG
        for (int i = 0; i < 16; i++)
        {
            printf("%f ", this->Transform._mat4[i]);
            if ((i + 1) % 4 == 0)
            {
                printf("\n");
            }
        }
        printf("\n\n");
#endif
    }

    TransformAttribute()
    {
        this->scale = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        this->rotation = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        this->position = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    TransformAttribute(Vec4 pos, Vec4 rot, Vec4 scl)
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

    void SetScale(Vec4 scl) { this->scale = scl; }
    void SetScaleX(float x) { this->scale.x += x; }
    void SetScaleY(float y) { this->scale.x += y; }
    void SetScaleZ(float z) { this->scale.x += z; }

    // Rotation
    Vec4 GetRotation() { return this->rotation; }
    float GetRotationX() const { return this->rotation.x; }
    float GetRotationY() const { return this->rotation.y; }
    float GetRotationZ() const { return this->rotation.z; }

    void SetRotation(Vec4 rot) { this->rotation = rot; }
    void SetRotationX(float x) { this->rotation.x += x; }
    void SetRotationY(float y) { this->rotation.y += y; }
    void SetRotationZ(float z) { this->rotation.z += z; }

    // Position
    Vec4 GetPosition() const { return this->position; }
    float GetPositionX() const { return this->position.x; }
    float GetPositionY() const { return this->position.y; }
    float GetPositionZ() const { return this->position.z; }

    void SetPosition(Vec4 pos) { this->position = pos; }
    void SetPositionX(float x) { this->position.x += x; }
    void SetPositionY(float y) { this->position.y += y; }
    void SetPositionZ(float z) { this->position.z += z; }
};

struct ColorComponent
{
    union
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        float rgba[4];
    };

    ColorComponent &operator=(const Vec4 color)
    {
        this->r = color.x;
        this->g = color.y;
        this->b = color.z;
        this->a = color.w;

        return *this;
    }
};
struct ImageComponent
{
    union
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
};
using ComponentTypes = std::variant<ColorComponent>;

struct SceneObject : public TransformAttribute
{
    std::unordered_map<std::string, ComponentTypes> components;
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
};

struct Light : public SceneObject
{
    /* data */
    Vec4 Color;
    float Intensity = 0;
    Vec4 Direction;
    Light()
    {
        this->Color = Vec4(1.0, 1.0, 1.0, 1.0);
        this->Intensity = 1.0f;
        this->Direction = Vec4(1.0, 0.0, 0.0, 0.0);
    }
    Light(Vec4 color, float instensity, Vec4 dir)
    {
        this->Color = color * instensity;
        this->Intensity = instensity;
        this->Direction = dir;
    }

    Vec4 GetColorMultipliedByInstensity()
    {
        return this->Color * this->Intensity;
    }
    Vec4 GetColor()
    {
        return this->Color;
    }
};

using VariantSceneObject = std::variant<SceneObject, Light, Camera>;

typedef class SceneObjectRoot
{
public:
    std::unordered_map<std::string, VariantSceneObject> sceneObjects;

} Hierarchy;

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

#endif