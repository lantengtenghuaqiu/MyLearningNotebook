// #include<iostream>
// #include<stdio.h>

#define XYL_FLOAT
#include"xyl_math.hpp"




int main()
{

    //xyl::math sumer
    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,2,xyl::math::basic_sum_general_term_formula));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,2,xyl::math::basic_multi_general_term_formula));

    // std::cout<<add(1,2);
    // std::cout<<add(1,2);

    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::basic_sum_general_term_formula));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::fx_2i));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(5,1,xyl::math::fx_2i_multi));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(4,0,xyl::math::fx_iPlus1));
    printf("%d\n",xyl::math::Sum_Multi_er<int>(100,1,xyl::math::basic_sum_general_term_formula));

    xyl::_vector::vec v(1.0,1.0,1.0);

    printf("%f",xyl::vector::dot(v));
    //xyl::cartesian
    // int x_axis_length = 80;
    // int y_axis_length = 80;

    // int x= -1 * (x_axis_length / 2);
    // int y =-1 * (y_axis_length / 2);
    // printf("(%d,%d)  ",x,y);

    // for(int i=0;i<x_axis_length;i++)
    // {
    //     x += 1;
    //     for(int j = 0 ; j<y_axis_length ; j++)
    //     {
    //         y += 1;
    //         printf("(%d,%d)  ",x,y);
            
    //     }
    //     y =-1 * (y_axis_length / 2);
    // }

    return 0;
}