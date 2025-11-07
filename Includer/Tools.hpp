#ifndef Tools
#define Tools

// #include<cstdint>
// #include<iostream>
#include <fstream>
#include<cstdlib>
#include "Constants.hpp"



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
    namespace math
    {
        double DegToRad(const double &deg);
        double random_double_normalized(void);
        int floor (const int& value);
        int ceil(const int& value);

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
        
        template <typename K>
        inline K clamp(const K &val, const K& min, const K& max)
        {

            if (val < min)
                return min;
            if (val > max)
                return max;
            return val;
        }
        
        template <typename T>
        inline T random_range(const T &min, const T &max)
        {
            // 这是通过随机值归一化[0,1]来获得最小为min,最大为(max-min) * [0,1]的值
            return min + ((max - min) * random_double_normalized());
        }
        

    }

    unsigned int strlen(const char* str,unsigned int max);

    namespace error_checker
    {
        enum class XTYPES
        {
            SUCCESS = 0
        };
    }
}




#endif