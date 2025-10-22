#include<iostream>
#include<fstream>

#include<cstdint>

#ifdef _WIN32
    #include <conio.h>
    // #define WHILELOOPING
    #define LOGER(msg) std::cout<<__LINE__<<" : "<<msg<<std::endl;
#endif

const auto PPM_WIDTH = 516;
const auto PPM_HEIGHT = 516;
const uint16_t MAXCOLOR = 255;
const uint8_t uint8tLength = sizeof(uint8_t);
int main()
{
    std::cout<<"Hello Wolrd"<<std::endl;
    std::ofstream testFILE("test.ppm",std::ios::binary);

    const uint8_t MAXCOLOR2 = 254;
    std::cout<<(int)MAXCOLOR2<<std::endl;

    int16_t ScreenCenterX = PPM_WIDTH/2;
    int16_t ScreenCenterY = PPM_HEIGHT/2;



    #ifdef _WIN32
        std::string fileName = __FILE__;
        std::cout<<"There environment is under the win32"<<std::endl;

        testFILE<<"P6\n";
        testFILE<<PPM_WIDTH<<" "<<PPM_HEIGHT<<"\n";
        testFILE<<MAXCOLOR<<"\n";

        int16_t r = 0;
        int16_t g = 0;
        int16_t b = 0;

        for(int v = 0 ; v <  PPM_HEIGHT; v++)
        {
            for(int u = 0 ; u <  PPM_WIDTH; u++)
            {
                r = ((static_cast<float>(u) - (PPM_WIDTH/2)) / (PPM_WIDTH/2)) * 255;
                g = ((static_cast<float>(v) - (PPM_HEIGHT/2)) / (PPM_HEIGHT/2)) * 255;
                b = 0;

                testFILE<<(char)(r)<<(char)(g)<<(char)(b);
                // testFILE.write(reinterpret_cast<const char*> (&r) , 1);
                // testFILE.write(reinterpret_cast<const char*> (&g) , 1);
                // testFILE.write(reinterpret_cast<const char*> (&b) , 1);
            }
        }

        LOGER(r);
        #ifdef WHILELOOPING
            while(true)
            {
                if(_kbhit())
                {
                    uint8_t key = _getch();
                    std::cout<<"End the "<<fileName<<" while looping"<<key<<std::endl;
                    break;
                }
            }
        #endif
    #elif _APPLE_

        testFILE <<"P6\n";
        testFILE<<"#test for ppm version 0.0.1\n";
        testFILE <<PPM_WIDTH<<" "<<PPM_HEIGHT<<"\n";
        testFILE <<MAXCOLOR<<"\n";
        for(int v = 0 ; v < PPM_HEIGHT ; v++)
        {
            for(int u = 0 ; u < PPM_WIDTH ; u++)
            {
                uint8_t r = u/PPM_WIDTH * 255;
                uint8_t g = v/PPM_HEIGHT * 255;
                uint8_t b = 0;
                testFILE.write(reinterpret_cast<const char*>(&r),uint8tLength);
                testFILE.write(reinterpret_cast<const char*>(&g),uint8tLength);
                testFILE.write(reinterpret_cast<const char*>(&b),uint8tLength);

            }
            testFILE<<std::endl;;
        }

    #endif


    std::cout<<"END!!!";
    return 0;
}