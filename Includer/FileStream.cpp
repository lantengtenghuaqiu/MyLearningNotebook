#include "FileStream.hpp"

static void PPMStream(std::ofstream& ppmstream,auto width,auto height)
{
    std::ofstream ppmstream("PPMstream.ppm");

    if(!ppmstream.is_open())
    {
        std::cout<<__LINE__<<" : File wrong"<<std::endl;
        return -1;
    }

    ppmstream<<"P6\n"<<static_cast<int>(width)<<" "<<static_cast<int>(height)<<"\n255\n";

}