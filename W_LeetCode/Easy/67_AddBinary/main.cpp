#include<string>
// a = "11"  + b = "1"  = "100"

int main()
{
    // 3 -> 100

    // Model is wrong
    //{
    // 3 % 2 = 1
    // 1 % 2 = 1
    // 1 % 2 = 1

    //}

    /* Divide is wrong
        4 / 2 = 2
        2 / 2 = 1
        1 / 2 = 0.5
    */

    // subtraction is wrong
    //{
    // 3 - 2 = 1
    // 2 - 2 = 0
    // 1 - 2 = 0;

    // 4 - 2 = 2
    // 2 - 2 = 1
    // 1 - 2 = 0
    //}

    //1111 + 11
    //1122 % 2 = 01001
    //'1' == 2  = 0 carry = 1;
    std::string a = "1111";
    std::string b = "11";
    char carry = '0';
    size_t length = a.size() + 1;
    size_t i = length;
    char * c = new char[length];

    // while(i<length)
    // {
    //     if(a[i] == '0' && b[i]==)
    //     {

    //     }

    //     i++;
    // }
    

    return 0;
}