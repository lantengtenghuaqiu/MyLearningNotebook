#ifndef SCENEOBJECT
#define SCENEOBJECT
#include "Data.hpp"
typedef struct SceneObjectEntity
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

} SceneObject;

struct Camera : public SceneObject
{
    /* data */
    Vec4 Forward;
    Vec4 Up;
    Vec4 Right;
    Camera(){
        this->Forward.Set(0.0f,0.0f,-1.0f,0.0f);
        this->Up.Set(0.0f,1.0f,0.0f,0.0f);
        this->Right.Set(1.0f,0.0f,0.0f,0.0f);
    }
};

struct SceneRoot
{
    SceneObjectEntity *scene_objects;

    SceneRoot(const int objects)
    {
        this->scene_objects = new SceneObject[objects];
    }
};

#endif