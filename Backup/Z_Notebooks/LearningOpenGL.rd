<<OpenGL>>
    ->#单词表
        requisite 形容某一个东西必要的比如必要的纸质文件:requisite paperwork;Engine development knowledge is requisite;
        deprecate 表示对某个东西抨击,或者反对.I deprecated using AI to create games;
        deprecated function 弃用的函数
        immediate 表示立刻的,一种十分短的时间出现的.
        specific 表示形容某个东西是具体的确切的
        isolate 与世隔绝的;独立的;使..独立;独立

    ->#编译架构:
        首先,需要准备环境:
            glfw代码库找到对应代码:https://github.com/glfw/glfw
                Windows:
                    glfw-3.4.bin.WIN64.zip
                    glfw-3.4.bin.WIN64.zip
                    包含了库文件和头文件:
                        glfw-3.4.bin.WIN64中有关于MinGW文件下的.dll .a dll.a的库文件
            glad官网生成代码:https://glad.dav1d.de/

            将代码包含在项目中,以下是包结构:
                OpenGL:
                    include:
                        glad:
                            glad.h
                        KHR:
                            khrplatform.h
                        GLFW:
                            glfw3.h
                            glfw3native.h
                    lib:
                        glfw3.dll
                        libglfw3.a
                        libglfw3dll.a
                    src:
                        glad.c
                    project:
                        main.cpp

                Terminal终端编译:
                    ....OpenGL\project> g++ main.cpp ..\src\glad.c -I ../include -L ../lib -lopengl32 -lgdi32 -lglfw3 -std=c++17 -fno-permissive -Wall -Wextra -o main.exe
    ->#什么是GLAD:
        首先明确一点OpenGL并没有具体的代码和代码实现,它只是一种规范,OpenGL本身没有对应的具体函数实现,可以根据规范进行底层驱动.
        而GLAD则是根据规范开发的c++第三方代码库.通过调用GLAD中封装好的函数可以驱动GPU.
        因此理论上,遵循OpenGL的规范,是可以写一套属于自己的代码.
        Vulkan:
            此外Vulkan与OpenGL同样,都是驱动规范,因此,Vulkan同样需要具体的函数实现.
            但是Vulkan规范不开源,函数实现是由Khronos维护.
        DirectX:
            而DirectX不是规范也不是单一的图形API,而是包含了一系列音频,输入,计算等API.
            其中Direct3D(D3D)是核心3D渲染,对标OpenGL和Vulkan.
            并且DirectX不开源,具体的函数API实现由微软维护

            Direct3D (D3D)	|DirectX 的 “核心中的核心”,负责 3D 图形渲染（顶点处理、光栅化、着色器、管线管理）	   对标 OpenGL/Vulkan，是图形渲染的核心
            DXGI	        |显示管理接口（交换链、显卡适配、多显示器、分辨率切换,是 D3D 与显卡 / 显示器的桥梁	   无直接对标（OpenGL 需手动结合窗口库如 GLFW 实现）
            DirectCompute	|通用计算接口（利用显卡 GPU 做非图形计算，如物理模拟、AI 推理）	                      对标 OpenGL Compute Shader、Vulkan 计算管线
            DirectSound	    |音频处理（3D 音效、混音、音频播放 / 捕获）	                                        对标 OpenAL
            DirectInput	    |输入设备管理（键鼠、手柄、摇杆、触摸）	                                            对标 GLFW/SDL 的输入模块
            DirectStorage	|高速存储读写（专为游戏优化，比如快速加载大型场景 / 资源）	                          无直接对标，微软独家优化



    ->#使用OpenGL:
        ->#创建GLFW:
            第一步
                需要准备一个应用窗口和OpenGL的上下文.
                glfwInit()创建GLFW库,必须与glfwTerminate()连用;必须在主线程中创建,保证线程安全.
                返回int值,如果成功返回GL_TRUE;
            
            第二步
                需要通过调用glfwWindowHint指定OpenGL版本和模式,GLFW会根据系统调用并创建对应系统的窗口.
                对于Windows会调用Windows下的窗口创建函数.并且可以修改一些参数以便于控制窗口;
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);                       指定OpenGL主版本
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);                       指定OpenGL的此版本
                    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);     指定OpenGL的兼容模式
                                                                                        GLFW_OPENGL_CORE_PROFILE:为核心模式,表示强制禁用3.0前的函数.
                                                                                        GLFW_OPENGL_COMPAT_PROFILE:兼容模式,表示保留老旧函数
                    GLFWwindow * window=glfwCreateWindow(width,height,"title",nullptr,nullptr);
                    glfwMakeContextCurrent(window);
            
            第三步
                创建主循环:
                    创建事件:
                    glfwSetKeyCallback(GLFWwindow * window , callback function);
                    while(!glfwWindowShouldClose(GLFWwindow*window)){
                        glfwPollEvents();
                        glfwSwapBuffers(window);//可选
                        
                    }
                    解析:
                        glfwSetKeyCallback(GLFWwindow * window , callback function);是设置键盘的回调函数.
                            callback function是自拟的,并且需要包含参数(GLFWwindow*window ,int key , int action ,int scancode , int mode);
                        
                        glfwWindowShouldClose(GLFWwindow * window)为true的时候关闭循环,一般通过事件系统设置.
                        glfwPollEvents();是轮询系统查询.根据当前的时间输入会传递一些值给GLFW中关于事件库的部分;
                        glfwSwapBuffers(window);是窗口双缓冲,   
                            原理是解决GPU渲染无法一次性渲染完的问题:
                                虽然GPU是多线程并行运算,但是每次运算使用的核心只能一次计算完一部分,并且因为复杂度不同或者屏幕的分辨率差别,会导致渲染断断续续地传给显示缓冲区中.
                                而且对于一个超大型渲染任务即便是使用全部核心也无法一次渲染完成.
                                但是显示器是逐帧显示的,因此会每帧读取前台显示缓冲区的内容,就会导致画面撕裂,读取到因为没有渲染完而未覆盖的部分.
                                因此,当GPU对一个项目进行计算时,渲染完成的内容不直接写入到前台缓冲区中,而是写入到后台显示缓冲区中.
                                当完全渲染完毕之后再将整个后台显示缓冲区的内容交给前台缓冲区,显示器在该帧读取前台缓冲区的内容,就会显示完整的渲染画面,而不是割裂的画面.

        
        ->#配置GLAD:
            第四步
                ...glfwMakeContextCurrent(...);
                if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
                    printwrong;
                    glViewport(..,..,..,..);
                    return -1;
                }

                1.创建VAO的索引Id:
                    什么是VAO?
                        VAO是Vertex Array Object的缩写,它的功能是记录当前通过glBufferData绑定在gpu中的顶点数据.
                        因为glBindBuffer一次只能绑定一个vertex数据,此外它不具有索引可以告诉gpu我要拿哪一个我绑定的顶点数据.
                        !!!注意:glVertexAttribPointer只是写VAO的规则,并不是存入数据,真正存入数据的是glBufferData,VAO只是从glBufferData中根据现在制定好的规则去读取数据.

                    unsigned int VAO[number];

                    glGenVertexArray(number,VAO);

                2.创建绑定Vertex数据的索引Id:
                    unsigned int VBO[number];

                    glGenBuffer(number,VBO);

                3.绑定第一个VBO和顶点数据:
                    glBindBuffer(GL_ARRAY_BUFFER , VBO[0]);
                        将GL_ARRAY_BUFFER类型的buffer,与VBO[0]这个索引Id绑定.
                    glBufferData(GL_ARRAY_BUFFER , sizeof(vertics_1),vertex_1_dates,Draw type);
                        将vertics_1的顶点数据,传递给GL_ARRAY_BUFFER类型的buffer中,并指定绘制类型,并传递给GPU,存储到显存中.
                            Draw type有三种形式,根据功能区分
                                GL_DYNAMIC_DRAW:是指一些数据平凡替换的数据,如角色动画,移动的物体的顶点数据等.在此进行标记,会存储到可读写的快速存储区;
                                GL_STATIC_DRAW:是指那些静态数据,比如不动的桌子椅子等.会存储到高速显存中;
                                GL_STREAM_DRAW:是一些存在时间较短,并且会销毁的数据,比如粒子系统.会存储到临时显存中;
                    
                glClearColor(0.2f,0.2f,0.2f,1.0f);
                glClearDepth(0.0f);
                glClearStencil(0.0f);

                while(...){
                glClear(...);
                ....;
                }

                解析:
                    glfwMakeContextCurrent(GLFWwindw*window);
                        用于绑定上下文,绑定哪一个GLFW创建的窗口.
                        因为可以创建很多,所以这个函数是确定哪一个的.
                        之后具体的"工具"功能函数比如Viewport()或者glClearColor()这种函数,就会对上下文绑定的窗口进行操作.
                    glfwGetProcAddress();是GLFW获取OpenGL/Vulkan 函数地址的接口.
                        详解:
                            首先OpenGL/Vulkan都只是规范,而各厂商比如NVIDIA会根据规范写一套驱动程序,驱动程序则包含对显卡进行具体操作函数的实现.
                            在驱动加载的时候,驱动动态库会编译,将函数被映射到内存中,并有相应的内存地址.
                            glfwGetProcAddress()是获取这些驱动的函数的内存地址.
                    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);会"截胡"GLFW获取到的驱动函数的地址,为己所用;
                    glViewport();就是获取到的驱动函数,只是用glad的函数名称而已

                    glClearColor(0.2f,0.2f,0.2f,1.0f);
                        是设置清屏后的颜色初始值,到上下文管理器中.只设置值,不进行具体操作
                    glClearDepth(float);
                        是设置清屏后的初始深度缓冲值,到上下文管理器中.只设置值,不进行具体操作
                    glClearStencil(1);
                        是设置清屏后的初始模板缓冲值,到上下文管理器中.只设置值,不进行具体操作
                    glClear(...)
                        是进行对上下文绑定的窗口的后台缓冲区的值,根据设置的glClearDepth和glClearStencil和glClearColor进行统一覆盖.需要放在while中
                        GL_COLOR_BUFFER_BIT     : 清空颜色缓冲区
                        GL_DEPTH_BUFFER_BIT     : 清空深度缓冲区
                        GL_STENCIL_BUFFER_BIT   : 清空模板缓冲区
        ->#处理Vertex数据:

        ->#Uniform 传入数据:
            传入float4:
                int _color = glGetUniformLocation(shaders.ShaderProgram, "_color");
                传入方式一:
                    glUniform4f(_color ,1.0f,1.0f,1.0f,1.0f);
                传入方式二:
                    传入单个float:
                        float _Color[]={0.0,1.0,1.0,1.0};
                        glUniform4fv(_color,1,_Color);
                            |shader:
                            |   uniform vec4 _color;
                    传入多个float:
                        float _Color[]={
                                        0.0,1.0,1.0,1.0,
                                        1.0,0.0,1.0,1.0
                                        };
                        glUniform4fv(_color,2,_Color);
                            |shader:
                            |   uniform vec4 _color[2];
                            |   ...
                            |   fragmentColor = _color[0];
            传入Matrix float4:
                先获取location:
                    int proj = glGetUniformLocation(shaders.ShaderProgram, "proj");
                传入:
                    float projMat[16] = {
                                        2.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 2.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, -2.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f};
                    glUniformMatrix4fv(proj, 1, GL_FALSE, projMat);
                        参数含义:
                            第一个参数,为location.
                            第二个参数,为矩阵个数,与4fv相同,可以传入多个,用[]表示.
                            第三个参数,为是否转置.
                            第四个参数,为矩阵数据.


    ->#理解OpenGL:
        首先,无论是对于3D items,还是2D items,都是在3D空间下的.而最终显示在屏幕上的都是2D空间下的.因此OpenGL流水线的绝大部份工作都是将3D空间信息通过变换,变成2D空间信息.
        变换总的来说,首先把3D空间下的数据变为2D空间下的数据,然后2D空间下的数据变成实际的颜色.
        根据Real time中的规范,首先准备一组3D Coordinates数据,将这组3D Coordinates Data即Input data进行数据处理.
        数据处理方式就是通过Input Assembler阶段将Input data进行打组,将顶点数据变成三角形数据在OpenGL中称Vertex Data.
        在OpenGL中不叫Input Assembler 而称为 Vertex attributes.

        这个阶段就是Vertex stage的第一部分;
        随后开始进行MVP变换.


        Real time rendering流水线:
            分为四大阶段:
                Application:
                    Vertex Data
                Geometry Processing:
                    Input Assembler Stage
                    Vertex Shading:
                        Vertex Shader
                        Tessellation Shader
                        Geometry Shader
                        Projection
                        Clipping
                        Screen Mapping

                Rasterization:
                    Triangle Setup
                    Triangle Traversal

                Pixel Processing:
                    Pixel Shading
                    Merging

        OpenGL的流水线:
            Vertex Data(顶点输入阶段)(对应应用阶段.包含数据处理阶段(对应Input Assembler阶段))

            Vertex Shader Stage(对应几何处理阶段的Vertex Stage对顶点进行变换)
                目的:
                    将vertex datas从3D coordinates变换到2D coordinates中
            Tessellation Shader(曲面细分阶段)
            Shape Assembly(图元装配阶段,在Real time rendering中没有讲这一阶段)
                目的:设置三角形的绘制方式(GL_POINTS , GL_TRIANGLES , GL_LINE_STRIP).
            Geometry Shader(几何着色器阶段)
            Rasterization(光栅化阶段,Real time rendering将这个阶段分为triangle setup和triangle travel(三角形遍历)两大阶段)
            Fragment Shader(Pixel Shader阶段,在Real time rendering中属于Pixel Processing阶段)
            Test And Blending(Merge阶段,在Real time rendering中属于Pixel Processing阶段)



        代码流水线:
            第一步:
                获取顶点数据:   
                                                            |顶点1|      |顶点2|       |顶点3|
                    如自定义一个三角形:float triangle[] = {0.5,1.0,0.0, 1.0,0.0,0.0, -1.0,0.0,0.0};
            第二步:
                进行Input Assembler阶段:
                    使用glGenBuffer(count , &VBO);生成buffer ID
                    传递给glBindBuffer(Data type , buffer ID)进行绑定缓冲区,设置顶点缓冲区.(这里还不是传递数据给GPU的,这里是告诉GPU申请的现存区域).
                    glBufferData(Data type , sizeof vertices data , vertices data , Draw type)是进行将顶点传递给绑定的缓冲区,进行数据存储(这里是将数据传递到显存中的阶段).
                        Draw type有三种形式,根据功能区分
                            GL_DYNAMIC_DRAW:是指一些数据平凡替换的数据,如角色动画,移动的物体的顶点数据等.在此进行标记,会存储到可读写的快速存储区;
                            GL_STATIC_DRAW:是指那些静态数据,比如不动的桌子椅子等.会存储到高速显存中;
                            GL_STREAM_DRAW:是一些存在时间较短,并且会销毁的数据,比如粒子系统.会存储到临时显存中;
                            
                        
                    LearnOpen中讲的VBO(vertex buffer objects)就是buffer ID.是一个int类型数据.

                这个阶段是将数据整合,并传递给GPU,存储到缓存中.(并不是Draw Call)
            Vertex Shader:
                进行变换的地方.
                首先进行


    ->#理解GLSL:
        glsl作为OpenGL的着色器语言,主要包含vertex shader和fragment shader.
        shader之间是isolated的,之间的交流只有output到input.开头都包含版本信息#version version_number core

        Vertex Shader:
            #version version_number core
            layerout (location = 0) in type vertex_value_1;
            layerout (location = 1) in type vertex_value_2;

            out type deliver_to_fragment_date;

            uniform type uniform_name;

            int main(){
                gl_Position = vertex_value_n;
                deliver_to_fragment_date = processing_value;
            }
            layerout (location = 0) 他有最大上限数量,由硬件决定,OpenGL guaranteed至少16 4-component vertex attributes
            ->#Max Vertex Attributes:
                最大数量可以通过glGetInterv(GL_MAX_VERTER_ATTRIBS , &int_parameter)查看.
            gl_Position 是固定的内置输出变量.

        Fragment Shader:
            #version 460 core
            out type out_fragment_date;

            in type input_date_from_vertex_shader

            int main(){
                processing...
                    input_date_from_vertex_shader
                    other date
                .........

                out_fragment_date = Processed_vec4_date;
            }


        其中in type in_variable_name;是指从外部传入的值

        ->#数据类型:
            ->#修饰符:
                layout (location number) ...
                    这是修饰用,不同于uniform,没有存储功能,通过glVertexAttribPointer(locatoin,...);传入
                    编译后无法修改;
                    着色器间不能共用;
            ->#值类型:
                与c语言类型相似,但更以多components为主,最大一般为4:
                    vecn 浮点类型,是默认使用最多的数据类型
                    dvecn 双精度浮点类型
                    bvecn bool类型如bvec1 bvec2 bvec3...
                    ivecn 整数类型
                    uvecn 无符号整数类型

            ->#存储容器类型:
                uniform:
                    是一个相对于每个shader program的全局变量,vertex shader和fragment shader都可以使用.
                    数据并且会一直保持,知道重置或更新.

                    代码上:
                        unsigned _color_index = glGetUniformLocation(shader_program_id,"color");
                            从当前的ShaderProgram中的shader中找到对应的uniform type color;这一步是找到并记录.下一步是传值进去
                        
                        
                        glUniform4f(_color_index,1.0f,0.0f,0.0f,1.0f);
                            传递具体值:
                                f:对应float;
                                i:对应int;
                                ui:对应unsigned int;
                                3f:对应3float(vec3);
                                4f:对应4float(vec4);
                                fv:对应vector数组;

    ->#加载贴图:
        1.首先需要生成贴图索引id:
            unsigned int textures[number];
            glGenTextures(number , texutres);
            接下来对贴图的设置以及数据加载都会绑定在当前贴图索引id;
        2.绑定挡墙要设置的,贴图索引id:
            glBindTexture(GL_TEXTURE_2D , texture[0]);
            设置贴图属性:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S , GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T , GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S , GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S , GL_REPEAT);

        详解:
            glBindTexture(Texture_type , Bind_texture_id)
                Texture_type:
                    指的是当前的要设置的贴图属性的类型:
                        GL_TEXTURE2D:贴图图片
                        GL_TEXTURE_CUBE_MAP:立方体(天空球)贴图
                        GL_TEXTURE_1D:一维纹理
                        GL_TEXTURE_3D:三维纹理
                    
                Bind_texture_id:绑定当前纹理的Id;
            
            glTexParameteri(Texture_type, Texture_filtering , Patten);
                用于设置贴图配置属性.
                Texture_type:同上

                Texture_filtering:
                    设置纹理过滤相关配置问题,有固定选项与之对应:
                            GL_TEXTURE_MAG_FILTER 当前纹理小于<映射目标像素时
                            GL_TEXTURE_MIN_FILTER 当前纹理小于>映射目标像素时
                    Patten:
                            GL_NEAREST  {OpenGL 会选择「纹理坐标最接近的那个纹理像素」的颜色作为采样结果.纹理会呈现 「像素化 / 马赛克」风格，边缘锐利，放大后能看到明显的像素块}
                            GL_LINEAR   {会取「纹理坐标周围的 4 个纹理像素」，对它们的颜色做加权平均计算，得到最终采样颜色.纹理会呈现 「平滑 / 模糊」风格，放大后边缘过渡自然，没有明显的像素块}

                Texture_wrapping:
                    纹理环绕方式,解决,纹理超出边界后的处理效果:
                        GL_TEXTURE_WRAP_S 水平纹理环绕方式
                        GL_TEXTURE_WRAP_T 竖直纹理环绕方式
                        GL_TEXTURE_WRAP_R 深度纹理环绕方式(3D纹理)

                        Patten:
                            GL_REPEAT 重复纹理:
                                纹理坐标超出 [0,1] 的部分，会无限重复平铺纹理图片；
                            GL_MIRRORED_REPEAT 反向重复纹理:
                                纹理坐标超出 [0,1] 的部分，会镜像翻转后重复平铺；
                            GL_CLAMP_TO_EDGE 重复贴图边缘纹理:
                                纹理坐标超出 [0,1] 的部分，会使用纹理边缘的像素颜色填充；
                            GL_CLAMP_TO_BORDER 边缘色纹理:
                                纹理坐标超出 [0,1] 的部分，会使用一个自定义的「边框颜色」填充；
                                需要用 glTexParameterfv（浮点版）设置边框颜色，否则用默认的黑色透明色；
->#编译报错:
    E:/Environments/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: ..\lib/libglfw3.a(wgl_context.c.obj):wgl_context.c:(.text+0x127a): undefined reference to `__imp_SetPixelFormat'
collect2.exe: error: ld returned 1 exit status
    有可能是因为编译时导入的库的顺序出现问题-lglfw3需要在-lgdi32前面;