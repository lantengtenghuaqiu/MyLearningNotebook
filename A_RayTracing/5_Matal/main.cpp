#include <iostream>
#include <fstream>
#include <cstdint>


#define XYLMATH
#define RTONEWEEK
#define LOGER

#define MATERIAL

#include "Camera.hpp"
#include "Variables.hpp"
#include "Tools.hpp"
#include "Loger.hpp"
#include "Material.hpp"

//���������:
int main()
{
    //�����ļ���:
    std::ofstream file("xxx.ppm" , std::ios::binary|std::ios::out);

    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<IMG_HEIGHT<<"\n255\n";

    //Camera:
    Camera camera;

    auto ground         =   std::make_shared<Lambertain>(color3(0.5,0.5,0.7));
    auto centerSphere   =   std::make_shared<Lambertain>(color3(0.7,0.7,0.6));
    auto rightSphere    =   std::make_shared<Metal>(color3(0.8,0.8,0.8),0.3);
    auto leftSphere     =   std::make_shared<Metal>(color3(0.8,0.6,0.2),1.0);

    //World:
    hittable_list world;
    world.add(std::make_shared<Sphere>(point3( 0.0,   -100.5,    -1.0),    100.0,    ground));
    world.add(std::make_shared<Sphere>(point3( 0.0,      0.0,    -1.2),      0.5,    centerSphere));
    world.add(std::make_shared<Sphere>(point3(-1.0,      0.0,    -1.0),      0.5,    leftSphere));
    world.add(std::make_shared<Sphere>(point3( 1.0,      0.0,    -1.0),      0.5,    rightSphere));

    //��ʼ��Ⱦ,д���ļ�:
    camera.Render(file,world);

    //�ر������ļ���:
    xyl::log::CloseStreamings(xyl::log::LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------