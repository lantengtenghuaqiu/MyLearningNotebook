#include<iostream>
#include<CL/cl.h>


int main()
{
    cl_platform_id platform;
    cl_uint platformCounts;
    cl_platform_id platforms;

    clGetPlatformIDs(0,nullptr,&platformCounts);

    std::cout<<"Platform counts: "<<platformCounts<<std::endl;

    

    return 0;
}