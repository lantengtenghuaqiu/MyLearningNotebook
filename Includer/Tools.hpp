#ifndef Tools
#define Tools

#include<cstdint>
#include<fstream>
#include<iostream>

#define LOG_INFO(msg) std::cerr<<"["<<__FILE__<<" "<<__LINE__<<"]"<<msg<<std::endl;

template<typename T>
inline void V3_LOGE( T a , T b , T c)
{
    std::ofstream loger("log.log");
    loger<<__FILE__<<" : ["<<__TIME__<<"] : "<<a<<"\t"<<b<<"\t"<<c<<"\n";

    loger.close();

}




#endif