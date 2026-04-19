#ifndef XYLEVENT
#define XYLEVENT

#include "Window.hpp"
#include "Transformation.hpp"
#include "SceneObject.hpp"

void GlobalCameraRotate(GLFWwindow *window, Camera *camera, float speed)
{
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera->SetRotationY(-speed);
        Transform::Rotation(camera->GetRotation(), camera->RotationMatrix);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera->SetRotationY(speed);
        Transform::Rotation(camera->GetRotation(), camera->RotationMatrix);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        camera->SetRotationX(-speed);
        Transform::Rotation(camera->GetRotation(), camera->RotationMatrix);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        camera->SetRotationX(speed);
        Transform::Rotation(camera->GetRotation(), camera->RotationMatrix);
    }
}

void GlobalCameraTranslate(GLFWwindow *window, Camera *camera, float speed)
{

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->SetPositionX(-speed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->SetPositionX(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera->SetPositionY(-speed);
    }
#ifdef __APPLE__
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera->SetPositionY(speed);
    }
#else
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera->SetPositionY(-speed);
    }
#endif
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->SetPositionZ(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->SetPositionZ(-speed);
    }

    Transform::Translate(camera->GetPosition(), camera->TranslateMatrix);
}



void KeyRotate(GLFWwindow *window, Transform &entity, float speed, float *mat)
{

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        entity.SetRotationY(speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        entity.SetRotationY(-speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        entity.SetRotationX(speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        entity.SetRotationX(-speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        entity.SetRotationZ(speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        entity.SetRotationZ(-speed);
        Transform::Rotation(entity.GetRotation(), mat);
    }
}

void KeyTranslate(GLFWwindow *window, Transform &entity, float speed, float *mat)
{

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        entity.SetPositionX(speed);
        Transform::TranslateX(entity.GetPositionX(), mat);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        entity.SetPositionX(-speed);
        Transform::TranslateX(entity.GetPositionX(), mat);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        entity.SetPositionY(speed);
        Transform::TranslateY(entity.GetPositionY(), mat);
    }
#ifdef __APPLE__
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        entity.SetPositionY(-speed);
        Transform::TranslateY(entity.GetPositionY(), mat);
    }
#else
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        entity.SetPositionY(-speed);
        Transform::TranslateY(entity.GetPositionY(), mat);
    }
#endif
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        entity.SetPositionZ(speed);
        Transform::TranslateZ(entity.GetPositionZ(), mat);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        entity.SetPositionZ(-speed);
        Transform::TranslateZ(entity.GetPositionZ(), mat);
    }
}
#endif