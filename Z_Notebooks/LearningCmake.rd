CMake合集:
	->#终端命令:
		cmake -B <文件名> 当前目录下配置<文件名>(一般是build)的cmake文件,尤其是第一次配置cmake和修改后cmake要使用,重新配置cmake文件

		cmake -bulid <文件名> 构建cmake配置好的<文件名>(一般是build)文件,进行源文件和main文件以及链接头文件,生成可执行.exe文件
		CMakeLists.txt Code:

	->#各参数代表含义:
		${PROJECT_SOURCE_DIR} 包含CmakeLists.txt的文件的根目录绝对位置;
			${PROJECT_SOURCE_DIR}/Includer
				可以表示${PROJECT_SOURCE_DIR}下的Includer文件夹

			${PROJECT_SOURCE_DIR}/../../X_Includer
				可以表示${PROJECT_SOURCE_DIR}上层的上层中的X_Includer文件夹
		
		#${CMAKE_BINARY_DIR} 是build文件夹的地址,但build并不是固定名称,和一开始build cmake文件有关


	->#全局设置头文件include_directorie:
		include_directorie(${PROJECT_SOURCE_DIR}/../../X_Includer)设置所有头文件都在该地址
	
	->#局部设置头文件target_include_directories:
		target_include_directories(head1 PRIVATE ${PROJECT_SOURCE_DIR}/../../X_Includer )
			这样在之后对于cpp文件或者库文件的引用的时候,可以在前声明对应的head
				add_executable(myapp src/main.cpp) 

	->#指定编译版本:
		set(CMAKE_CXX_STANDARD 17)
	->#强制执行该编译版本:
		set(CMAKE_CXX_STANDARD_REQUIRE ON)
	->#关闭GNU扩展:
		set(CMAKE_CXX_EXTENSIONS OFF)
	
	->#设置需要编译的项目文件(需要精确到文件名和后缀):
		set(SOURCES
			#需要编译的文件1:
			${PROJECT_SOURCE_DIR}/../main.cpp
			#需要编译的文件1:
			${PROJECT_SOURCE_DIR}/../glad.c
		)
		(注意:SOURCES是自拟的名称)

	->#设置需要编译的项目:
		add_executable(name ${SOURCE})

	->#配置输出位置和名称
		set_target_properties(name PROPERTIES
		    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/main
		)

		add_custom_target(
		    run_auto ALL									#自定义名称
		    COMMAND “可执行文件的路径”  		#运行可执行文件
		    DEPENDS 编译文件名 						#依赖于 “编译文件名”构建完成
		    COMMENT "Auto-running myapp..."		#这个是类似注释一样的东西
		)

	->#设置头文件目录
		#设置路径:
		set(includes
			${PROJECT_SOURCE_DIR}/../includes
		)
		#设置头文件路径:
		target_include_directories(name ${includes})

	

->#C++的Cmake示例:
	#最低Cmake版本要求
	cmake_minimum_required(VERSION 3.10)

	#项目信息(名称 版本 支持的语言)
	project(HelloOpenGL LANGUAGES CXX)
		#名称:
			#用于创建可执行时指定哪个项目,名称唯一.
		#版本:
			#自拟,如
				# 写法1：语义化版本（推荐）
				#project(MyApp VERSION 1.2.3 LANGUAGES CXX)

				# 写法2：仅主版本+次版本
				#project(MyApp VERSION 2.0 LANGUAGES CXX)

				# 写法3：自定义格式（不推荐，但合法）
				#project(MyApp VERSION v1.0-beta LANGUAGES CXX)
		#命名的项目构建支持的语言:
			#指HellOpenGL作为命名的项目,为他的构建,指定支持的语言,CXX指cpp

	#设置C++标准的版本        
	set(CMAKE_CXX_STANDARD 17)
	#设置对应的编译器(g++/clang++)是否强制支持该版本的标准
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	#禁用GNU扩展,严格遵循标准
	set(CMAKE_CXX_EXTENSIONS OFF)
		#GNU扩展是指,为C/C++语言增加的非标准特性,一般是用来提升性能或简化开发的.
		#这些扩展特性不属于C/C++标准.有些能被gcc/g++/clang等兼容,有些则不能,则需要其他编译器.
		#而GNU扩展仅被gcc/clang支持,而MSVC(Visual Studio编译器则不能)等非GNU编译器会报错.

	#收集源文件:
	set(SOURCES 
		${PROJECT_SOURCE_DIR}/project/main.cpp
	)
		#这里只是收集,类似SOURCES是一个变量,而./main.cpp 和 ../src/glad.c是值.

	#为创建的项目,创建可执行目标:
	add_executable(HelloOpenGL ${SOURCES})
		#项目名称(HelloOpenGL):
			#指定需要生成可执行目标的,是哪个项目.
		#源文件组名(SOURCES):
			#制定项目生成的可执行文件,所需要的源文件.

	#设置文件的输出路径(项目级):
	set_target_properties(HelloOpenGL PROPERTIES
		#设置输出文件的路径
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/project
		#设置输出文件的名称
		OUTPUT_NAME "HelloOpenGL"
	)

	#设置头文件:
	set(INCLUDES
		${PROJECT_SOURCE_DIR}/include
	)    
	#指定编译需要的头文件:
	target_include_directories(HelloOpenGL PRIVATE ${INCLUDES})

	#设置库路径
	set(LIBS
		${PROJECT_SOURCE_DIR}/libs
	)

	#设置项目目标的库地址(如果要设置全局地址link_directories(...)需要在add_execute前):
	target_link_directories(HelloOpenGL PRIVATE ${LIBS})

	#设置库文件名称(自动补全lib*.a)
	set(LINK
		abc
		def
	)

	#指定编译需要的库文件:
	target_link_libraries(HelloOpenGL PRIVATE abc def)

	#message用于输出信息到console
	# message(STATUS "Path:  ${PROJECT_SOURCE_DIR}/libs")

	# 在 add_executable 之后添加
	set_target_properties(HelloOpenGL PROPERTIES
		LINK_FLAGS "-v"  # 打印链接器的完整命令和查找路径
	)