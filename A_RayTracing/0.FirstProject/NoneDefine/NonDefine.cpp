#include <iostream>
#include <fstream>
#include "..\..\Includer\Variables.hpp"

#define CANVASWIDTH 512
#define CANVASHEIGHT 512

int main()
{
    std::cout<<"Hello World"<<std::endl;
    std::ofstream file("NoneDefine.ppm",std::ios::binary);
    if(!file.is_open())
    {
        std::cerr<<"Here is something wrong"<<std::endl;
        return -1;
    }
    
    file<<"P6\n"<<CANVASWIDTH<<" "<<CANVASHEIGHT<<"\n255\n";

    Color_255RGB white(255,255,255);
    UV uv(100,100);

    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;

    for(int u = 0 ; u <CANVASHEIGHT ; u++)
    {
        for(int v = 0 ; v<CANVASWIDTH ; v++)
        {
            file<<white.r
                <<white.g
                <<white.b;

        }

    }
    file.close();


    
    
    //vec3
    vec3 *direction = new vec3(1,2,3);
    *direction = -(*direction);
    std::cout<<direction->x()<<std::endl;

    vec3 dir =  vec3(3,4,5);
    dir = -dir;
    std::cout<<dir.x()<<std::endl;
    std::cout<<dir<<std::endl;

    dir = dir * 8.0;
    std::cout<<dir<<std::endl;

    dir = dir/8.0;
    std::cout<<dir<<std::endl;

    std::cout<<dir.length()<<std::endl;

    std::cout<<normalize(dir,VEC_CHECKER::e_vec)<<std::endl;

    delete(direction);

    vec3 col = vec3(5,2,3);


    std::cout<<std::endl;
    std::cout<<"Good Bye World"<<std::endl;
    // getchar();
    return 0;
}

