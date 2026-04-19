#ifndef GLOBAL_DATAS
#define GLOBAL_DATAS

#include "./BasicIncludes.hpp"
// 全局数据------------------------------------------------

float Color_white[] = {1.0f, 1.0f, 1.0f, 1.0f};

typedef struct TextureAttribute
{
    int width, height, nChanel;
    unsigned int textureLocation;

} TexAttri;



namespace GlobalLocation
{
    struct ShadowMap
    {
        unsigned int _M;
    };

    struct CubeMap
    {
        unsigned int _skybox;
    };
}

#endif