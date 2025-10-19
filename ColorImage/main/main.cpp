#include <iostream>
#include <fstream>
#include "Variables.hpp"


#define IMG_WIDTH 1080
#define IMG_HEIGHT IMG_WIDTH/ratio_d_8_5

#define LOG_INFO(msg) std::cerr<<"["<<__FILE__<<" "<<__LINE__<<"]"<<msg<<std::endl;

int main()
{
    std::cout<<"Hello World"<<std::endl;

    std::ofstream file("ColorImage.ppm" , std::ios::binary|std::ios::out);

    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<(int)IMG_HEIGHT<<"\n255\n";
    DrawImg(IMG_WIDTH,IMG_HEIGHT,file);

    file.close();



    return 0;
} 