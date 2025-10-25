#include "Variables.hpp"

Color_255RGB::Color_255RGB(unsigned char _r , unsigned char _g ,unsigned char _b)
{
    this->r = _r;

    this->g = _g;

    this->b = _b;
}

UV::UV(){}

UV::UV(int u , int v)
{
    this->u = u;
    this->v = v;
}


IMAGEPROPERY::IMAGEPROPERY(int width , int height)
{
    this->uv = UV();
    this->uv.u = width;
    this->uv.v = height;

}

FX_LINEARFUNCTION_INT::FX_LINEARFUNCTION_INT(float k , float a , float b,int width ,int height)
{
    this->img = new IMAGEPROPERY(width , height);
    this->fx = img->uv.v; 
    
    this->k = k;
    this->a = a;
    this->b = b;

    this->f = (int)(k * (img->uv.u - a) + b);
}


