#ifndef COMPONENTS
#define COMPONENTS
#include "./Data.hpp"

struct ColorComponent
{
    union
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        float rgba[4];
    };

    ColorComponent &operator=(const Vec4 color)
    {
        this->r = color.x;
        this->g = color.y;
        this->b = color.z;
        this->a = color.w;

        return *this;
    }
};
struct ImageComponent
{
    union
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
};
#endif