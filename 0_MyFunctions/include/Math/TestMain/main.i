# 0 "main.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "main.cpp"




# 1 "../xyl_math.hpp" 1







# 1 "../xyl_return_def_type.hpp" 1
# 9 "../xyl_math.hpp" 2
# 1 "../xyl_vector.hpp" 1






namespace xyl
{
    struct point
    {
        protected:
            float origin[3];
        public:
            float get_point_x()const;

    };


    namespace _vector
    {
        struct vec : public point
        {
            private:



                float magnitude;
                float direction[3];
                float tail[3];
                float head[3];
            public:

                vec(){}

                vec(const float& v1 , const float& v2 , const float& v3)
                {
                    this->origin[0] = v1;
                    this->origin[1] = v2;
                    if(3==3)
                    {
                        this->origin[2] = v3;
                    }
                }
        };
    }



}
# 10 "../xyl_math.hpp" 2



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

    }

    namespace constants
    {
        constexpr double pi = 3.1415926535897932;
        constexpr double ang_to_rad_cofficient = pi / 180.0;

    }


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
    }






    namespace vector
    {
        template<typename T>
        inline float normalize(unsigned int loop_times,T vec)
        {
            return 0.0;
        }

        template<typename T>
        inline float dot(T v)
        {
            if constexpr(xyl::type::is_same<T,xyl::_vector::vec>::value)
            {
                xyl::_vector::vec v3 = v;
                float scale = v3.get_point_x();
                return scale;

            }
            return 0.0;
        }

    }

    namespace trigonometry
    {
        template<typename T>
        inline T AngToRad(T angle)
        {

            return angle * xyl::constants::ang_to_rad_cofficient;
        }

    }


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
    }

}
# 6 "main.cpp" 2




int main()
{


    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,2,xyl::math::basic_sum_general_term_formula));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,2,xyl::math::basic_multi_general_term_formula));




    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::basic_sum_general_term_formula));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::fx_2i));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::fx_2i_multi));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,0,xyl::math::fx_iPlus1));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(100,1,xyl::math::basic_sum_general_term_formula));

    xyl::_vector::vec v(1.0,1.0,1.0);

    printf("%f",xyl::vector::dot(v));
# 49 "main.cpp"
    return 0;
}
