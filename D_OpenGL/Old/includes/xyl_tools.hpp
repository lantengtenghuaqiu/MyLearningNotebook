#ifndef XYLTOOLS
#define XYLTOOLS


#include "./BasicIncludes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

namespace ReadFile
{

    class TheFile
    {
        FILE *_file;

    public:
        char* path;

        TheFile() {}

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

        TheFile(const TheFile &) = delete;
        TheFile operator=(TheFile &) = delete;
    };
} // namespace ReadFile

#endif