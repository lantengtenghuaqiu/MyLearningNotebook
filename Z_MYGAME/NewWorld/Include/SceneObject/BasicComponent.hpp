#ifndef BASICCOMPONENT
#define BASICCOMPONENT
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
};

#endif