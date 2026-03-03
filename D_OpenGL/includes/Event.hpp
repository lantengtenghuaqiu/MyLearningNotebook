#ifndef XYLEVENT
#define XYLEVENT

#include "Window.hpp"
#include "Transformation.hpp"
#include "SceneObject.hpp"

void CameraRotate(GLFWwindow *window, Camera &camera, float speed)
{
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.SetRotationY(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.SetRotationY(-speed);
    }
}

void CameraTranslate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat, int location)
{

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        entity.SetPositionX(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        entity.SetPositionX(-speed);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        entity.SetPositionY(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        entity.SetPositionY(-speed);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        entity.SetPositionZ(speed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        entity.SetPositionZ(-speed);
    }

    Transform::Translate(entity.GetPosition(), mat);
}

void KeyRotate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat)
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

void KeyTranslate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat)
{

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        entity.SetPositionX(speed);
        Transform::TranslateX(entity.GetPosition().x,mat);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        entity.SetPositionX(-speed);
        Transform::TranslateX(entity.GetPosition().x,mat);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        entity.SetPositionY(speed);
        Transform::TranslateY(entity.GetPosition().y,mat);
    }
#ifdef __APPLE__
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        entity.SetPositionY();
        Transform::TranslateY(entity.GetPosition().y,mat);
    }
#else
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        entity.SetPositionY(-speed);
        Transform::TranslateY(entity.GetPosition().y,mat);
    }
#endif
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        entity.SetPositionZ(speed);
        Transform::TranslateZ(entity.GetPosition().z,mat);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        entity.SetPositionZ(-speed);
        Transform::TranslateZ(entity.GetPosition().z,mat);
    }

    // Transform::Translate(entity.GetPosition(), mat);
    // glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}
#endif