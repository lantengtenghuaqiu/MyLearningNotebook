#ifndef XYLMATH_H
#define XYLMATH_H


// #include<initializer_list>
// #include<functional>
// #include<cmath>
#include"xyl_return_def_type.hpp"
#include"xyl_vector.hpp"



namespace xyl
{   
    namespace type
    {
        template<typename T,typename U>
        struct is_same
        {
            static constexpr bool value = false;
        };

        template<typename T>
        struct is_same<T,T>
        {
            static constexpr bool value = true;
        };

    } // namespace type

    namespace constants
    {
        constexpr double pi = 3.1415926535897932;
        constexpr double ang_to_rad_cofficient = pi / 180.0;  

    } // namespace constants
    

    namespace math
    {
        template<typename T>
        T Sum_Multi_er(unsigned int index_variable , T val , T(*general_term_formula)(unsigned int , int))
        {
            return general_term_formula(index_variable, val);
        }

        int basic_sum_general_term_formula(unsigned int a ,int b);

        int fx_2i(unsigned int loop_times ,int number);
 

        int basic_multi_general_term_formula(unsigned int a ,int b);


        int fx_2i_multi(unsigned int loop_times , int num);


        int fx_iPlus1(unsigned int loop_times , int num);
     

        template<typename T>
        inline T pow(const T& val ,const int& loop_time)
        {
            T result = val;
            for(int i = 0 ; i<loop_time-1 ; i++)
            {
                result *= result;
            }
            return result;
        }
    } // namespace math
    // namespace vector
    // {




    namespace vector
    {
        template<typename T>
        inline VEC_TYPE normalize(unsigned int loop_times,T vec)
        {
            return 0.0;
        }

        template<typename T>
        inline VEC_TYPE dot(T v)
        {            
            if constexpr(xyl::type::is_same<T,xyl::_vector::vec>::value)
            {
                xyl::_vector::vec v3 = v;
                float scale = v3.get_point_x();
                return scale;

            }
            return 0.0;
        }

    } // namespace vector
    
    namespace trigonometry
    {
        template<typename T>
        inline T AngToRad(T angle)
        {
            
            return angle * xyl::constants::ang_to_rad_cofficient;
        }

    } // namespace trigonometry

    
    namespace tools
    {
        
        template<typename T>
        inline T floor(const T& value)
        {
            return value - (value % 1);
        }

        template<typename T>
        inline T ceil(const T& value)
        {
            return value + (1 - (value % 1));
        }
    } // namespace tools
    
} // namespace xyl




#endif