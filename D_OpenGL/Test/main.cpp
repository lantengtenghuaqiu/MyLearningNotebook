#include"../include/glad/glad.h"
#include"../include/GLFW/glfw3.h"
#include<iostream>

        
#define printwrong std::cerr<<__LINE__<<" Wrong"<<std::endl


void InitWindow(){
    if(glfwInit() != GL_TRUE){
        printwrong;
        exit(0);
    }

    //glfwWindowHint()必须在glfwCreateWindow()前调用
    glfwWindowHint(GLFW_RESIZABLE , GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE , GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED , GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED , GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED , GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);
    #endif

}

void callback_keyboard(GLFWwindow * window , int key , int scancode , int action , int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window , GLFW_TRUE);
    }

}


void InitGlad(GLint width , GLint height){
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printwrong;
        exit(0);
    }

    glViewport(0,0,width,height);
    

}

float vertices[]={
    -1.0,-2.0,0.0,
    -2.0,0.5,0.0,
    2.0,0.5,0.0
};

void SetVertexArrayBuffer(int size , unsigned int bufferID, void * data,GLenum darw_type){
    glGenBuffers(size,&bufferID);
    glBindBuffer(GL_ARRAY_BUFFER,bufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,darw_type);
    
}


int main() {
    InitWindow();

    const uint16_t width = 1920;
    const uint16_t height = 1080;

    GLFWwindow * window = glfwCreateWindow(width,height,"Hello OpenGL",nullptr,nullptr);

    if(window ==NULL){
        printwrong;
        glfwTerminate();
        exit(0);
    }

    glfwMakeContextCurrent(window);

    InitGlad(width,height);

    unsigned int VBO1;
    SetVertexArrayBuffer(1,VBO1,vertices,GL_STATIC_DRAW);

    glfwSetKeyCallback(window , callback_keyboard);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClearDepth(0.0f);
    glClearStencil(0.0f);
    
    while(!glfwWindowShouldClose(window)){
        
        glClear(GL_COLOR_BUFFER_BIT);
        



        glfwSwapBuffers(window);
        // EscapeWindow();
        glfwPollEvents();
        
    }
    //销毁窗口
    glfwDestroyWindow(window);

    //销毁GLFW库,释放内存.
    glfwTerminate();

    return 0;
}