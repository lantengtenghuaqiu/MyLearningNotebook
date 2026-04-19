#ifndef XYLTOOLS
#define XYLTOOLS


#include "./BasicIncludes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

namespace Tools
{

    class TheFileManager
    {
        FILE *_file;

    public:
        char* path;

        TheFileManager() {}

        void GetContent( char *path,  char *mode, char *&container)
        {
            this->_file = fopen(path, mode);
            if (this->_file)
            {
                fseek(this->_file, 0, SEEK_END);

                long file_size = ftell(this->_file);

                fseek(this->_file, 0, SEEK_SET);

                if (container != nullptr)
                {
                    delete[] (container);
                }
                container = new char[file_size + 1]();

                fread(container, 1, file_size, this->_file);
            }

            fclose(this->_file);
        }

        TheFileManager(const TheFileManager &) = delete;
        TheFileManager operator=(TheFileManager &) = delete;
    };
} // namespace ReadFile

#endif