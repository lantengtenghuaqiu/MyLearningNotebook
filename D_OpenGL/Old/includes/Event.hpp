#ifndef XYLEVENT
#define XYLEVENT

#include "Window.hpp"
#include "Transformation.hpp"
#include "SceneObject.hpp"

void CameraRotate(GLFWwindow *window, SceneObject &entity, float speed, float *mat, int location)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        entity.rotationY += speed;
        Transform::Rotation(0.0f, entity.rotationY, 0.0f, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        entity.rotationY -= speed;

        Transform::Rotation(0.0f, entity.rotationY, 0.0f, mat);
        glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }
}
void CameraTranslate(GLFWwindow *window, SceneObject &entity, float speed, float *mat, int location)
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

void KeyRotate(GLFWwindow *window, SceneObject &entity, float speed, float *mat, int location)
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

void KeyTranslate(GLFWwindow *window, SceneObject &entity, float speed, float *mat, int location)
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
        entity.translatY -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        entity.translatY += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        entity.translatZ -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        entity.translatZ += speed;
    }

    Transform::Translate(entity.translatX, entity.translatY, entity.translatZ, mat);
    glUniformMatrix4fv(location, 1, GL_TRUE, mat);
}
#endif