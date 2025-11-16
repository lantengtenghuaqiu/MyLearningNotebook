#include<iostream>

class v
{
    public:
        int a=10;
};

const v func(v aa)
{
    return aa;
}
int func2()
{
    return 10;
}
int main()
{
    v aa;
    v bb;

    bb =aa;
    func(aa) = bb;
    std::cout<<aa.a<<std::endl;

    return 0;
}