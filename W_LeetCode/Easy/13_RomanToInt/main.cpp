
#include <string>
#include <stdio.h>
// #include <cstring>
/*
I       1
II      2
III     3
IV      4
V       5
VI      6
VII     7
VIII    8
VIIII   9
X       10
L       50
C       100
D       500
M       1000
*/

// 如果1比2大则不管2,得到1的得数,反之1比2小,1就会被2减去.
// 比如MCM M是1000比C大,但是这里不减去C.但C比M小就会被M减去,得到M + (M-C) : 1000 + (1000-100);

// 所以要用两个指针对比befor和after的值

int RomanToInt(std::string s)
{
    size_t size = s.size();
    int *numbers = new int[size + 1]{0};
    printf("size:%lld\n", size);

    for (size_t i = 0; i < size; i++)
    {
        switch (s[i])
        {
        case 'I':
            numbers[i] = 1;
            break;
        case 'V':
            numbers[i] = 5;
            break;
        case 'X':
            numbers[i] = 10;
            break;
        case 'L':
            numbers[i] = 50;
            break;
        case 'C':
            numbers[i] = 100;
            break;
        case 'D':
            numbers[i] = 500;
            break;
        case 'M':
            numbers[i] = 1000;
            break;

        default:
            numbers[i] = 0;
            break;
        }
    }

    size_t i;
    int result = 0;
    for (i = 0; i < size + 1;)
    {
        printf("index %lld:\n", i);
        printf("result:%d\n", result);

        printf("number:%d\n", numbers[i]);

        if ((i + 1) >= (size + 1))
            break;

        if (numbers[i] < numbers[i + 1])
        {
            result += (numbers[i + 1] - numbers[i]);
            i += 2;
            printf("1<2\n");
        }
        else if (numbers[i] >= numbers[i + 1])
        {
            printf("1>=2\n");
            while ((i + 1) < (size + 1) && numbers[i] >= numbers[i + 1])
            {
                result += numbers[i];
                i += 1;
            }
        }
    }
    printf("result:%d\n", result);

    return result;
}

class RomanToInt_II
{
public:
    int GetResult(std::string s)
    {
        size_t size = s.size();
        size_t i;
        int result = 0;
        for (i = 0; i < size + 1;)
        {
            if ((i + 1) >= (size + 1))
                break;

            if (GetValue(s[i]) < GetValue(s[i + 1]))
            {
                result += (GetValue(s[i + 1]) - GetValue(s[i]));
                i += 2;
            }
            else if (GetValue(s[i]) >= GetValue(s[i + 1]))
            {

                while ((i + 1) < (size + 1) && GetValue(s[i]) >= GetValue(s[i + 1]))
                {
                    result += GetValue(s[i]);
                    i += 1;
                }
            }
        }
        printf("%d", result);
        return result;
    }

private:
    int GetValue(char s)
    {
        switch (s)
        {
        case 'I':
            return 1;

        case 'V':
            return 5;

        case 'X':
            return 10;

        case 'L':
            return 50;

        case 'C':
            return 100;

        case 'D':
            return 500;

        case 'M':
            return 1000;

        default:
            return 0;
        }

        return 0;
    }
};

int main()
{
    // RomanToInt("III");
    // RomanToInt("IV");
    // RomanToInt("IX");
    // RomanToInt("LVIII");
    // RomanToInt("MCMXCIV");
    RomanToInt_II roman;
    roman.GetResult("III");
    roman.GetResult("IV");
    roman.GetResult("IX");
    roman.GetResult("LVIII");
    roman.GetResult("MCMXCIV");

    return 0;
}