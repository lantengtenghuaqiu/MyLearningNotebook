#include "Constants.hpp"
#include "xyl_vector.hpp"
// #define do_print

#ifdef do_print
#include<stdio.h> 
#define xprint_d(msg) printf("%d",msg);
#else
#define xprint_d(msg);
#endif


#define CreateStruct(name)\
struct name##__\
{\
    unsigned short width = 0;\
    unsigned short height = 0;\
};\
typedef name##__ name;

CreateStruct(Canvas);

const static unsigned int canvas_width = 1280;
const static unsigned int canvas_height = canvas_width / xyl::ratio::ratio_d_16_9;


int main()
{
    Canvas rasterizerCanvas;
    rasterizerCanvas.width = canvas_width;
    rasterizerCanvas.height = canvas_height;
    // xprint_d(rasterizerCanvas.height)
    xyl::coordinate::Point3 p0(3,5,0);

    return 0;
}