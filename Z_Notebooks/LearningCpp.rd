<<C/C++笔记>>:
	->#目录索引#:
		-简介
		-终端编译专栏
		-线程专栏
		-线程专栏
		-条件判断专栏
		-特化专栏
		-自定义类型专栏
		-函数专栏
		-宏专栏
		-头文件专栏
		-修饰符专栏(关键字)
		-指针专栏
		-数据结构专栏
		-优化思路

		->简介:
			我将<>作为占位符,一般来说所有的<...>中的内容都是可变的,除非<>有特殊编程含义,比如模板template<typename T>,这里的<>是固定写法

		->C++深入:
			在c++中,一个类的在初始化时,如果时普通变量,则他的地址会和其第一个成员变量的地址相同.
			从底层考虑是第一个成员变量的地址偏移为0,所以变量地址和成员地址相同.

		->#终端编译专栏#:
			-#通过终端编译c++文件,生成.exe文件:
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
				
				5.批量编译:
					对于头文件批量编译成.o或者.obj等文件可以通过通配符解决:
						对于Inlcuder下有10个.hpp文件:
							g++ -c ./*.cpp -fno-permissive -std=c++17 -Wall -Wextra -Werror
						可以将当前文件夹下的所有.cpp文件进行编译
				
				6.严格的c++标准编译:
					对于windows下的MinGW下的g++默认使用宽容编译,大量错误难以严格指出,所以需要在编译的时候严格限定编译规范:
						g++ <....> -fno-permissive -std=c++17 -Wall -Wextra -Werror 可以检测大量问题,满足多平台都可以跑
						__________________________________________________________________________________________________________________
						|OPERATIONS			|	EFFECTS
						|-fno-permissive	|	禁用 -fpermissive 宽容模式（默认可能开启），将非标准语法从 “警告” 升级为 “错误”。			
						|-std=c++17(or more)|	明确指定 C++ 标准（如 C++17、C++20），强制编译器按该标准检查语法（而非默认的非标准模式）。
						|-Wall				|	启用所有重要警告（如未使用变量、类型不匹配等）。
						|-Wextra			|	启用额外警告（比 -Wall 更严格，如无意义的比较、冗余代码等）。
						|-Werror			|	将所有警告视为错误（确保不忽略任何潜在问题）。
						__________________________________________________________________________________________________________________

				7.生成预处理阶段的代码:
					g++ -E <main>.cpp -I <includer> -o <main>.i

				8.编译时设置全局宏:
					对于一些宏定义,进行判断选择那种函数逻辑的时候,如果函数的逻辑和实现都在hpp文件,main.cpp文件进行宏定义#define的时候,函数会获得我定义的宏
					但是如果,函数的声明在hpp,实现在cpp,在main.cpp中声明#define并不会在函数实习内生效
					结构:
						// A.hpp
							#pragma once
							int func();  // 仅声明，无实现
						// A.cpp
							#include "A.hpp"
							int func() {
								#ifdef F1
									return 1;
								#endif
								#ifdef F2
									return 2;
								#endif
								return 0;
							}
					对于这种情况,需要在编译的时候,设置全局宏:
						g++ <main>.cpp <A>.cpp -D <F1> -o <main>.exe
			
			
			->#库文件生成:
				MinGW:
					1.MinGW下编译生成静态库:
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
							ar rcs <stlb>.lib <stlb>.o
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
							g++ <folder/main>.cpp -I <Includer_Path> -L <Library_Path> -l <stlb> -o <folder/main>.exe
												//其中-L.中的.表示当前目录.
												//如果不在当前目录可以使用-L </folder/stlb> 表示在当前目录下的folder文件夹内
			->#终端构建的完整流程:
				->#文件目录:
					Name:
						includes:
							head1.hpp
							head2.hpp
						src:
							head1.cpp
							head2.cpp
						lib:
							libhead1.a
							libhead2.a
						project:
							main.cpp
				->#详解:
					main.cpp中包含并调用了head1.hpp和head2.hpp中的方法.
					(但是头文件只是函数的引用不包含方法,因此他只是一个目录,真正的实现方法在src中的cpp文件中)
					因此首先需要构建两个头文件成二进制文件:
						Mac下:
							1.构建头文件:
								构建头文件1:
									../include>clang++ -E head1.cpp -o head1.ii 这一步将文本编译为 预编译文件
									../include>clang++ -S head1.ii -o head1.s 这一步为将预编译文件编译为 汇编文件
									../include>clang++ -c head1.s -o head1.obj 这一步是将汇编文件编译为 二进制文件 | 等待链接
									../include>ar rcs libhead1.a head1.obj 这一步是将二进制文件编译为 静态库.a文件
								构建头文件2:
									../include>clang++ -E head2.cpp -o head2.ii 这一步将文本编译为 预编译文件
									../include>clang++ -S head2.ii -o head2.s 这一步为将预编译文件编译为 汇编文件
									../include>clang++ -c head2.s -o head2.obj 这一步是将汇编文件编译为 二进制文件 | 等待链接
									../include>ar rcs libhead2.a head2.obj 这一步是将二进制文件编译为 静态库.a文件

							2.构建main文件:
								../include>clang++ -E main.cpp -o main.ii 这一步将文本编译为 预编译文件
								../include>clang++ -S main.ii -o main.s 这一步为将预编译文件编译为 汇编文件
								../include>clang++ -c main.s -o main.obj 这一步是将汇编文件编译为 二进制文件 | 等待链接

							3.链接二进制文件:
								../include>clang++ main.obj -I ../includes -L ../lib -lhead1 -lhead2 -o main
								(这里需要注意,因为链接库文件,因此不需要在链接时添加对应的头文件的obj文件)

								如果不链接库文件
									../include>clang++ main.obj ../src/head1.obj ../src/head2.obj -I ../includes -o main

		->#线程专栏#:
			-#对于c++的线程休眠:
				_sleep(time) 这个函数需要包含#include<thread>,但是使用MinGW时在编译时会提示不推荐使用
						|--------------------------------------------------------------------------------------------|
						|E:/Environments/mingw64/x86_64-w64-mingw32/include/stdlib.h:689:24: note: declared here 689 |
						|   _CRTIMP void __cdecl _sleep(unsigned long _Duration) __MINGW_ATTRIB_DEPRECATED;		     |
						|--------------------------------------------------------------------------------------------|
				是因为这不是c++标准,是微软平台下的线程休眠的旧方法,已废弃,所以在不同平台下应使用相应平台的休眠:
				Windows 下: 包含#include<window.h> 使用Sleep(time)

		->#运算符专栏#:
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

		->#条件判断专栏#:
			-#while 和 return:
				除了 break;会结束循环.
				如果在while里面使用return 0;之类的也会结束循环.
				此外在执行上,while 和 for循环的资源消耗相同

		->#特化专栏#:
			特化(c++特性):
				特化时c++的一种特性,
					使用特化创建类似c#中的typeof判断:
						主模板:
							template<typename T, typename U> 
							struct is_same
							{
								static constexpr bool value = false;
							};
						偏特化:
							template<typename T>
							struc is_same<T,T>
							{
								static constexpr bool value = true;
							};
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

		->#自定义类型专栏#:
			-#类class:
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
			
				4.强枚举 enum class:

				5.模板类继承:
					对于基类,如果是一个模板类,则他的继承类需要在继承的时候声明类型:
						template<typename T>
						class point
						{
							T origin[3];
						};
						
						template<typename T>
						class vec3 : public point<T>
						{
							private:
								T magnitude;
								T direction[3];
						};
			
			-#结构体struct:
				1.列表初始化:
					对于结构体,有一种初始化方法:
						struct MyStruct
						{
							int num;
							float f_num;
							string str;
						};
						对齐使用{0}初始化:
						main
						{
							MyStruct first{0};	//这里就是列表初始化
												//表示对其成员进行初始化,如果是数值类型的如int float等会直接赋予0,
												//如果是自定义数据类型,如我自己创建的class或struct,或者说是string(他也是一个自定义数据),则调用他们的构造函数.
							...
						}

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

			3.函数指针--请看#指针专栏#
				函数也有地址

			4.C风格输出流:
				输出指针地址占位符:
					printf("%p" , &point);
					printf("%d" , (int)val);
					printf("%f" , (float)val);
					printf("%s" , "abc");
			5.回调函数:
				通过函数指针,在同一个函数里,通过调用不同函数,进行不同形式逻辑执行:
					现在我有三个函数:
						计算逻辑函数:
						int Add(int a ,int b)
						{
							return a + b;
						}
						int Multiple(int a , int b)
						{
							return a * b;
						}
						执行逻辑函数:
						int Calculate(int a,int b , (int)(*func)(int a,int b))
						{
							return func(a,b);
						}
						执行:
						int main()
						{
							int result = 0;

							result = Calculate(1,2,Add);   //输出1 + 2 = 3

							result = Calculate(1,2,Multiple); //输出 1 * 2 = 2

							return 0;
						}

			6.类似C#中的变长参数:
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

				3.通过宏进行静态函数或者普通函数的定义:
					当使用#define时如果内容为空则视为无内容,否则可以作为一种判断标准
					#define F_NONE 
					#define F_STATIC static
					class Myclass
					{
						public:
							int F_NONE func(){}		//define 没有赋值,则当作普通函数
							int F_STATIC func2(){} 	//define 被赋值,则会改为静态函数
					};
				4.令牌拼接:
					对于宏定义可以使用类似占位符的方法,进行拼接:
						#define Func(your_name) struct name##__ {...};
						这样的一个宏定义,在使用Func(参数);传入的参数就会替换掉name##__
							比如:
								func(anyway)
								main
								{
									anyway__ mystruct;
									...
								}
								这样声明是有效的.
							此外在令牌拼接时,name##__中的__并不是非得是__也可以是其它的,但不能是空:
								#define func(name) struct name##_vec {int a;};
								func(anyname)
								main
								{
									anyname_vec mystruct;
									...
								}
						如果对其重命名为指针类型如:
							#define Func(your_name) struct name##__ {...};typedef name##__* your_name;
							在使用func(anyname);创建一个结构体时;
							main
							{
								//这里就不需要加__或者其它的例如上述的_vec
								//直接使用anyname,相当于创建一个结构体的指针
								anyname my_struct; //等价于 anyname__* my_struct;
								...
							}
							注意一点就是typedef name##__* your_name;是name##__*(类型) + your_name(变量名)
					


		->#头文件专栏#:
			1.头文件前向声明:
				当头文件A.hpp中定义了B.hpp文件,并且在B.hpp文件中又包含了A.hpp文件,当main.cpp文件包含了这两个其中一个文件,都会导致循环定义,最后冲突无法正确构建:
				所以需要使用前向声明改变这种冲突:
				
				对于前向声明的其他功能:
					因为标准c++编程中,如果有一个类中使用一个函数,而函数是在class类后声明,就会导致,无法找到.
					尤其是如果函数里也包含了该类,则会进入进退两难的地步.
						class vec3
						{
							void func(){dot();}
						}

						void dot(vec3 v){}
					所以函数放到前面,使用前向声明class类:
						class vec3;
						void dot(vec3 v){}

						class vec3
						{
							void func(){dot();}
						}


			2.通过define,如果包含一个特殊宏定义,该头文件内的方法才可以使用:
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
			3.头文件,多宏包含判断:
				#if defined(宏定义1) && defined(宏定义2)
				#endif



		->#修饰符专栏(关键字)#:
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

				2.const变量:
					对于const变量,为保证该变量不在之后的逻辑中被修改,而添加const修饰.此外,由const修饰的变量,也称为常量,如一个自定义类型常量不只调用非const成员函数,只能调用const成员函数;
				3.const为复杂类型的开头保护函数返回值不被修改:
					const vec x(vec test) const
					{
						return test
					} 
					这时候返回值为自定义类型<vec>,
						如果我现在有变量vec temp;   和   vec temp2;
						调用参数x(temp);这里会返回一个vec类型的数据,即temp
						如果我现在令x(temp) = temp2;就会因为const保护导致报错.
					如果我没有const头作为保护
						对于x(temp) = temp2;并不会出现任何问题,temp最终会等于temp2;
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
			-inline 内联关键字:
				首先有一个重点,一般来说,inline的函数的声明和实现必须在一起,即在hpp中使用inline必须在hpp中声明而不能在cpp中声明.
				对于有一些类中函数声明在hpp但是实现在cpp,且main.cpp包含的是hpp文件,不报错是因为编译器原则,原有的版本或者不同系统下,会忽视inline
				即:编译器对 inline 的 “建议性” 处理 或 函数未被实际调用

						
		->#指针专栏#:
			-所谓指针,就是主动创建的一份内存空间,将地址传递给指针,让该指针操作该空间的内容.
				指针的理解(地址理解):
					int val=10; 
					int * point = new int; 
					point = &val; 
					首先,指针初始化本身也占用一块内存地址,因此,指针本身也有地址一说 printf("%p\n",point); 
					这里表示point指向的那块值的地址 printf("%p\n",&point); 
					这里表示point指针自己的地址. printf("%d\n",*point); 
					这里表示对指针指的那块地址的解引用,即获取该地址的值 printf("%p\n",*&num); 
					这里表示先获取指针自身的地址,再对自身指针地址解引用,获得指针指向的那个地址,并不是解引用为值,最终表示的是val的地址

			-注意:指针本身是一个存储地址的变量,它默认会存储在栈区,而当使用new创建一个变量的时候,对应的变量会保存在堆区,而指针依旧在栈区,只是指向着堆区对应的数据.

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

			7.函数指针:
				首先,函数也有地址,可以使用输出流查看如:printf("%p" , func);或printf("%p" , &func);
				其次,当想让函数成为参数可以使用函数指针或使用include<funcitional>库;
					1.创建函数指针:
						声明一个函数:
							int function(int a, int b,...){return a+b;}
						声明对应的函数指针:
							int(*p_func)(int,int) = function;
							这里需要注意,第一部分为返回值类型,第二部分为指针名称,第三部分为函数参数类型;
							其返回类型,和参数类型需要一一对应;
						使用:
							printf("%d",p_func(1,2));即可输出函数中的逻辑,即1+2=3;
				通过函数指针,使用函数作为参数:
					方法1:
						template<typename T>
						T Sum_er(unsigned int index_variable , T val ,int(*func)(unsigned int,int))
						{
							return func(index_variable, val);
						}
						其中:int(*func)(unsigned int,int)就是函数指针,func是函数名,各部分与上相同,传入的函数需要与之对应
						此外如果不加*:int(func)(unsigned int,int);也可以,这是c++的语法糖,会隐式转换.
					方法2:
						template<typename T>
						T Sum_er(unsigned int index_variable , T val ,std::function<T(unsigned int,T)> func)
						{
							return func(index_variable, val);
						}
						该方法需要引入头文件include<funcitional>,std::function<T(unsigned int,T)> func的各部分也需要与上相同
					使用:
						main
						{
							T result = Sum_er(1,2 , func);//直接使用该函数名称即可,函数名本身就对应指针,保存存储其逻辑的地址
							...
						}

		-#STL模板库#
			-std::optional<...> :
				这是一个只接受一个输入值的模板容器,
				当初始化std::optional<int> number;之后它为空,调用number.has_value();返回false
				当number = 1;赋予一个值之后,这个模板就为非空状态
				此时调用number.has_value();返回true.
			

		->#数据结构专栏#
			1.内存对齐:
				什么是内存对齐:
					内存对齐是指,一个程序在初始化的时候,对于数据存入内存中,会以上一个数据字节大小的整数倍进行存放:
				对于自定义类型struct举例(class同理):
					当我创建一个struct name
							{
								int id;			//4	
								string name;	//32	为4个8字节的组合,对齐值为8,总大小一般来说默认是32
								bool gender;	//1
							}
					这个结构体只看其内容各值好像是4+32+1=37;
					但不是.
					对于内存对齐上来讲:
						从第0个空间开始
						id为4字节,所用空间为0-3;满足其对齐值
						names的对齐值为8,应该从第八个整数倍的空间开始,所以会空出来4个字节的地址空间,即8-39(因为8开始,从第8个到第39个一共有32个);
						gender的对齐值为1,任意区域都可以存放
						所以总共是4 + 4 + 32 + 1 = 41;但内存对齐,内存大小需要是结构体数据最大的对齐值的整数倍,41不是,所以需要增加7为48
		->#文件读取:
			使用C风格的文件读取:
				1.实例化文件对象
					FILE * file = fopen(path , "打开方式");
						"打开方式"是指"rb"为可读可写
				
				2.获取txt文件字符数:
					long size;
					首先将实例化的文件对象的光标移动到最后:
						fseek(file,SEEK_END);
					使用ftell获取:
						size = ftell(file);
						(ftell的机制是:相对于文件起始位置的偏移量);
				
					
		->#优化思路#
			内存优化:
				1.在内存拷贝中,有一些类似自定义结构体的数据,他需要传递给另一个结构体的时候,其实并不需要将整个结构体传递过去,而是可以传递该结构体的地址即可:
					例如以下例子:
						这是一个vulkan创建Application Info 和Create Info的例子,通过创建A和B之后,B会携带A的信息,最后通过一个函数创建真正的Vulkan实例.
						但B携带A的时候并不是将结构体的数据传递给B,而是地址,这样节省了一个结构体的字节空间
						相对的损耗的则是寻址时间消耗和一个额外的地址指针字节空间
						此外需要主义的一点就是,这样的创建代表A和B只是临时数据,生命周期并不长

						VkApplicationInfo vk_app_info = {};
						vk_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
						vk_app_info.pNext = nullptr;
						vk_app_info.pApplicationName = "HelloVulkan";
						vk_app_info.apiVersion = VK_API_VERSION_1_0;
						vk_app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
						vk_app_info.pEngineName = "No Engine";
						vk_app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

						// 绑定创建vulkan实例需要的相关信息
						  VkInstanceCreateInfo vk_create_info = {};
						  vk_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
						* vk_create_info.pApplicationInfo = &vk_app_info; 
						  vk_create_info.pNext = nullptr;
						  vk_create_info.enabledExtensionCount = glfw_externs_count_to_vulkan;
						  vk_create_info.ppEnabledExtensionNames = glfw_extens_to_vulkan;
						  vk_create_info.enabledLayerCount = static_cast<uint32_t>(vk_validation_layer.size());
						  vk_create_info.ppEnabledLayerNames = vk_validation_layer.data();

						//其中有加*号的部分就是传递地址代替传递结构体数据.的部分

		-框架分析:
			对于一下例子:
				这是一个Vulkan创建队列族的方法:
					这里想要通过传递this->vk_physical_devices一个指针,最后获得queue_family_count并赋值
					vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, nullptr);
					printf("%d : QueueFamilyCount : %d\n", __LINE__, queue_family_count);

					但是我不明白的是如果只需要获得queue_family_count又何必再去传递vk_physical_devices的指针呢?他本身初始化是一个空指针
					后来我想到,在初始化Vulkan的时候会有一些我看不到的静态全局变量或者其他数据,而传递该vk_physical_devices指针的目的就是将那些全局的数据地址传递给这个指针,以便之后调用.
				 



