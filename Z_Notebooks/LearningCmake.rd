CMake合集:
	终端命令:
		cmake -B <文件名> 当前目录下配置<文件名>(一般是build)的cmake文件,尤其是第一次配置cmake和修改后cmake要使用,重新配置cmake文件

		cmake -bulid <文件名> 构建cmake配置好的<文件名>(一般是build)文件,进行源文件和main文件以及链接头文件,生成可执行.exe文件
		CMakeLists.txt Code:

	-各参数代表含义:
		${PROJECT_SOURCE_DIR} 包含project(...)的CmakeLists.txt文件所在位置;
			${PROJECT_SOURCE_DIR}/../../X_Includer
				可以表示${PROJECT_SOURCE_DIR}上层的上层中的X_Includer文件夹


	-全局设置头文件include_directorie:
		include_directorie(${PROJECT_SOURCE_DIR}/../../X_Includer)设置所有头文件都在该地址
	-局部设置头文件target_include_directories:
		target_include_directories(head1 PRIVATE ${PROJECT_SOURCE_DIR}/../../X_Includer )
			这样在之后对于cpp文件或者库文件的引用的时候,可以在前声明对应的head
				add_executable(myapp src/main.cpp) 

		//指定编译版本
		set(CMAKE_CXX_STANDARD 17)
		//强制执行该编译版本
		set(CMAKE_CXX_STANDARD_REQUIRE ON)

		#配置生成位置
		#${CMAKE_BINARY_DIR}是build文件夹,但build并不是固定名称,和一开始build cmake文件有关
		set_target_properties(nodefine PROPERTIES
		    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/main
		)

		add_custom_target(
		    run_auto ALL									#自定义名称
		    COMMAND “可执行文件的路径”  		#运行可执行文件
		    DEPENDS 编译文件名 						#依赖于 “编译文件名”构建完成
		    COMMENT "Auto-running myapp..."		#这个是类似注释一样的东西
		)
