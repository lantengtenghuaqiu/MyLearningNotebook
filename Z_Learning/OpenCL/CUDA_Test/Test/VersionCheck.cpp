#include <CL/cl.h>
#include <iostream>
#include <cstring>

// 错误检查宏
#define CHECK_ERROR(err) \
    if (err != CL_SUCCESS) { \
        std::cerr << "OpenCL error: " << err << std::endl; \
        exit(EXIT_FAILURE); \
    }

int main() {
    cl_platform_id* platforms;
    cl_uint num_platforms;
    cl_int err;

    // 1. 获取所有平台数量
    err = clGetPlatformIDs(0, nullptr, &num_platforms);
    CHECK_ERROR(err);

    // 2. 分配平台数组并获取平台
    platforms = new cl_platform_id[num_platforms];
    err = clGetPlatformIDs(num_platforms, platforms, nullptr);
    CHECK_ERROR(err);

    // 3. 遍历每个平台，查询OpenCL版本
    for (cl_uint i = 0; i < num_platforms; i++) {
        char platform_name[1024];  // 平台名称（如NVIDIA CUDA）
        char cl_version[1024];     // OpenCL版本信息

        // 获取平台名称
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, nullptr);
        CHECK_ERROR(err);

        // 获取平台支持的OpenCL版本（核心）
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(cl_version), cl_version, nullptr);
        CHECK_ERROR(err);

        // 输出结果
        std::cout << "Platform " << i << " : " << platform_name << std::endl;
        std::cout << "Support versions: " << cl_version << std::endl;
    }

    // 释放资源
    delete[] platforms;
    return 0;
}