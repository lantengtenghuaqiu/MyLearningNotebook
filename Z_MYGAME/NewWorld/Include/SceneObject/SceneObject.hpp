#ifndef SCENEOBJECT
#define SCENEOBJECT

#include <variant>
#include <unordered_map>

#include "../BasicIncludes.hpp"
#include "../XMath.hpp"
#include "../Transformation.hpp"
#include "./Components.hpp"

struct SceneObject : public TransformComponent
{
private:
    std::unordered_map<int, ComponentTypes> components;

public:
    void AddComponent(COMPONENT compnent)
    {
        if (compnent & COMPONENT::Transform)
        {
            this->components.emplace(COMPONENT::Transform, TransformComponent());
        }
        if (compnent & COMPONENT::MeshRender)
        {
            this->components.emplace(COMPONENT::Transform, TransformComponent());
        }
        if (compnent & COMPONENT::Image)
        {
            this->components.emplace(COMPONENT::Transform, TransformComponent());
        }
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

        this->viewSpace[0] = (this->Right).x;
        this->viewSpace[1] = (this->Up).x;
        this->viewSpace[2] = (this->Forward).x;

        this->viewSpace[4] = (this->Right).y;
        this->viewSpace[5] = (this->Up).y;
        this->viewSpace[6] = (this->Forward).y;

        this->viewSpace[8] = (this->Right).z;
        this->viewSpace[9] = (this->Up).z;
        this->viewSpace[10] = (this->Forward).z;
    }

    void UpdateCameraProjectionMatrix(int Type)
    {
        // {
        //     1.0f / (this->aspect * tan_fov)  ,   0.0f,               0.0f,                                               0.0f,
        //     0.0f,                                1.0f / (tan_fov),   0.0f,                                               0.0f,
        //     0.0f,                                0.0f,               (this->n + this->f) / (this->n - this->f),          -1.0f,
        //     0.0f,                                0.0f,               (2.0f * this->n * this->f) / (this->n - this->f),   0.0f
        // };
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

void SetMainCamera(Camera *&camera, const float &w, const float &h, int &frameW, int &frameH)
{
    camera->SetPosition(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    camera->fov = 30.0f;
    camera->w = w;
    camera->h = h;
    camera->n = 0.1f;
    camera->f = 300.0f;
    camera->aspect = (float)frameW / frameH;
    camera->UpdateCameraSpaceMatrix();
    camera->UpdateCameraProjectionMatrix(Persp);
}
// ----------------------------------------------------------------------------

// Light Scene Objects---------------------------------------------------------
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
// ----------------------------------------------------------------------------

// Container for contain and get the scene object easly.
using VariantSceneObject = std::variant<SceneObject, DirectionalLight, PointLight, Camera>;

typedef class SCENEOBJECTROOT
{
public:
    std::unordered_map<std::string, VariantSceneObject> Root;

} HIERARCHY;

#endif