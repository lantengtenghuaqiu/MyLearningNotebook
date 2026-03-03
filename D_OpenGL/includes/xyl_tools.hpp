#ifndef XYLTOOLS
#define XYLTOOLS

#include "./BasicIncludes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"
// #define DEBUG

namespace Tools
{
    // Image Manger
    struct ImageManager
    {
        static ImageManager *GetInstance()
        {
            static ImageManager instance;
            return &instance;
        }
        unsigned char *data;
        char *path;

        int width;
        int height;
        int channels;

        ImageManager(const ImageManager &) = delete;
        ImageManager &operator=(const ImageManager &) = delete;

    private:
        ImageManager() = default;
        ~ImageManager() {};
    };

    // File Manager
    class TheFile
    {
    private:
        FILE *_file;
        TheFile() = default;
        ~TheFile() {};

    public:
        static TheFile *GetInstance()
        {
            static TheFile instance;
            return &instance;
        }
        char *path;

        void GetContent(const char *path, const char *mode, char *&container)
        {
            this->_file = fopen(path, mode);
            if (this->_file)
            {
                fseek(this->_file, 0, SEEK_END);

                long file_size = ftell(this->_file);

                fseek(this->_file, 0, SEEK_SET);

                container = new char[file_size + 1]();

                fread(container, 1, file_size, this->_file);

#ifdef DEBUG
                for (long i = 0; i < file_size; i++)
                {
                    printf("%c", container[i]);
                }
                printf("\n");
#endif
            }
            else
            {
                printf("File open is wrong");
            }

            fclose(this->_file);
        }

        TheFile(const TheFile &) = delete;
        TheFile operator=(TheFile &) = delete;
    };

    template <typename T, size_t N>
    size_t GetSizeofArray(T (&Array)[N])
    {
        return N;
    }
} // namespace Tools

#endif