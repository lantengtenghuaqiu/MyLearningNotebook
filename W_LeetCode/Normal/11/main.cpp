#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
std::vector<int> height{1, 8, 6, 2, 5, 4, 8, 3, 7};
//Size = 8 | Index = 0 ~ 7
//算法题解:
//在移动时移动小的那一方是因为,小的那一方决定了y的高度,并且在移动前的x已经是最大的宽度,所以无论左面如何移动永远都不会大于之前的容量.

//疑惑:
//为什么选用移动小的那一方的索引和暴力算法和对每个进行进行比较的效果是一样的?
    //
    //首先还是因为小的决定最终容量,当进行暴力解法进行逐一查找确实可以得到正确答案,但是包含了无效解,移动短边的目的是排除无效解.
    //以下数组:
    //1, 8, 11, 0, 2, 5, 4, 9, 3, 10
    //当选择暴力解法,即便是遇到了最大数,再正常升高左侧索引,永远都只是对于最右边的那个值的变化,而他的宽度在逐渐减少,所以不会因为只变化左边而提高最终容量,只会变小.
    //而选择双指针,遇到小值增加的方法是保证当前遇到最大值的时候,宽度最大的方法.所以移动小值是遇上下一个更大值时保证宽度的优秀方法.


int MaxCapacity(std::vector<int> &height)
{
    int rightSize = height.size();
    int maxCapacity = 0;
    int gap = 0;
    int maxHeight = 0;
    int leftP = 0;
    int rightP = rightSize - 1;

    try
    {
        while (leftP != rightP)
        {
            if (height[leftP] <= height[rightP])
            {
                maxHeight = height[leftP];
                gap = rightP - leftP;
                leftP++;
            }
            else
            {
                maxHeight = height[rightP];
                gap = rightP - leftP;
                rightP--;
            }

            if (maxHeight * gap > maxCapacity)
            {
                maxCapacity = maxHeight * gap;
            }

            if (leftP > rightSize || rightP < 0)
            {
                throw "Out of range or border!!! Wrong while loop...";
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return maxCapacity;
}

int MaxCapacityLowMemory()
{
    int rightSize = height.size();
    int maxCapacity = 0;
    int gap = 0;
    int maxHeight = 0;
    int leftP = 0;
    int rightP = rightSize - 1;

    while (leftP != rightP)
    {
        maxHeight = std::min(height[leftP], height[rightP]);
        gap = rightP - leftP;
        maxCapacity = std::max(maxCapacity, maxHeight * gap);

        if (height[leftP] <= height[rightP])
            leftP++;
        else
            rightP--;
    }

    return maxCapacity;
}

int main()
{
    int maxCapacity = MaxCapacity(height);
    std::cout << maxCapacity << std::endl;

    return 0;
}