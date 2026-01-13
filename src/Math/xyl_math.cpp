#include"xyl_math.hpp"

int xyl::math::basic_sum_general_term_formula(unsigned int a, int b)
{
    int temp=b;
    for(unsigned int i = b+1 ; i<=a ; i++){temp+=i;}
    return temp;
}

int xyl::math::fx_2i(unsigned int loop_times, int number)
{
    int temp = 2*number;
    for(unsigned int i = number+1 ;i<=loop_times;i++)
    {
        temp +=(2*i);
    }
    return temp;

}

int xyl::math::basic_multi_general_term_formula(unsigned int a, int b)
{
    int temp=b;
    for(unsigned int i = b+1 ; i<=a ; i++){temp*=i;}
    return temp;
}

int xyl::math::fx_2i_multi(unsigned int loop_times, int num)
{
    int temp = 2*num;

    for(unsigned int i = num + 1 ; i<=loop_times ; i++)
    {
        temp *= (2*i);
    }
    
    return temp;
}

int xyl::math::fx_iPlus1(unsigned int loop_times, int num)
{
    int temp = 7*(1+num);

    for(unsigned int i = num + 1; i<=loop_times ;i++)
    {
        temp = temp * (7*(i+1));
    }

    return temp;
}   
