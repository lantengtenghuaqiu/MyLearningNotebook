#include<iostream>
#include<CL/cl.h>

#define LOGER
#include"Tools.hpp"

int main()
{
    cl_platform_id platforms;
    cl_uint platformCounts;
    cl_device_id devices;
    cl_uint deviceCounts;

    //获取平台数
    clGetPlatformIDs(0,nullptr,&platformCounts);
    std::cout<<"Platform counts: "<<platformCounts<<std::endl;

    //获取平台
    clGetPlatformIDs(platformCounts,&platforms,nullptr);
    
    //获取设备数
    clGetDeviceIDs(platforms,CL_DEVICE_TYPE_GPU,1,nullptr,&deviceCounts);
    std::cout<<"Device counts: "<<deviceCounts<<std::endl;

    //获取设备
    clGetDeviceIDs(platforms,CL_DEVICE_TYPE_GPU,deviceCounts,&devices,nullptr);

    cl_context_properties properties[]
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platforms,
        0
    };

    cl_context context = clCreateContext(properties,1,&devices,nullptr,nullptr,nullptr);
    // LOG_INFO(context);

    

    return 0;
}