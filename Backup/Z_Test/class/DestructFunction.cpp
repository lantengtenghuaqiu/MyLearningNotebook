#include <stdio.h>
class destruct
{
public:
    destruct(int index) { printf("Construct\n"); }
    ~destruct() { printf("Destruct\n"); }
};

void func()
{
    printf("Function life circle:\n");
    destruct de3;
}

int main()
{
    printf("main life circle:\n");
    destruct de;
    {
        printf("life sub-circle under the main:\n");
        destruct de2;
    }
    func();

    return 0;
}