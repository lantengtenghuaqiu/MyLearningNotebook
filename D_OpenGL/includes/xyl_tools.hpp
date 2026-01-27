#ifndef XYLTOOLS
#define XYLTOOLS
#include <stdio.h>
#define ShaderChecker                               \
    printf("Wrong Compile (%d)\n", checker);        \
    char info[512];                                 \
    glGetShaderInfoLog(shader_id, 512, NULL, info); \
    for (int i = 0; i < 512; i++)                   \
    {                                               \
        printf("%c", info[i]);                      \
    }                                               \
    printf("\n");

class ReadFile
{
    FILE *_file;

public:
    ReadFile() {}

    void GetContent(const char *path, const char *mode, char *&container)
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

            // container[file_size + 1] = '\0';
        }

        fclose(this->_file);
    }

    ReadFile(const ReadFile &) = delete;
    ReadFile operator=(ReadFile &) = delete;
};

#endif