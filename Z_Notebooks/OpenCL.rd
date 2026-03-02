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


