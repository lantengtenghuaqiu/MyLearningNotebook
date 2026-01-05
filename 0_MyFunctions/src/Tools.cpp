#define XYLMATH
#include"Tools.hpp"

double xyl::math::DegToRad(const double &deg)
{
    return deg * (xyl::consts::pi / 180.0);
}

double xyl::math::random_double_normalized(void)
{
    // 这是一个归一化随机值从[0,1]的方法
    // rand会获得从0到RAND_MAX+1.0的随机数,所以rand = RAND_MAX+1.0的时候等于1,所以rand = 0则等于0;
    return std::rand() / (RAND_MAX + 1.0);
}


int xyl::math::floor(const int& value)
{
    return value - (value % 1);
}

int xyl::math::ceil(const int& value)
{
    return value + (1 - (value % 1));
}

// void xyl::WileProtector(long max)


unsigned int xyl::strlen(const char *str, unsigned int max = 0)
{
    unsigned int index = 0;
    if(max>0)
    {
        while(str[index]!='\0' && index<max)
        {
            index+=1;
        }
        return index;
    }
    else
    {
        while(str[index]!='\0')
        {
            index+=1;
        }
        return index;
    }
}

unsigned int xyl::wstrlen(const wchar_t *str, unsigned int max)
{
    unsigned int index;
    if(max==0)
    {
        while(str[index] != '\0')
        {
            index =index + 1;
        }
    }

    return index;
}
