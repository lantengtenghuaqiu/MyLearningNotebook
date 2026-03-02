#include <vector>
#include <stdio.h>
#include <algorithm>
// 一组数字,给数组的最后一位的值plus one.
// 但是最后一位如果是9则要根据十进制向前进一位,也就是说数组内的数值需要保持为一位.
//[1,2,3] -> [1,2,4]
//[9] -> [1,0]

std::vector<int> PlusOneAtTheEnd(std::vector<int> &digits)
{
    size_t max_size = digits.size();
    digits[max_size - 1] += 1;

    std::vector<int> temp;
    for (size_t i = max_size; i > 0; i--)
    {
        temp.push_back(digits[i - 1]);
    }

    for (size_t i = 0; i < max_size; i++)
    {
        if (temp[i] >= 10)
        {
            temp[i] %= 10;
            if (i == max_size - 1)
            {
                temp.push_back(1);
                break;
            }
            temp[i + 1] += 1;
        }
    }
    digits.clear();
    max_size = temp.size();
    for (size_t i = max_size; i > 0; i--)
    {
        digits.push_back(temp[i - 1]);
    }
    return digits;
}

std::vector<int> PlusOneAtTheEndII(std::vector<int> &digits)
{
    size_t max_size = digits.size();
    std::reverse(digits.begin(), digits.end());
    digits[0] += 1;

    for (size_t i = 0; i < max_size; i++)
    {
        if (digits[i] >= 10)
        {
            digits[i] %= 10;
            if (i == max_size - 1)
            {
                digits.emplace_back(1);
                std::reverse(digits.begin(), digits.end());
                return digits;
            }
            digits[i + 1] += 1;
        }
    }
    std::reverse(digits.begin(), digits.end());
    return digits;
}

void Print(std::vector<int> numbers)
{
    for (size_t i = 0; i < numbers.size(); i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

int main()
{
    std::vector<int> numbers{1, 2, 3};
    std::vector<int> numbersII{4, 3, 2, 1};
    std::vector<int> numbersIII{9, 9, 9};

    // std::vector<int> result = PlusOneAtTheEnd(numbersII);
    // Print(result);

    std::vector<int> resultII = PlusOneAtTheEndII(numbersII);
    Print(resultII);

    printf("%d", 20 % 10);
    return 0;
}