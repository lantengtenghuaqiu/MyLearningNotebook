#ifndef Tools
#define Tools

// #include<cstdint>
// #include<iostream>
#include <fstream>
#include "Constants.hpp"


#ifdef LOGER
#include <initializer_list>
#endif
#ifdef LOGER

    #define LOG_INFO(msg) std::cerr << "[" << __FILE__ << " : " << __TIME__ << " : " << __LINE__ << "] : " << msg << std::endl;

    template <typename T>
    inline void V3_LOGE(T a, T b, T c)
    {
        std::ofstream loger("log.log");
        loger << " : [" << __FILE__ << " : " << __LINE__ << " : " << __TIME__ << "] : " << a << "\t" << b << "\t" << c << "\n";

        loger.close();
    }

    template <typename T>
    inline void SINGLE_LOGE(T a)
    {
        std::ofstream loger("log.log");
        loger << " : [" << __FILE__ << " : " << __LINE__ << " : " << __TIME__ << "] : " << a << "\n";

        loger.close();
    }

    class vec3;
    static std::ofstream LOGER_FILE_STREAM("loger.log", std::ios::app);



    template <typename T>
    inline static void DOLOGE(const std::initializer_list<T> args)
    {
        if (LOGER_FILE_STREAM.is_open())
        {
            xyl::types::is_same<T, vec3> typeCheck;
            if constexpr (typeCheck.value)
            {
                LOGER_FILE_STREAM << "[" << __DATE__ << " : " << __TIME__ << "] : ";
    #ifdef BINARYREAD
                for (auto it = args.begin(); it != args.end(); it++)
                {
                    LOGER_FILE_STREAM << *it;
                }
    #else
                for (const vec3 &it : args)
                {
                    LOGER_FILE_STREAM << it.x() << "\t\t" << it.y() << "\t\t" << it.z() << "\n";
                }

    #endif
            }
            else
            {
                for (const T &it : args)
                {
                    LOGER_FILE_STREAM << "[" << __DATE__ << " : " << __TIME__ << "] : " << it << "\t" << std::endl;
                }
            }
        }
    }

    inline void CloseStreamings() {}
    template <typename... Args> // 这里的...是固定写法
    inline void CloseStreamings(std::ofstream &first, Args &...rest)
    {
        if (first.is_open())
        {
            LOG_INFO("file is closed");
            LOGER_FILE_STREAM << "---[" << __DATE__ << " : " << __TIME__ << "file is closed" << "]---\n";
            first.close();
        }

        CloseStreamings(rest...);
    }
#endif

namespace xyl
{

    namespace types
    {
        template <typename T, typename U>
        struct is_same
        {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_same<T, T>
        {
            static constexpr bool value = true;
        };
    }

    #ifdef XYLMATH
    namespace math
    {
        template <typename T>
        inline T pow(const T &a, const int b)
        {
            T result = a;
            if (b > 0)
            {
                for (int i = 0; i < b - 1; i++)
                {
                    result = result * a;
                }
            }
            else if (b == 0)
            {
                return 1;
            }

            return result;
        }

        template <typename T>
        inline T max(const T &a, const T &b)
        {
            return a > b ? a : b;
        }

        template <typename T>
        inline T min(const T &a, const T &b)
        {
            return a < b ? a : b;
        }

        
        template <typename T>
        inline bool surrounds(const T &x, const T &min, const T &max)
        {
            return min < x && x < max;
        }
        
        class vec3;
        template <typename K, typename T>
        inline K clamp(const K &val, const T &min, const T &max)
        {
            if constexpr(xyl::types::is_same<K,vec3>::value)
            {
                vec3 val_temp = val;
                val_temp.x() = val_temp.x()<min ? min : val_temp.x > max ? max:val_temp.x();
                val_temp.y() = val_temp.y()<min ? min : val_temp.y > max ? max:val_temp.y();
                val_temp.z() = val_temp.z()<min ? min : val_temp.z > max ? max:val_temp.z();
                return val_temp;
            }
            
            if constexpr (xyl::types::is_same<K,T>::value)
            {
                if (val < min)
                return min;
                if (val > max)
                return max;
            }
            
            return val;
        }
        
        template <typename T>
        inline T random_range(const T &min, const T &max)
        {
            // 这是通过随机值归一化[0,1]来获得最小为min,最大为(max-min) * [0,1]的值
            return min + ((max - min) * random_double_normalized());
        }
        
        inline double DegToRad(const double &deg);
        inline double random_double_normalized(void);
    }
    #endif

    inline unsigned int strlen(const char* str,unsigned int max);

    namespace error_checker
    {
        enum class XTYPES
        {
            SUCCESS = 0
        };
    }
}

namespace xyl
{

}



#endif