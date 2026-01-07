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
    0.5,0.5,0.0,
    2.0,0.5,0.0
};
float vertices2[]={
    -1.0,-1.0,0.0,
    0.5,1.0,0.0,
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

    GLint success;



    //创建数据缓冲区
    //用于记录
    unsigned int vertex_arribute_ids[2];
    glGenVertexArrays(2,vertex_arribute_ids);



    
    //1.创建vertex缓冲区
    unsigned int vertex_buffer_ids[2];
    glGenBuffers(2,vertex_buffer_ids);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_ids[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindVertexArray(vertex_arribute_ids[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    /**
     *  逻辑和底层原理:
     *      告诉GPU如何从当前绑定的VBO中,读取数据,并传递给着色器程序绑定的vertex中的值.
     * 
     *  参数一:这个跟Vertex Shader中的location index有关,这里的0表示对应的是location 0
     *  参数二:对应Location index的类型,如vec 则对应数量为3,如果是vec4 则对应4
     *  参数三:对应location index中值类型,这里选择的是float类型
     *  参数四:对传入的location index数据是否归一化处理
     *  参数五:对应的location每组值的跨度,这里是3则对应012是一组,然后下一组的索引会从3开始数三个345.....
     *  参数六:指针,从哪里开始,这里0表示从第一个值开始.
     */
    glEnableVertexAttribArray(0);




    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_ids[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);

    glBindVertexArray(vertex_arribute_ids[1]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    /**
     *  逻辑和底层原理:
     *      告诉GPU如何从当前绑定的VBO中,读取数据,并传递给着色器程序绑定的vertex中的值.
     * 
     *  参数一:这个跟Vertex Shader中的location index有关,这里的0表示对应的是location 0
     *  参数二:对应Location index的类型,如vec 则对应数量为3,如果是vec4 则对应4
     *  参数三:对应location index中值类型,这里选择的是float类型
     *  参数四:对传入的location index数据是否归一化处理
     *  参数五:对应的location每组值的跨度,这里是3则对应012是一组,然后下一组的索引会从3开始数三个345.....
     *  参数六:指针,从哪里开始,这里0表示从第一个值开始.
     */
    glEnableVertexAttribArray(0);


    //2.加载vertex shader
    char * vertex_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\HelloOpenGL\\hello_opengl.vertex");
    //3.配置vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id,1,&vertex_shader,NULL);
    //4.编译vertex shader
    glCompileShader(vertex_shader_id);  //glCompileShader()是将ID对应的着色器源码在cpu端使用GPU驱动程序编译成GPU能看懂的二进制指令(中间码/机器码)

    glGetShaderiv(vertex_shader_id,GL_COMPILE_STATUS,&success);
    if(!success){
        printwrong;
    }

    //5.加载fragment shader
    char * fragment_shader = ReadFile("G:\\user\\desktop\\C++\\GraphicLearning\\D_OpenGL\\HelloOpenGL\\hello_opengl.fragment");
    //6.加载fragment shader
    //因为前面已经创建了vertex shader,所以fragmen 的 id不是1而是2
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id , 1 ,&fragment_shader,NULL);
    //7.加载fragment shader
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id,GL_COMPILE_STATUS,&success);
    if(!success){
        printwrong;
    }

    //8.链接所有shader
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id , vertex_shader_id);
    glAttachShader(shader_program_id , fragment_shader_id);
    glLinkProgram(shader_program_id);
    
    glGetProgramiv(shader_program_id,GL_LINK_STATUS,&success);
    if(!success){
        printwrong;
    }

    //9.因为编译并链接完成,则vertex shader和fragment shader都不需要了,则需要删除.
    delete(vertex_shader);
    delete(fragment_shader);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    glUseProgram(shader_program_id);



    glfwSetKeyCallback(window , callback_keyboard);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClearDepth(0.0f);
    glClearStencil(0.0f);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader_program_id);
        glBindVertexArray(vertex_arribute_ids[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glfwSwapBuffers(window);//如果他在绘制三角形前面会导致,当glClear()完成后,背景内容会被Swap,但是还没绘制,等之后绘制之后,没有Swap就会导致绘制的图形不显示.
        // EscapeWindow();
        glfwPollEvents();
    }
    //销毁窗口
    glfwDestroyWindow(window);

    //销毁GLFW库,释放内存.
    glfwTerminate();

    return 0;
}