#include <iostream>
#include<windows.h>
#include<conio.h>
#include"Tools.hpp"

static bool _break = false; 

int main()
{


    std::cerr<<"Press to break while looping";
    while(!_break)
    {
        if(_kbhit() != 0)
        {
            SINGLE_LOGE(_kbhit());
            _break = true;
        }

        Sleep(1000);
    }
    return 0;
}