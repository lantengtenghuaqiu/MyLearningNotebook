#ifndef XYLEVENT
#define XYLEVENT

#include "Window.hpp"
#include "Transformation.hpp"
#include "SceneObject.hpp"

void CameraRotate(GLFWwindow *window, Camera &camera, float speed)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        // camera.Position.y -= speed;
        camera.rotationY += speed;
        Transform::Rotation(0.0f,camera.rotationY,0.0f,camera.rotationMatrix._mat4);
        camera.Right = camera.rotationMatrix * camera.Right;
        camera.Up = camera.rotationMatrix * camera.Up;
        camera.Forward = camera.rotationMatrix * camera.Forward;

    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.Position.y += speed;
    }
}

void CameraTranslate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat, int location)
{

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        entity.translatX -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        entity.translatX += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        entity.translatY -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        entity.translatY += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        entity.translatZ += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        entity.translatZ -= speed;
    }

    Transform::Translate(entity.translatX, entity.translatY, entity.translatZ, mat);
    glUniformMatrix4fv(location, 1, GL_TRUE, mat);
}

void KeyRotate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat, int location)
{
    

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        entity.rotationY += speed;
        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        entity.rotationY -= speed;

        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        entity.rotationX += speed;
        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        entity.rotationX -= speed;

        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        entity.rotationZ += speed;
        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        entity.rotationZ -= speed;

        Transform::Rotation(entity.rotationX, entity.rotationY, entity.rotationZ, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }
}

void KeyTranslate(GLFWwindow *window, TransformAttribute &entity, float speed, float *mat, int location)
{

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        entity.translatX += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        entity.translatX -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        entity.translatY += speed;
    }
    #ifdef __APPLE__
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        entity.translatY -= speed;
    }
    #else
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        entity.translatY -= speed;
    }
    #endif
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        entity.translatZ += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        entity.translatZ -= speed;
    }

    Transform::Translate(entity.translatX, entity.translatY, entity.translatZ, mat);
    glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}
#endif