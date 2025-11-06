#include<iostream>
#include <fstream>

#define FILENAME "LOGER.log"
#define CL_TARGET_OPENCL_VERSION 300

#include<CL/cl.h>

#define SIZE_FLOAT sizeof(float)
#define SIZE_INT sizeof(int)
#define SIZE_DOUBLE sizeof(double)
#define SIZE_CHAR sizeof(char)

#define ERRORS(err)\
                if(err != CL_SUCCESS)\
                {\
                    char buffer[2048];\
                    std::cout<<"Wrong: "<<__LINE__<<std::endl;\
                    exit(EXIT_FAILURE);\
                }\

#define LOG(msg) std::cout<<__LINE__<<" : "<<msg<<std::endl;

const char* add = R"(
        __kernel void Add(__local const float* inputa,__local const float* inputb , __global float* output)
        {
            int id = get_global_id(0);
            output[id] = inputa[id] + inputb[id];
        }
    )";

unsigned int strlen2(const char* str,unsigned int max);

unsigned int const LENGTH = 1000;
float* number1 = new float[LENGTH];
float* number2 = new float[LENGTH];

void RandomVal(float* num , unsigned int len) 
{
    for(int i = 0 ; i < LENGTH ;i++)
    {
        num[i] = (float)rand()/100.0;
    }
}

int main()
{
    std::ofstream file(FILENAME);

    RandomVal(number1,LENGTH);
    RandomVal(number2,LENGTH);

    cl_uint length;
    cl_int err;
    size_t size;
    std::string name;

    cl_platform_id platformIDs;

    clGetPlatformIDs(0,nullptr,&length);
    std::cout<<length<<std::endl;
    clGetPlatformIDs(length,&platformIDs,nullptr);

    clGetPlatformInfo(platformIDs,CL_PLATFORM_NAME,0,nullptr,&size);

    clGetPlatformInfo(platformIDs,CL_PLATFORM_NAME,size,name.data(),nullptr);
    std::cout<<name.data()<<std::endl;

    cl_device_id deviceIDs;
    clGetDeviceIDs(platformIDs,CL_DEVICE_TYPE_GPU,0,nullptr,&length);
    std::cout<<length<<std::endl;
    clGetDeviceIDs(platformIDs,CL_DEVICE_TYPE_GPU,length,&deviceIDs,nullptr);
    
    clGetDeviceInfo(deviceIDs,CL_DEVICE_NAME,0,nullptr,&size);
    LOG(size);
    clGetDeviceInfo(deviceIDs,CL_DEVICE_NAME,size,name.data(),nullptr);
    LOG(name.data())


    cl_context_properties properties[]
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platformIDs,
        0
    };

    cl_context context = clCreateContext(properties,1,&deviceIDs,nullptr,nullptr,&err);
    ERRORS(err);

    cl_command_queue queue = clCreateCommandQueueWithProperties(context,deviceIDs,0,&err);
    ERRORS(err);
    size_t codesize = strlen(add,0)+1;

    cl_program program = clCreateProgramWithSource(context,1,&add,nullptr,&err);
    ERRORS(err);




    err=clBuildProgram(program,1,&deviceIDs,nullptr,nullptr,nullptr);

    if(err != CL_SUCCESS)
    {
        char buffer[2048];
        
        std::cout<<"clBuildProgram Wrong: "<<__LINE__<<" : "<<clGetProgramBuildInfo(program,deviceIDs,CL_PROGRAM_BUILD_LOG,2048,buffer,nullptr)<<std::endl;
        return EXIT_FAILURE;
    }

    cl_kernel kernel = clCreateKernel(program,"Add",&err);
    ERRORS(err);

    cl_mem number1Buffer = clCreateBuffer(context,CL_MEM_READ_ONLY,LENGTH*SIZE_FLOAT,nullptr,&err);
    ERRORS(err);

    cl_mem number2Buffer = clCreateBuffer(context,CL_MEM_WRITE_ONLY,LENGTH*SIZE_FLOAT,nullptr,&err);
    ERRORS(err);


    delete(number1);
    file.close();
    return 0;
}

unsigned int strlen2(const char* str,unsigned int max)
{
    unsigned int index = 0;
    if(max>0)
    {
        while(str[index]!='\0' && index<max)
        {
            index+=1;
        }
        return index;
    }
    else
    {
        while(str[index]!='\0')
        {
            index+=1;
        }
        return index;
    }
}
