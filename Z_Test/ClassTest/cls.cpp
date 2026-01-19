#include <iostream>

class vec3
{
    public:
        double d1 = 7;
        int i1 = 8;
        char ch1 = '9';
};

class ii{
    public:
        char ch2 = 'o';
};

int main()
{
    vec3 v;
    void* p1 = &v.d1;
    void* p2 = &v.i1;
    void* p3 = &v.ch1;


    ii * a = new ii();
    char m = a->ch2;
    delete(a);
    a=nullptr;

    std::cout<<(v.ch1)<<std::endl;
    std::cout<<p1<<std::endl;
    std::cout<<p2<<std::endl;
    std::cout<<p3<<std::endl;


    std::cout<<m<<std::endl;

    return 0;
}