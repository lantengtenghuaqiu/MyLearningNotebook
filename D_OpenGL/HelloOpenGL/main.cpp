#include"../include/glad/glad.h"
#include"../include/GLFW/glfw3.h"

#include<cstdio>
#include<cstring>

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

// void SetVertexArrayBuffer(int size , unsigned int &bufferID, void * data,GLenum darw_type){
//     glGenBuffers(size,&bufferID);
//     glBindBuffer(GL_ARRAY_BUFFER,bufferID);
//     glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,darw_type);
    
// }

char * ReadFile(const char* path)
{
    long file_size = -1;
    FILE * file = fopen(path,"rb");

    if(file == nullptr){
        printwrong;
        fclose(file);
        return nullptr;
    }

    fseek(file , 0 , SEEK_END);
    file_size = ftell(file);
    if(file_size < 0){
        printwrong;
        fclose(file);
        return nullptr;

    }

    fseek(file , 0 , SEEK_SET);

    char* contents = new char[file_size+1];
    if(contents == nullptr){
        printwrong;
        fclose(file);
        return nullptr;

    }

    memset(contents,0,file_size+1);

    fread(contents,1,file_size,file);

    contents[file_size]='\0';
    fclose(file);
    
    return contents;

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

    //创建数据缓冲区
    //1.创建vertex缓冲区
    unsigned int vertex_buffer_id_1;
    glGenBuffers(1,&vertex_buffer_id_1);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id_1);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //2.加载vertex shader
    char * vertex_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\HelloOpenGL\\hello_opengl.vertex");
    //3.配置vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id,1,&vertex_shader,NULL);
    //4.编译vertex shader
    glCompileShader(vertex_shader_id);  //glCompileShader()是将ID对应的着色器源码在cpu端使用GPU驱动程序编译成GPU能看懂的二进制指令(中间码/机器码)


    //5.加载fragment shader
    char * fragment_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\HelloOpenGL\\hello_opengl.fragment");
    //6.加载fragment shader
    //因为前面已经创建了vertex shader,所以fragmen 的 id不是1而是2
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id , 1 ,&fragment_shader,NULL);
    //7.加载fragment shader
    glCompileShader(fragment_shader_id);


    //8.链接所有shader
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id , vertex_buffer_id_1);
    glAttachShader(shader_program_id , fragment_shader_id);
    glLinkProgram(shader_program_id);
    
    //9.因为编译并链接完成,则vertex shader和fragment shader都不需要了,则需要删除.
    delete(vertex_shader);
    delete(fragment_shader);
    glDeleteShader(vertex_buffer_id_1);
    glDeleteShader(fragment_shader_id);

    glUseProgram(shader_program_id);


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