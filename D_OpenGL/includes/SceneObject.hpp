#ifndef SCENEOBJECT
#define SCENEOBJECT
#include "Data.hpp"
#include <iostream>
#include <variant>
#include <unordered_map>

struct TransformAttribute
{
    /* data */
    float scaleX = 0.0f;
    float scaleY = 0.0f;
    float scaleZ = 0.0f;

    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;

    float translatX = 0.0f;
    float translatY = 0.0f;
    float translatZ = 0.0f;

    mutable Vec4 Position;

    mat4 Scale;
    mat4 Rotation;
    mat4 Translate;

    TransformAttribute()
    {
        this->Position = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    TransformAttribute(Vec4 pos)
    {
        this->Position = pos;
    }
    void SetPosition(Vec4 position)
    {
        this->Position = position;
        this->translatX = position.x;
        this->translatY = position.y;
        this->translatZ = position.z;
    }
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
    Vec4 Forward;
    Vec4 Up;
    Vec4 Right;
    mat4 rotationMatrix;

    mat4 viewMatrix;

    float cameraspacematrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    // float Projection[][16] = {
    //     {
    //         //@
    //         2.0f / this->w, 0.0f, 0.0f, 0.0f,
    //         0.0f, 2.0f / this->h, 0.0f, 0.0f,
    //         0.0f, 0.0f, 2.0f / (this->f - this->n), 0.0f,
    //         0.0f, 0.0f, (this->f + this->n) / (this->f - this->n), 1.0f
    //         //@
    //     },
    //     {
    //         //@
    //         1.0f / (this->aspect * tan(this->fov / 2.0f)), 0.0f, 0.0f, 0.0f,
    //         0.0f, 1.0f / tan(this->fov / 2.0f), 0.0f, 0.0f,
    //         0.0f, 0.0f, (this->n + this->f) / (this->n - this->f), -1.0f,
    //         0.0f, 0.0f, (2.0f * this->n * this->f) / (this->n - this->f), 0.0f
    //         //@
    //     }};

    Camera()
    {
        this->Forward.Set(0.0f, 0.0f, -1.0f, 0.0f);
        this->Up.Set(0.0f, -1.0f, 0.0f, 0.0f);
        this->Right.Set(-1.0f, 0.0f, 0.0f, 0.0f);
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
        this->f = f;
        this->n = n;
        this->h = h;
        this->w = w;
        this->fov = fov;
    }

    float *ViewMatrix()
    {
        this->cameraspacematrix[0] = (this->Right).x;
        this->cameraspacematrix[1] = (this->Right).y;
        this->cameraspacematrix[2] = (this->Right).z;

        this->cameraspacematrix[4] = (this->Up).x;
        this->cameraspacematrix[5] = (this->Up).y;
        this->cameraspacematrix[6] = (this->Up).z;

        this->cameraspacematrix[8] = (this->Forward).x;
        this->cameraspacematrix[9] = (this->Forward).y;
        this->cameraspacematrix[10] = (this->Forward).z;
        return this->cameraspacematrix;
    }
};

struct Light : public SceneObject
{
    /* data */
    Vec4 Color;
    float Instensity = 0;
    Vec4 Direction;
    Light()
    {
        this->Color = Vec4(1.0, 1.0, 1.0, 1.0);
        this->Instensity = 1.0f;
        this->Direction = Vec4(1.0, 0.0, 0.0, 0.0);
    }
    Light(Vec4 color, float instensity, Vec4 dir)
    {
        this->Color = color * instensity;
        this->Instensity = instensity;
        this->Direction = dir;
    }

    Vec4 GetColorMultipliedByInstensity()
    {
        return this->Color * this->Instensity;
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

namespace Global
{
    typedef struct Textures
    {
        int TextureWidth;
        int TextureHeight;
        unsigned char *TextureData;

    } Textures;

}

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
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f};
    float uv[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f};
} // namespace Plane

#endif