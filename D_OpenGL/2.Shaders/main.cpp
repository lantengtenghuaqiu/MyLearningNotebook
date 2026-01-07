#include<stdio.h>
#include<string>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

// #define printwrong printf("Some thing wrong %d",__LINE__);


int main(){
    //初始化glfw
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE , GL_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED , GL_FALSE);
    glfwWindowHint(GLFW_DECORATED , GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE , GL_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
    #endif
    const int width = 1920;
    const int height = 720;
    GLFWwindow * window = glfwCreateWindow(width,height,"Shaders",nullptr , nullptr);

    if(window == NULL){
        exit(0);
    }

    glfwMakeContextCurrent(window);
    
    //Init glad
    int success;
    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if(success == 0){
        exit(0);
    }

    glClearColor(0.2f,0.2f,0.15f,1.0f);
    glClearDepth(0.0f);
    glClearStencil(0.0f);


    while(!glfwWindowShouldClose()){
        glClear();
        glfwPollEvents();


        glfwSwapBuffers(window);
    }

    return 0;
}