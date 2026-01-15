#include <stdio.h>

template <typename T>
class coordinate4
{
protected:
    T coordinate[4] = {};

public:
    T &operator[](int i)
    {
        if (i >= 0 && i <= 3)
            return this->coordinate[i];
        else
            return this->coordinate[0];
    }
};

class float4 : public coordinate4<float>
{

    
};

int main()
{
    float4 ns;
    printf("%f", ns[4]);

    return 0;
}