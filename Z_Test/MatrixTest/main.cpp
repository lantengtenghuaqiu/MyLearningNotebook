#include <stdio.h>
#include <string>
#include <iostream>
class Matrix
{
public:
    std::string *colum1;
    std::string *colum2;
    std::string *colum3;
    std::string *colum4;

    Matrix(std::string *_colum1, std::string *_colum2, std::string *_colum3, std::string *_colum4)
    {
        this->colum1 = _colum1;
        this->colum2 = _colum2;
        this->colum3 = _colum3;
        this->colum4 = _colum4;
    }

    void PrintMatrix()
    {
        for (size_t i = 0; i < this->colum1->size(); i++)
        {
            // printf("",)
        }
    }

    // std::operator*(Matrix & m)
    // {
        
    // }
};

int main()
{
    std::string *colum1 = new std::string[4]{"1", "cos(t)", "-sin(t)", "0"};
    std::string *colum2 = new std::string[4]{"0", "sin(t)", "cos(t)", "0"};
    std::string *colum3 = new std::string[4]{"0", "0", "1", "0"};
    std::string *colum4 = new std::string[4]{"0", "0", "0", "1"};
    Matrix m1(colum1, colum2, colum3, colum4);



    // m1.colum1[0] = "1";
    std::cout << m1.colum1[2] << std::endl;

    return 0;
}