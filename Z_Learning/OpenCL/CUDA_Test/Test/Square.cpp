#include <iostream>

#include<CL/cl.h>

#include<vector>

#include<fstream>

#define CHECK_ERROR(err) \
    if(err != CL_SUCCESS){ \
        std::cerr<<"OpenCL error at line "<<__LINE__<<" : "<<err<< std::endl; \
        exit(EXIT_FAILURE); \
    }

void KernelError(cl_int err,cl_program program, cl_device_id device, const char* kernel_core);


int main()
{
    std::ofstream file("log.log",std::ios::app);

    cl_platform_id platform;
    cl_device_id device;
    cl_int err;
    
    //get the first of usable platform.
    err = clGetPlatformIDs(1,&platform,nullptr);
    CHECK_ERROR(err);

    err = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,nullptr);
    CHECK_ERROR(err);


    //2.创建计算环境:上下文(context)和命令队列(Command queue)
    //上下文:管理设备和内存的"容器"
    cl_context context = clCreateContext(nullptr,1,&device,nullptr,nullptr,&err);
    CHECK_ERROR(err);

    //命令队列:
    // cl_command_queue queue=clCreateCommandQueue(context,device,0,&err);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);
    CHECK_ERROR(err);

    std::cout<<"Hello World"<<std::endl;

    //3.定义输入数据和内核代码(并行计算逻辑)
    //输入数组(长度为8)

    std::vector<float> input = 
    {
        1.0f,5.0f,9.0f,8.0f,4.0f,2.0f,7.0f,6.0f
    };

    const int capacity = input.size();

    std::vector<float> output(capacity);

    //OpenCL的内核代码(字符串形式计算)
    const char* kernel_code = R"(
        __kernel void square(__global const float* input, __global float* output)
        {
            int id = get_global_id(0);
            output[id] = input[id] * input[id];
        }
    )";

    //4.编译内核:创建程序对象并编译:
    //创建程序对象:
    cl_program program =  clCreateProgramWithSource(context,1,&kernel_code,nullptr,&err);
    CHECK_ERROR(err);

    err = clBuildProgram(program,1,&device,nullptr,nullptr,nullptr);
    //如果编译失败输出错误
    KernelError(err,program,device,kernel_code);

    //从程序中创建内核对象:
    cl_kernel kernel = clCreateKernel(program,"square",&err);
    CHECK_ERROR(err);


    //5.分配设备内存(创建缓冲区):
    //创建输入缓冲区(CL_MEM_READ_ONLY 设备只读):
    cl_mem inputBuffer = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        capacity * sizeof(float),
        input.data(),
        &err
    );
    CHECK_ERROR(err);

    //创建输出缓冲区(CL_MEM_WRITE_ONLY 设备只写):
    cl_mem outputBuffer = clCreateBuffer(
        context,
        CL_MEM_WRITE_ONLY,
        capacity * sizeof(float),
        nullptr,
        &err
    );
    CHECK_ERROR(err);

    //设置内核参数并执行:
                            //缓冲区0
    err = clSetKernelArg(kernel,0,sizeof(cl_mem),&inputBuffer);
    CHECK_ERROR(err);
                            //缓冲区1
    err = clSetKernelArg(kernel,1,sizeof(cl_mem),&outputBuffer);
    CHECK_ERROR(err);
    
    //定义工作项数量(全局工作大小:8个 与数组长度一致):
    size_t global_work_size = capacity;

    //执行内核(将命令加入队列,有设备运行)
    err = clEnqueueNDRangeKernel(
        queue,
        kernel,
        1,  //1维工作空间(对应数组维度)
        nullptr,//全局工作偏移(默认为0);
        &global_work_size,
        nullptr,
        0,      //本地工作大小,nullprt表示自动分配
        nullptr,
        nullptr
    );
    // CHECK_ERROR(err);


    //7.从设备读取结果到主机内存
    err = clEnqueueReadBuffer(
        queue,
        outputBuffer,
        CL_TRUE,//阻塞读取
        0,//读取偏移
        capacity * sizeof(float),
        output.data(),
        0,
        nullptr,
        nullptr
    );
    CHECK_ERROR(err);

    // std::cout<<"Input array: ";
    // for(float x : input)
    // {
    //     std::cout<<x<<" ";
    // }
    // std::cout<<std::endl;

    std::cout<<"Output array: ";
    for(float x : output)
    {
        file<<x<<" "<<"\n";
    }
    std::cout<<std::endl;


    // 释放资源（按创建的逆序释放）
    clReleaseMemObject(inputBuffer);
    clReleaseMemObject(outputBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

void KernelError(cl_int err,cl_program program, cl_device_id device, const char* kernel_core)
{
    if(err != CL_SUCCESS)
    {
        size_t log_size;
        clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,0,nullptr,&log_size);

        std::vector<char> log(log_size);

        clGetProgramBuildInfo(program,device,CL_PROGRAM_BUILD_LOG,log_size,log.data(),nullptr);

        std::cerr<<"Kernel programing error"<<log.data()<<std::endl;
        exit(EXIT_FAILURE);
    }

}