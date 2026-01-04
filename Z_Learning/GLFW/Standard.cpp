#include<stdio.h>
#include<glfw3.h>


void key_callback(GLFWwindow * window , int key , int scancode , int action , int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}


int main()
{
    if(!glfwInit())
    {
        printf("%s" , "wrong");
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API , GLFW_NO_API);


    GLFWwindow * window = glfwCreateWindow(1280,720,"Learning GLFW",nullptr ,nullptr);

    if(window == NULL)
    {
        printf("%s" , "wrong");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    glfwSetKeyCallback(window , key_callback);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}