1.“初始化”: 无法从“const _Elem”转换为“const vec3 &”:
    实例:
        模板特化判断:
            namespace types
            {
                template<typename T ,typename U>
                struct is_same 
                {
                    static constexpr bool value = false;
                };
        

                template<typename T>
                struct is_same<T,T>
                {
                    static constexpr bool value = true;
                };
            }

        class vec3;
        static std::ofstream LOGER_FILE_STREAM("loger.log",std::ios::app);

        template<typename T>
        inline static void DOLOGE(const std::initializer_list<T> args)
        {   
            if(LOGER_FILE_STREAM.is_open())
            {
                xyl::types::is_same<T,vec3> typeCheck;
                if(typeCheck.value)     --出错地方
                {
                    LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "; 
                    #ifdef BINARYREAD
                        for(auto it = args.begin();it != args.end();it++)
                        {   
                            LOGER_FILE_STREAM<<*it;
                        }
                    #else
                        for(const vec3& it : args)
                        {
                            LOGER_FILE_STREAM<<it.x()<<"\t\t"<<it.y()<<"\t\t"<<it.z()<<"\n";         
                        }
                        
                    #endif
                }
                else
                {
                    for(const T& it : args)
                    {
                        LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "<<it<<"\t"<<std::endl;
                    }
                }
            }
        }
    在使用模板特化判断两个模板是否相同的时候,进行if判断,即使传入的args类型与模板比对不同,即为false,依然会走if(true)的判断,并进行报错,显示为,两个类型不同且无法转换.
    解决:
        因为if()是在运行时判断,但是在编译时无论条件判断为true还是false,都会进行遍历,所以在编译的时候这两个类型无法互相转换,则报错;
        因此需要在条件判断中添加修饰符if constexpr(){},如此在编译期间就会进行判断,如果是false则跳过该分支.
        此外,对于if constexpr(<param>)中的<param>可以在编译期间进行初始化并判断是因为,特化模板中的参数也是constexpr

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


 MinGW 编译 Win32 原生窗口导致的字符编码不匹配:
PS G:\user\desktop\C++\GraphicLearning\D_OpenGL\3.Camera> g++ main.cpp ..\src\glad.c -I ..\includes -L ..\libs -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -std=c++17 -o main.exe
main.cpp: In function 'HWND__* CreateMainWindow(HINSTANCE)':
main.cpp:29:24: error: cannot convert 'const wchar_t [16]' to 'LPCSTR' {aka 'const char*'} in assignment     
   29 |     wc.lpszClassName = L"MainWindowClass";
      |                        ^~~~~~~~~~~~~~~~~~
      |                        |
      |                        const wchar_t [16]
main.cpp:36:9: error: cannot convert 'const wchar_t*' to 'LPCSTR' {aka 'const char*'}
   36 |         L"MainWindowClass",             // 绐         ^~~~~~~~~~~~~~~~~~
      |         |
      |         const wchar_t*
In file included from E:/Environments/mingw64/x86_64-w64-mingw32/include/windows.h:72,
                 from main.cpp:2:


    对于windows的窗口代码中会使用到L类型(宽字符类型),MinGW默认是多字节编译模式,因此需要改变编译模式:
    在编译时添加:-DUNICODE -D_UNICODE
    完全:
        g++ main.cpp ..\src\glad.c -I ..\includes -L ..\libs -lopengl32 -lglfw3 -lgdi32 -fno-permissive -Wall -Wextra -std=c++17 -DUNICODE -D_UNICODE -o main.exe
        
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

PS G:\user\desktop\C++\GraphicLearning\0_MyFunctions> g++ main.cpp -fno-permissive -std=c++17
In file included from main.cpp:1:
./includes/xyl_math.hpp:113:27: error: expected ')' before '<' token
  113 |         mat4x4(coordinate4<T> coordinate1,
      |               ~           ^
      |                           )

这里是因为没有加名称空间:
    xyl::coordinate4<T> ....就好了










E:/Environments/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\LENOVO\AppData\Local\Temp\cc6JyPgm.o:main.cpp:
(.rdata$.refptr._ZN6Entity5widthE[.refptr._ZN6Entity5widthE]+0x0): 
undefined reference to `Entity::width'

问题源代码:
struct Entity
{
public:
    static const int width = 1280;
    static const int height = 1080;
    GLFW glfw;
    GLAD glad;
};

int main()
{
    Entity entity;
    entity.glfw.InitGlfw(Entity::width, Entity::height, "Camera");
......
}


问题原因:
    报错是C++ 静态成员变量的声明 / 定义分离规则导致的链接错误，与语法、编译器（MinGW）无关，是 C++ 语言标准要求；
核心解决步骤：类内声明（static + 类型 + 变量名）+ 类外定义（类型 + 类名：：变量名，可初始化）；

解决办法:
    static const int width = 1280;
    static const int height = 1080;
    在类外定义;
    也可以单独在一个hpp文件里声明,然后在cpp中定义,这样就会有一次编译,对该静态变量可以初始化成功.
