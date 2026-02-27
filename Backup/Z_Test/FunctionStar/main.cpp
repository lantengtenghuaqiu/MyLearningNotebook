#include <stdio.h>
int Add(int a ,int b)
{
    return a + b;
}

int Multiple(int a , int b)
{
    return a * b;
}

int Calculate(int a,int b , int(*callback)(int a,int b))
{
    return callback(a,b);
}

int main()
{
    int result = 0;
    result = Calculate(1,2,Add);
    printf("%d\n",result);
    result = Calculate(1,2,Multiple);
    printf("%d\n",result);
    return 0;
}