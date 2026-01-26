#ifndef READFILE
#define READFILE
#include <stdio.h>
class ReadFile
{
    FILE file;
    ReadFile() {}

public:
    FILE get()
    {
        if (file)
        {
            return this->file;
        }
    }
    void GetContent(const char *path, const char *mode, char *container)
    {
        if (this->file == NULL)
        {
            this->file = fopen(path, mode);

            fseek(this->file, 0, SEEK_END);

            long file_size = ftell(this->file);

            fseek(this->file, 0, SEEK_SET);

            fclose(this->file);
        }
        else
        {
            fclose(this->file);
        }
    }
};

#endif