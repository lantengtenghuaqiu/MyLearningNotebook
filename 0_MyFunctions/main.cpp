#include "./includes/xyl_math.hpp"
#include <stdio.h>
int main()
{
    xyl::mat4x4<float> vec1(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    
    // printf("%f %f %f", m1.x,m1.coord1.y,m1.coord1.z);

    return 0;
}