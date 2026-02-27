<<GLFW>>
    -#简介#

    -#目录#

    --#参数类型#:
        GLFWwindow *window;
            结构体,用于管理窗口的结构体信息.
        GLFWmonitor *monitor;
            结构体,接收物理层的显示器信息
    -#API#
        -初始化(Init):
            --glfwInit();
                它用于初始化glfw的环境.它会检测当前系统环境,根据系统初始化一些核心数据结构,用于管理后续的事件和上下文信息等,如果需要理解它做了什么需要学习对应系统的窗口创建.
            --glfwWindowHint(...);
                用于在创建窗口前对窗口的参数的设置,比如使用什么图形api,抗锯齿,窗口行为等...
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   // OpenGL主版本号3
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);   // 次版本号3
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 核心模式
                glfwWindowHint(GLFW_SAMPLES, 4); 表示4x多重采样抗锯齿
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // 窗口大小固定
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // 窗口大小可伸缩调整
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // 不创建OpenGL上下文，仅用于Vulkan
                ...
            --glfwCreateWindow(...);
                返回GLFWwindow类型,需要有参数接收.
                GLFWwindow *window = glfwCreateWindow(窗口宽,窗口高,窗口标题,窗口在哪个显示器中显示,指定一个存在的窗口,新窗口会与指定窗口共享上下文);
                glfwCreateWindow
                (   
                    ...,
                    ...,
                    NULL,       窗口讲以窗口模式进行,如果指定了显示器,则对应显示器全屏.
                    NULL        NULL表示创建全新窗口,上下文无关联
                );
        -循环逻辑:
            --glfwSetKeyCallBack(GLFWwindow * window , key_call_back);
                对一个窗口获取一些操作,比如事件输出等..
                key_call_back是一个自定义回调函数:
                    void key_call_back(GLFWwindow * window , int key , int scand , int modes)
                    {
                        if (key == GLFW_KEY_ESCAPE && actions == GLFW_PRESS)
                        {
                            glfwSetWindowShouldClose(window, GLFW_TRUE);
                        }                    
                    }
                这个函数会被glfwSetKeyCallBack设置到对应的window中,在主循环中glfwPollEvent()会检查并启用回调函数中的逻辑.
            --glfwWindowShouldClose(GLFWwindow * window);
                会对传入的window中的数据检测,用于关闭while循环
                一般的会由while(!glfwWindowShouldClose(window)){}进行逻辑循环和是否关闭的判断.
                    函数返回GLFW_TRUE,则关闭循环
                    GLFW_FALSE 表示会继续循环 
            --glfwPollEvents();
                事件检测系统();

        -结束逻辑:
            glfwTerminal();

        -其他:
            glfwGetRequiredInstanceExtensions(uint32_t glfw_extension_count);
                自动检测当前平台（Windows/Linux/macOS），返回 GLFW 与 Vulkan 兼容必需的扩展列表，无需开发者手动判断平台或硬编码扩展名称，简化跨平台开发。

        -扩展Extensions:
            VK_KHR_surface