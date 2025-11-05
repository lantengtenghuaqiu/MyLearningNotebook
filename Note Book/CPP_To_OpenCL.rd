
<<C/C++笔记>>:
	->简介:
		我将<>作为占位符,一般来说所有的<...>中的内容都是可变的,除非<>有特殊编程含义,比如模板template<typename T>,这里的<>是固定写法

	->#终端编译专栏#:
		-通过终端编译c++文件,生成.exe文件:
			1.需要在被编译的文件夹下	
				g++ <filename>.cpp -o <filename>.exe会根据<filename>.cpp查询并编译cpp文件,然后通过-o生成二进制文件<filename>.exe.
		
			2.只编译不生成可执行文件:
				g++ -c <filename>.cpp 这里的-c指的是compile only,所以只会编译成.o文件不会生成可执行文件,可以为了之后链接
				g++ -c <filename>.cpp -o <folder/filename.obj> -o指的是out,这段代码指的是将编译的文件输出到指定位置和指定名字

			3.如果有头文件则需要先编译头文件所对应的源代码.cpp文件,再生成.o文件,最后链接main的.o文件和头文件的.o文件
				g++ -c <headcpp>.cpp
				g++ -c <main>.cpp
				g++ <main>.o <headcpp>.o -o <executable file name>


			4.对于不在同一路径下的头文件包含:
				-当编译时,包含头文件不是相对路径也不是绝对路径,只是#include”header.hpp”的时候,编译需要说明:
					g++ -c <main>.cpp -I <头文件目录(无需显示声明.hpp文件) -o <生成地址和文件.obj>
                -多头文件且不在一个文件夹下的包含:
					g++ -c <main>.cpp -I <头文件目录1(无需显示声明.hpp文件)> -I <头文件目录2(无需显示声明.hpp文件)> -o <生成地址和文件>.obj
		-库文件生成:
			MinGW:
				MinGW下编译生成静态库:
					首先要准备头文件<stlb>.hpp和对应的实现文件<stlb>.cpp
					<stlb>.hpp文件结构:
						#ifndef MTBL_H
						#define MTBL_H
							#include<iostream>
							void func1();
						#endif
					<stlb>.cpp文件结构:
						#include"stlb.hpp"
						void func1()
						{
							std::cout<<""Hello World"<<std::endl;
						}
					<main>.cpp文件结构:
						#include <iostream>
						#include "stlb.hpp"
						int main()
						{
							func1();//这是stlb.hpp中的函数
							return 0;
						}
					1.将头文件所对应的实现文件编译为.obj文件:
						g++ -c <stlb>.cpp -o <folder/stlb>.obj

					2.将编译好的.obj文件生成为.a文件:
						ar rcs <stlb>.a <stlb>.obj
							这里的是对ar打包工具的详解:
								ar [Operational Option][Auxiliary Option 1][Auxiliary Option 2][...] <静态库名>.a <obj文件>.obj
									[Operational Option] is a prerequisite,but [Auxiliary Option] is a optional.
									[Operational Option]:	
										r(add or replace)

									[Auxiliary Option]:
										"Create static library" :c(If the assigned .a file does not exist then create the .a file,but if it existed ,othing needs to be done)
										"Create symbol table"	:s(The symbol table records the locations of all functions,variables and names in a static library,and helps the Linker quickly find them)
															 		//And it is necessary to static librarys.
					3.将main.cpp文件和库文件进行链接,并生成.exe文件:
						g++ <folder/main>.cpp -L. <folder/stlb>.hpp -o <folder/main>.exe
											//其中-L.中的.表示当前目录.
											//如果不在当前目录可以使用-L. <folder/stlb>.a表示在当前目录下的folder文件夹内
									

	->#线程专栏#:
		-对于c++的线程休眠:
			_sleep(time) 这个函数需要包含#include<thread>,但是使用MinGW时在编译时会提示不推荐使用
					|--------------------------------------------------------------------------------------------|
					|E:/Environments/mingw64/x86_64-w64-mingw32/include/stdlib.h:689:24: note: declared here 689 |
					|   _CRTIMP void __cdecl _sleep(unsigned long _Duration) __MINGW_ATTRIB_DEPRECATED;		     |
					|--------------------------------------------------------------------------------------------|
			是因为这不是c++标准,是微软平台下的线程休眠的旧方法,已废弃,所以在不同平台下应使用相应平台的休眠:
			Windows 下: 包含#include<window.h> 使用Sleep(time)

	->#运算符#:
		1.位运算:
			int n=3<<1;     11 -- 110
			std::cout<<n<<std::endl;  == 6

			int n=1<<2;     1 -- 10
			std::cout<<n<<std::endl;  == 4

			int n=2<<2;     10 -- 1000
			std::cout<<n<<std::endl;  == 8

			位运算或相加:
				int result = (1<<1)|(1<<2);  等价于  2|4 = 6 && 0b10 | 0b100 = 6;
				int result = (2<<1)|(2<<3);  等价于  4|8 = 12 && 0b100 | 0b1000 = 12;


	//类似C#中的变长参数:
		需要包含头文件#include<initializer_list>
		-变量使用变长参数:
			1.使用变长参数进行文件流输入:
				static std::ofstream LOGER_FILE_STREAM("loger.log",std::ios::app);

				template<typename T>
				inline static void DoLog(const std::initializer<T> args)
				{
					for(const auto& it : args)
					{
						LOGER_FILE_STREAM<<args<<" ";
					}
					LOGER_FILE_STREAM<<std::endl;
				}

				在使用的时候比较特殊:
					1.
						DoLog({1,2,3,4,5});

					2.
						int a=1;
						int b=2;
						int c=3;
						DoLog({a,b,c});

		-文件流可用的变长参数:
			1.对文件流文件批量关闭:
				对于流文件的变长参数不可以使用上方,因为initializer_list<std::fstream>中的std::fstream默认为const形式,无法修改:
					所以使用变长参数,关闭流文件
						void CloseStreaming(){}	//这是关键不要漏下
						template<typename ... Args>
						void CloseStreaming(std::ofstream& first , Args& ... rest)
						{
							if(first.is_open())
							{
								first.close();
							}

							CloseStreaming(rest...);
						}



	
	//特化(c++特性):
			特化时c++的一种特性,
				使用特化创建类似c#中的typeof判断:
					主模板:
						template<typename T, typename U> 
						struct is_same
						{
							static constexpr bool value = false;
						}
					偏特化:
						template<typename T>
						struc is_same<T,T>
						{
							static constexpr bool value = true;
						}
					当使用is_same<type1,type2> check;的时候,传入type1和type2如果相同会返回偏特化,则value为true,反之返回主模板false
					实例:
						class vec3;
						template <typename K, typename T>
						inline K clamp(const K &val, const T &min, const T &max)
						{
							xyl::types::is_same<K,vec3> type1;
							if constexpr(type1.value)
							{
								vec3 val_temp = val;
								val_temp.x() = val_temp.x()<min ? min : val_temp.x > max ? max:val_temp.x();
								val_temp.y() = val_temp.y()<min ? min : val_temp.y > max ? max:val_temp.y();
								val_temp.z() = val_temp.z()<min ? min : val_temp.z > max ? max:val_temp.z();
								return val_temp;
							}

							xyl::types::is_same<K, T> type2;
							if constexpr (type2.value)
							{
								if (val < min)
									return min;
								if (val > max)
									return max;
							}

							return val;
						}

						其中对于特化模板的使用,因为是constexpr所以可以使用类似static通过域调用的方法,减少一个变量的创建更加优化:
							if constexpr (xyl::types::is_same<K, T>::value);

	->#类专栏#:
		1.类,特殊成员函数的默认版本:
			类中,如果无显示声明,则会自动生成一些特殊成员函数包括:
				默认构造函数（无参构造函数）；
				析构函数；
				拷贝构造函数；
				拷贝赋值运算符（operator=）；
				移动构造函数（C++11 后）；
				移动赋值运算符（C++11 后）。
			这些自动生成的成员函数会包含默认版本,比如析构函数会自动调用.但是如果显示的声明了特殊成员函数,则特殊成员函数不会包含默认版本
			所以如果声明了特殊成员函数,在某些情况下需要手动调用默认版本;
			所以c++提供了显式声明启用默认版本(=default);
			例如当有一个虚函数,等待被继承,当显式声明析构函数,则需要保证继承类能正确调用析构函数.
				virtual ~hittable() = default;
		2.纯虚函数:
			表示一个函数没有默认实现,需要由派生类(子类)继承并实现;
			此外包含纯虚函数的类会变成抽象了,无法被直接实例化,只能被基类继承;
				virtual bool hit(const ray& r,double ray_tmin , double ray_tmax , hit_record& hitInfo)const = 0;
				这里的=0是声明纯虚函数的关键

		3.public private protected三种类型的继承含义:
			class example1
			{
				private:
					int num1;
				public:
					int num2;
				protected:
					int num3;
			};

			公共继承:
				class example2 : public example1
				{
					void func()
					{
						可以调用基类的公有和保护:
						this->num2;
						this->num3;
						
						基类中的私有无法调用
					}
				};

			私有继承:
				class example3 : privated example1
				{
					void func()
					{
						基类中所有的数据都私有化,无法调用
					}
				};

			保护继承:和公共继承几乎一样
				class example4 : privated example1
				{
					void func()
					{
						可以调用基类的公有和保护:
						this->num2;
						this->num3;

						基类中的私有无法调用
					}
				};
	
		强枚举 enum class:



	->#函数专栏#:
		1.函数中参数中的引用&:
			在一个函数中void func(int & para){}会有&这样的符号,这个符号表示引用,
			在使用函数func的时候传入para时,参数列表中的形参会直接关联实参,也就是说para改变时,实参也会跟着改变,同样,参数对应的实参改变,para也会随之改变;
				*上已证实(/Test/test_refanceParamates.cpp)
			引用的作用之一就是,避免传入参数时拷贝占用内存空间.此外对于非引用的重要一点就是不会改变实参,所以对于引用可以添加const修饰符进行修饰,避免para改变实参;
				我现在有一个class类vec3,内部有operator-()的实现,直接初始化,可以直接Negate a number:
				vec3 dir = new vec3(1,2,3)
				dir = -dir;
			但是如果是指针形式,需要对其解引用,再赋值:
				vec3 *dir = new vec3(2,3,4);
				*dir = -(*dir);

		2.函数参数中的void:
			下面有一函数:
				inline double random_double_normalized(void){}
				其中函数参数中的void表示无参函数,是c++标准写法,与inline double random_double_normalized()等价




	->#宏专栏#:
		在进行#define XXX宏定义的时候,宏只会包含在当前文件中,在编译其他cpp文件的时候,并不会因为在main.cpp中进行了XXX的define,而别的#include"head.hpp"就可以共享该宏定义
		-多行宏定义:
			#define CHECK_ERROR(err) \
			if (err != CL_SUCCESS) { \
				std::cerr << "OpenCL error at line " << __LINE__ << ": " << err << std::endl; \
				exit(EXIT_FAILURE); \
			}
			\ 后面面不能有任何字符（包括空格），否则会被视为普通字符，导致续行失败,去掉 \，编译器会认为 #define CHECK_ERROR(err) 只定义到第一行为止，后续的 if 语句会被当作普通代码，导致编译错误
		
		-宏标记:
			1.在c++中 可以使用宏进行特殊标记:
				#ifdef WIN32
					#define UDERWIN __declspec(dllexport) //Windows下标记导出记号
				#endif

				此时constexpr UDERWIN int num = 10;
				等价于constexpr __declspec(dllexport) int num = 10;
			
			2.有的时候也会使用空宏标记
				#ifdef WIN32
					#define UDERWIN  //Windows下标记导出记号
				#endif
				
				此时constexpr UDERWIN int num = 10;
				等价于constexpr int num = 10;即原来的代码

	->#头文件专栏#:
		//头文件前向声明:
			当头文件A.hpp中定义了B.hpp文件,并且在B.hpp文件中又包含了A.hpp文件,当main.cpp文件包含了这两个其中一个文件,都会导致循环定义,最后冲突无法正确构建:
			所以需要使用前向声明改变这种冲突:
			//没太搞明白//

		//通过define,如果包含一个特殊宏定义,该头文件内的方法才可以使用:
			头文件A:
				#ifndef DAH
				#define DAH

				#define MATH

				#endif
			头文件B:
				#ifndef DAH2
				#define DAH2

				#include"头文件A.hpp"

				#ifdef MATH	//如果包含了头文件A则查看该文件中有define MATH,所以该宏下可以使用
					int num = 0;

				#endif
				#endif
		//多宏包含判断:
			#if defined(宏定义1) && defined(宏定义2)
			#endif



	->#修饰符(关键字)类专栏#:
		-const:
			//const是一个常量修饰符,总的来说是承诺不能修改修饰的变量;
			1.成员函数const:
				常量成员函数(const成员函数),主要用在自定义类型的类和结构体种中,修饰在函数最后,用以保证该函数不修改成员变量.
				对于一些可能只是保证不改变某一些变量,但是有些变量需要被改变的,需要加上mutable修饰符
				class c
				{
					private:
						int a = 0;
						mutable int b = 0;
					public:
						void func() const
						{
							a = 1;***!!!*** 不合法,无法修改成员
							b = 9;//合法,可以修改
						}
				}

			2.const变量:
				对于const变量,为保证该变量不在之后的逻辑中被修改,而添加const修饰.此外,由const修饰的变量,也称为常量,如一个自定义类型常量不只调用非const成员函数,只能调用const成员函数;
				class C
				{
					public:
						void func1() const{}
						void func2(){}
				}

				main()
				{
					const C c;//没有显式定义无参构造,初始化只不需要C c();
					c.func1();//合法
					c.func2();**!!不合法!!**

					//于此同时,非const变量,都可以调用两个func()
				}
		-extern:
			× 该关键字是一个声明"外部链接性"的标识符,也是一个类似全局变量的东西,告诉编译器该定义的内容在其他文件中.而且extern修饰的变量不应该被初始化
				× 即:extern int num;是在A.hpp中声明的一个变量,我现在可以在包含该头文件的源文件夹下,如头文件B.hpp
		
		-constexpr 编译期初始化:
			该关键字可以修饰使一些变量,自定义类型,函数,构造函数,for循环等在编译期就确定值,而不是在运行时获得值;
				class Point {
					private:
						int x, y;
					public:
						// constexpr 构造函数：确保对象可在编译期初始化
						constexpr Point(int x_, int y_) : x(x_), y(y_) {}
				};
				如果构造函数的参数在编译前（编译期）没有确定（即参数是运行期变量），constexpr 构造函数会退化为普通构造函数，在运行期执行初始化逻辑 —— 这和非 constexpr 构造函数的行为完全一致
			
			constexpr和static的区别:
				static:	修饰的是在程序运行时初始化,全局生命周期,只有当程序结束时才销毁
				constexpr:	在编译期间就确定,所以运行期间也已确定,存储的生命周期,与局部变量相同,即当前域中(函数域/程序域/名称空间域...);

		-explicit:
			表示不允许隐式转换,必须使用显式转换

		-R"",原始字符字面量:
			引号内的字符串,不会因为有转义字符"\"而失效,完全按字面处理
				// 普通字符串：用 \ 转义 "
					const char* str1 = "He said \"Hello World\""; 

				// 原始字符串：直接写 "，无需转义
					const char* str2 = R"(He said "Hello World")"; 




					
	->#指针专栏#:
		所谓指针,就是主动创建的一份内存空间,将地址传递给指针,让该指针操作该空间的内容.
		1.创建array需要用引用形式:
			int *num = new int[3];

		2.new的简单使用:
			const int *number = new int;
			声明需要记得删除指针:delete(number);

		3.智能指针:
			需要包含#include<memory>
			1.共享指针--shared_ptr<...>:

		4.通用指针(无类型指针|泛型指针):
			void* 是一个无类型指针,可以指向任意类型的内存地址,但不包含所指向数据的"类型信息"

			此外void* 所指向的指针,它不能通过解引用获得具体数据,想要获得数据就必须转换为所指指针的数据类型.
				int* num0 = 10;
				void* num1 = &num0;
				//如果要获取地址中的数据(需要进行类型转换):
				int* result = (int*)num1;
			void* 用于处理类型不确定性.一般用在函数传参的时候,保持通用属性,以及作为c和c++的接口
		5.通过C语言跨过const,修改const char*的内容:
			extern "C" void func(void* val , size_t len , char* afterVal)
			{
				
				char* result = (char*) val;//首先需要转换类型到char*
				for(int i= 0;i<len;i++)
				{
					result[i] = afterVal[i];//通过指针深拷贝,间接修改原内容
				}
			}
			std::string val = "befor content";
			std::string afterVal = "Changed Values";
			val.resize(afterVal.capacity());

			func(val.data(),afterVal.capicty(),afterVal.data());
		
		6.C风格动态分配内存(calloc):
			float* number = (float*) calloc(uint length , sizof(float));
				创建length长度,每份都为float大小的内存空间,因为calloc返回为void*所以需要用(float*)转换,才能将开辟的空间传递给number


<<OpenCL>>:
    ->#简介#:
        ....
    ->#终端链接编译OpenCL#:
        -终端链接并编译OpenCL:
			1.<main>.cpp:
				#define CL_TARGET_OPENCL_VERSION 300 //指定版本解决版本没指定的提示
				#include<CL/cl.h>
				#include<iostream>
				int main()
				{	
					std::cout<<"Hello World"<<std::endl;
					return 0;
				}
			2.开始链接并编译:
				g++ <main>.cpp -I <E:\Environments\CUDA\include> -L <E:\Environments\CUDA\lib\x64> -lOpenCL -o <main>.exe


	->cl_platform_id 平台信息:
		是一个自定义的struct类型,可以保存一些平台信息.
		创建cl_platform_id platform;用于保存平台信息.
		信息的获取使用clGetPlatformIDs(1,&platform,nullptr);
			参数含义:
				cl_uint num_entries:        1           表示期望获取1个平台的数据;实际上如果电脑只有1个,传入多个也不会影响,最终还是以实际获取
				cl_platform_id* platforms:  &platform   这是将获得的1个平台的信息传递到该结构变量中;    
				cl_uint* num_platforms:     nullptr     表示不关心系统有多少个平台,只获取第1个;传入&platformCounts获得实际的平台数量,并写入platformCounts

		获取平台数量:
			cl_uint platformCounts;
			clGetPlatformIDs(0,nullptr,&platformCounts);
			std::cout<<"Platform counts: "<<platformCounts<<std::endl;我的电脑为1

	->cl_device_id 指定设备:
		是一个自定义的struct类型,可以保存一些设备信息.
		创建cl_device_id device;用于保存平台信息
		信息获取使用clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,nullptr);
			参数含义:
				cl_platform_id   platform       platform :          在上一步获取的结构体,包含所需要使用的平台信息
				cl_device_type   device_type    CL_DEVICE_TYPE_GPU: 表示需要获取GPU设备信息
				cl_uint          num_entries    1:                  表示获取期望获取设备数量
				cl_device_id *   devices        &device:            会根据num_entries获取信息,这里表示opencl找到的第1个设备信息,并传入该结构体
				cl_uint*         num_devices    nulltpr:            表示不关心有多少个GPU,只获取第一个      

	->cl_context 上下文:
		是一个自定义的struct类型,通过前置获取的device,是连接CPU和GPU等设备的桥梁.
		创建了cl_context context;变量后,就需要链接其他设备了.
			比如使用context = clCreateContext(nullptr,1,&device,nulltpr,&err);
			这里clCreateContext返回类型为"cl_context",这个函数是通过获取的设备信息创建上下文管理其他设备的管家.
				cl_context context = clCreateContext(nullptr,1,&device,nullptr,nullptr,&err);
					参数表示:
						const cl_context_properties *       properties                  :上下文属性表,nullptr表示默认opencl自动选择平台,但是如果系统有多个平台,而device又是其中一个会导致关联的设备不是预期的
						cl_uint                             num_devices                 :表示关联的设备数量,1表示只关联一个设备
						const cl_device_id *                devices                     :表示指向要关联的设备,与传入的&device挂钩.
						void (CL_CALLBACK * pfn_notify)     (const char * errinfo,      :这个是一个错误回调函数,当发生错误则会提示,如果传入nullptr表示不需要提示
															const void * private_info,
															size_t       cb,
															void *       user_data)
						void *                              user_data                   :与上一个参数关联,因为是nullptr所以这里也是nullptr.
						cl_int *                            errcode_ret                 :这是错误指针码,与一个cl_uint参数挂钩,一般会在开头创建一个宏函数,显示报错行数的.
		
		对于非默认获取设备类型需要自定义设置:
			cl_context_properties properties[] =
			{
				CL_CONTEXT_PLATFORM,                    // 键：指定上下文所属平台
				(cl_context_properties)platform,        // 值：平台ID（需强制类型转换）
				0                                       // 数组结束标志
			};
			properties是给驱动看的:
			CL_CONTEXT_PLATFORM:是告诉驱动我要配置的属性是上下文所属平台.
			(cl_context_properties)platform:是平台信息,通过clGetPlatformIDs(...)获取的平台数信息.
											这里能从结构体转到longlong类型是因为platform是指针类型,所以是转的地址码

		当然context和device都可以绑定多个设备:
			cl_device_id devices[2] = {gpu_device, cpu_device}; //通过数组创建多个设备管理器,在clGetDeviceIDs(...)中获取两个设备
			cl_context context = clCreateContext(nullptr, 2, devices, nullptr, nullptr, &err);


	->cl_command_queue 命令队列:
		是一个struct类型,命令队列是主机（CPU）向设备（GPU 等）发送计算命令（如执行内核、传输数据）的 “通道”，负责管理命令的执行顺序、同步方式和优先级。
		cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);来进行队列的设置.
			参数表示:
				clCreateCommandQueueWithProperties(
													cl_context                   context        上下文管理
													cl_device_id                 device         设备管理
													const cl_queue_properties*   properties     队列执行方式,nullptr表示默认有序执行模式
													cl_int*                      errcode_ret    &erro
												);
				cl_queue_properties有三种:
					性能分析:
						cl_queue_properties props[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE,0};
					乱序执行:
						cl_queue_properties props[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,0};
					乱序加性能分析:
						cl_queue_properties props[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE,0};
	
	->cl_program:
		内核源代码相关联的程序对象,
			cl_program clCreateProgramWithSource(
				cl_context          context,        // 参数1：关联的上下文
				cl_uint             count,          // 参数2：源代码字符串数组的数量		这里是指有多少组源代码,不是每段源代码有多少字符	
				const char**        strings,        // 参数3：指向源代码字符串的指针数组
				const size_t*       lengths,        // 参数4：每个源代码字符串的长度（可选）
				cl_int*             errcode_ret     // 参数5：错误码指针
			);
		之后会由clBuildProgram(...)针对上下文关联的设备,将程序对象转换为二进制代码
	
	->clBuildProgram()
		将内核源代码编译为设备可执行的二进制数据:
			cl_int clBuildProgram(
									cl_program            program,                 		// 参数1：要编译的程序对象
									cl_uint               num_devices,             		// 参数2：要编译的设备数量
									const cl_device_id*   device_list,             		// 参数3：目标设备ID数组
									const char*           options,                 		// 参数4：编译选项（字符串）
									void (*pfn_notify)(cl_program, void* user_data),  	// 参数5：编译完成的回调函数
									void*                 user_data                		// 参数6：传递给回调函数的用户数据
								);
	->cl_kernel:
		内核函数对象:
			创建好的kernel对象,之后会使用clCreateKernel进行内核代码具体函数提取:	
			cl_kernel clCreateKernel(
										cl_program    program,        // 参数1：已编译的程序对象
										const char*   kernel_name,    // 参数2：要提取的内核函数名
										cl_int*       errcode_ret     // 参数3：错误码指针
									);	
			因为每个内核源代码虽然是一段内容,但是里面可以包含多个函数,clCreateKernel就是提取具体函数的;
			比如下段:
				const char* kernelSource = R"(
										__kernel void Add(__local const float* inputa,__local const float* inputb , __global float* output)
										{
											int id = get_global_id(0);
											output[id] = inputa[id] + inputb[id];
										}
									)";
				kernel获取的就是Add

    ->#OpenCL#内核源代码专栏:
        内核源代码,是指用OpenGL C语言编写的,在计算机设备(GPU/CPU等)上并行计算的逻辑代码.主机CPU通过命令队列将内核代码发送到设备上.
			__kernel 返回类型 函数名称(...){...}
				传入的参数类型(非传统类型):
					__global:设备全局变量(供所有工作项共享,容量大但访问慢)
					__local:设备局部内存(仅同一工作组内工作项共享,容量小,但访问速度快)
					__private:每个工作项的私有内存(默认,仅当前可见)

