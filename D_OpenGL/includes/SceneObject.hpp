#ifndef SCENEOBJECT
#define SCENEOBJECT
#include "Data.hpp"
#include "../includes/BasicIncludes.hpp"
#include <iostream>
#include <variant>
#include <unordered_map>

// Components part
struct TransformAttribute
{
    /* data */
private:
    // float scaleX = 0.0f;
    // float scaleY = 0.0f;
    // float scaleZ = 0.0f;

    // float rotationX = 0.0f;
    // float rotationY = 0.0f;
    // float rotationZ = 0.0f;

    // float translatX = 0.0f;
    // float translatY = 0.0f;
    // float translatZ = 0.0f;

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

    Vec4 GetScale()
    {
        return this->scale;
    }
    void SetScale(Vec4 scl)
    {
        this->scale = scl;
    }
    void SetScaleX(float x) { this->scale.x += x; }
    void SetScaleY(float y) { this->scale.x += y; }
    void SetScaleZ(float z) { this->scale.x += z; }



    Vec4 GetRotation()
    {
        return this->rotation;
    }
    void SetRotation(Vec4 rot)
    {
        this->rotation = rot;
    }
    void SetRotationX(float x) { this->rotation.x += x; }
    void SetRotationY(float y) { this->rotation.x += y; }
    void SetRotationZ(float z) { this->rotation.x += z; }



    Vec4 GetPosition() const
    {
        return this->position;
    }
    void SetPosition(Vec4 pos)
    {
        this->position = pos;
        // this->translatX = pos.x;
        // this->translatY = pos.y;
        // this->translatZ = pos.z;
    }
    void SetPositionX(float x) { this->position.x += x; }
    void SetPositionY(float y) { this->position.x += y; }
    void SetPositionZ(float z) { this->position.x += z; }
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

struct Camera : public SceneObject
{
    float fov = 0.01f;
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

    mat4 rotationMatrix;

    float CameraSpace[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    float CameraProjection[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    Camera()
    {
        this->Forward.Set(0.0f, 0.0f, -1.0f, 0.0f);
        this->Up.Set(0.0f, 1.0f, 0.0f, 0.0f);
        this->Right.Set(1.0f, 0.0f, 0.0f, 0.0f);
        float rotationMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
        this->rotationMatrix = mat4(rotationMatrix);
    }

    Camera(float fov, float w, float h, float n, float aspect)
    {
        this->aspect = aspect;
        this->f = fov;
        this->n = n;
        this->h = h;
        this->w = w;
        this->fov = fov;
    }

    void UpdateCameraSpaceMatrix()
    {
        this->CameraSpace[0] = (this->Right).x;
        this->CameraSpace[1] = (this->Right).y;
        this->CameraSpace[2] = (this->Right).z;

        this->CameraSpace[4] = (this->Up).x;
        this->CameraSpace[5] = (this->Up).y;
        this->CameraSpace[6] = (this->Up).z;

        this->CameraSpace[8] = (this->Forward).x;
        this->CameraSpace[9] = (this->Forward).y;
        this->CameraSpace[10] = (this->Forward).z;
    }

    void UpdateCameraProjectionMatrix(int Type)
    {
        if (Type == Ortho)
        {
            this->CameraProjection[0] = 2.0f / (this->w);

            this->CameraProjection[5] = 2.0f / (this->h);

            this->CameraProjection[10] = 2.0f / (this->f - this->n);

            this->CameraProjection[14] = (this->f + this->n) / (this->f - this->n);
        }
        else
        {
            float tan_fov = tan(this->fov / 2.0f);
            this->CameraProjection[0] = 1.0f / (this->aspect * tan_fov);

            this->CameraProjection[5] = 1.0f / (tan_fov);

            this->CameraProjection[10] = (this->n + this->f) / (this->n - this->f);

            this->CameraProjection[11] = -1.0f;

            this->CameraProjection[14] = (2.0f * this->n * this->f) / (this->n - this->f);

            this->CameraProjection[15] = 0.0f;
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
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f};
    float uv[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f};
} // namespace Plane

#endif