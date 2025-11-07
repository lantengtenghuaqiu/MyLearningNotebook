#include "FileStream.hpp"

void PPMStream(std::ofstream& ppmstream,unsigned int width,unsigned int height)
{

    if(!ppmstream.is_open())
    {
        std::cout<<__LINE__<<" : File wrong"<<std::endl;
    }

    ppmstream<<"P6\n"<<width<<" "<<height<<"\n255\n";

}