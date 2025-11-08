#ifndef LOGER_H
#define LOGER_H

#include "Tools.hpp"
#include "Variables.hpp"
#include <initializer_list>
#include <fstream>

namespace xyl
{
    namespace log
    {
        #define LOG_INFO(msg) std::cerr << "[" << __FILE__ << " : " << __TIME__ << " : " << __LINE__ << "] : " << msg << std::endl;
        
        static std::ofstream LOGER_FILE_STREAM("loger.log");//, std::ios::app

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
                // LOGER_FILE_STREAM << "---[" << __DATE__ << " : " << __TIME__ << "file is closed" << "]---\n";
                first.close();
            }

            CloseStreamings(rest...);
        }

    }
}

#endif