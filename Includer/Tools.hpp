#ifndef Tools
#define Tools

// #include<cstdint>
// #include<iostream>
#include<fstream>
#include"Constants.hpp"
#ifdef LOGER
    #include<initializer_list>
#endif

#define LOG_INFO(msg) std::cerr<<"["<<__FILE__<<" : "<<__TIME__<<" : "<<__LINE__<<"] : "<<msg<<std::endl;

template<typename T>
inline void V3_LOGE( T a , T b , T c)
{
    std::ofstream loger("log.log");
    loger<<__FILE__<<" : ["<<__TIME__<<"] : "<<a<<"\t"<<b<<"\t"<<c<<"\n";

    loger.close();
}

template<typename T>
inline void SINGLE_LOGE(T a)
{
    std::ofstream loger("log.log");
    loger<<__FILE__<<" : ["<<__TIME__<<"] : "<<a<<"\n";

    loger.close();
}



#ifdef LOGER
    static std::ofstream LOGER_FILE_STREAM("loger.log",std::ios::app);

    template<typename T>
    inline static void DOLOGE(const std::initializer_list<T> args)
    {   
        if(LOGER_FILE_STREAM.is_open())
        {
            xyl::types::is_same<T,vec3> typeCheck;
            if(typeCheck.value)
            {
                // LOG_INFO("v3 log");
                LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "; 
                #ifdef BINARYREAD
                    for(auto it = args.begin();it != args.end();it++)
                    {   
                        LOGER_FILE_STREAM<<*it;
                        // LOG_INFO(*it);
                    }
                #else
                    for(const vec3& it : args)
                    {
                        LOGER_FILE_STREAM<<it.x()<<"\t\t"<<it.y()<<"\t\t"<<it.z()<<"\n";         
                    }
                    
                #endif

            }
            else
            {
                LOGER_FILE_STREAM<<"["<<__DATE__<<" : "<<__LINE__<<" : "<<__TIME__<<"] : "; 
                for(auto it = args.begin();it != args.end();it++)
                {
                    LOGER_FILE_STREAM<<it<<"\t"<<std::endl;
                }
                LOGER_FILE_STREAM<<"\n"; 
            }
        }
    }

    inline void CloseStreamings(){}
    template<typename ... Args> //这里的...是固定写法
    inline void CloseStreamings(std::ofstream& first , Args&... rest)
    {
        if(first.is_open())
        {    
            LOG_INFO("file is close");
            LOGER_FILE_STREAM<<"\n|---["<<__DATE__<<" : "<<__TIME__<<"]---|\n\n";
            first.close();
        }

        CloseStreamings(rest...);
    }


    namespace xyl
    {
        namespace types
        {
            template<typename T ,typename U>
            struct is_same 
            {
                static constexpr bool value = false;
            };
    

            template<typename T>
            struct is_same<T,T>
            {
                static constexpr bool value = true;
            };
        }

    } // namespace xyl
    

#endif

#ifdef XYLMATH
namespace xyl
{
    namespace math
    {
        template<typename T>
        inline T pow(const T& a, const int b)
        {   
            T result = a;
            if(b>0)
            {
                for(int i = 0 ; i<b-1 ; i++)
                {
                    result = result * a;
                }
            }
            else if(b==0)
            {
                return 1;
            }

            return result;
        }
        
        template<typename T>
        inline T max(const T& a, const T& b)
        {
            return a>b?a:b;
        }
        
        template<typename T>
        inline T min(const T& a , const T& b)
        {
            return a<b?a:b;
        }
        
        inline double DegToRad(double deg)
        {
            return deg * pi / 180.0;
        }

    }

}
#endif

#endif