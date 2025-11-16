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


int main()
{

    std::ofstream file("_12_2PositioningAndOrientingTheCamera.ppm" , std::ios::binary|std::ios::out);

    if(!file.is_open())
    {
        LOG_INFO("");
        return -1;
    }

    file<<"P6\n"<<IMG_WIDTH<<" "<<IMG_HEIGHT<<"\n255\n";

    double R = std::cos(xyl::consts::pi/4.0);

    auto material_ground = std::make_shared<Lambertain>(color3(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertain>(color3(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectic>(1.50);
    auto material_bubble = std::make_shared<Dielectic>(1.00 / 1.50);
    auto material_right  = std::make_shared<Metal>(color3(0.8, 0.6, 0.2), 0.9);

    //World:
    hittable_list world;

    world.add(std::make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(std::make_shared<Sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));


    //Camera:
    vec3 lookform(-2.0,2.0,1.0);
    vec3 lookup(0.0,0.0,-1.0);
    vec3 lookat(0.0,1.0,0.0);

    double fov = 90.0;

    Camera camera(lookform,lookup,lookat,fov);
    camera.Initialize();
  
    camera.Render(file,world);

 
    xyl::log::CloseStreamings(xyl::log::LOGER_FILE_STREAM,file);

    return 0;
} 

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------