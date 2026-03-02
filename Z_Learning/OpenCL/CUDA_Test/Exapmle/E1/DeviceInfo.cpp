#include <iostream>
#include <CL/cl.hpp>
#include <fstream>
#define FILENAME DeviceInfos.log
extern "C" void func(void* ss ,size_t len,const char* v){
    char* a = (char*)ss;
    for(int i = 0 ; i<len;i++)
    {
        a[i] = v[i];
    }
}
int main()
{
    std::cout<<"Hello World"<<std::endl;
    std::ofstream file("DeviceInfos.log");
    
    cl_uint errs;
    cl_uint platformCouts;

    errs = clGetPlatformIDs(0,nullptr,&platformCouts);
    file<<"Platform couts : "<<platformCouts<<std::endl;
    file<< "-------------------------" << std::endl;

    if(platformCouts == 1)
    {
        cl_platform_id platformsIDs;
        clGetPlatformIDs(platformCouts,&platformsIDs,nullptr);
        std::cerr<<"Platforms RAM addresse : "<<platformsIDs<<std::endl;

        size_t name_len;
        std::string platformName;

        //获取名字
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_NAME,0,NULL,&name_len);
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_NAME,name_len,platformName.data(),nullptr);
        file<<"Platform Name: "<<platformName.data()<<std::endl;

        //获取VENDOR长度
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_VENDOR,0,NULL,&name_len);
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_VENDOR,name_len,platformName.data(),nullptr);
        file<<"VENDOR Name: "<<platformName.data()<<std::endl;

        //获取Version
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_VERSION,0,NULL,&name_len);
        clGetPlatformInfo(platformsIDs,CL_PLATFORM_VERSION,name_len,platformName.data(),nullptr);
        file<<"VERSION Name: "<<platformName.data()<<std::endl;


    std::string val = "befor content";
    std::string afterVal = "Changed Values";
    val.resize(afterVal.capacity());
    func(val.data(),afterVal.capacity(),afterVal.data());
    std::cout<<val<<std::endl;

        cl_uint deviceCouts;
        cl_device_id devcieIDs;
        clGetDeviceIDs(platformsIDs,CL_DEVICE_TYPE_ALL,0,nullptr,&deviceCouts);
        std::cerr<<"Devices cout : "<<deviceCouts<<std::endl;
        clGetDeviceIDs(platformsIDs,CL_DEVICE_TYPE_ALL,0,&devcieIDs,nullptr);

    //     if(deviceCouts == 1)
    //     {

    //         errs = clGetDeviceIDs(platformsIDs,CL_DEVICE_TYPE,deviceCouts,&devcieIDs,nullptr);
    //         std::cerr<<"Devcie RAM addresses : "<<devcieIDs<<std::endl;
    //     }
    }


    


    return 0;
}