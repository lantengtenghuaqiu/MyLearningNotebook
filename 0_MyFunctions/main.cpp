#include "./includes/xyl_math.hpp"
#include <stdio.h>
int main()
{
    xyl::float4 vec1(1.0, 2.0, 1.0, 3.0);
    xyl::float4 vec2(2.0, 1.0, 5.0, 3.0);
    xyl::float4 vec3(1.0, 7.0, 1.0, 3.0);
    xyl::float4 vec4(-1.0, 2.0, 1.0, 3.0);

    xyl::mat4x4<float> m1(vec1, vec2, vec3, vec4);
    printf("%f %f %f", m1.x,m1.coord1.y,m1.coord1.z);

    return 0;
}