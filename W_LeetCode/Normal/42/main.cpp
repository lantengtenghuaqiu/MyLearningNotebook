#include <iostream>

std::vector<int> h1{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
std::vector<int> h2{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
std::vector<int> h3{4, 2, 0, 3, 2, 5};
std::vector<int> h4{2, 0, 2};

// 设一个temp存储当前可能存储的雨水.
// 设置当前和最高量尺

// 暴力解法:
int TrappingRainWater(std::vector<int> &height)
{
    int totalVolume = 0;

    int maxHeightIndex = 0;
    int maxHeight = 0;
    for (int i = 0; i < height.size(); i++)
    {
        if (height[i] >= maxHeight)
        {
            maxHeight = height[i];
            maxHeightIndex = i;
        }
    }
    std::cout << " Max Height Index : " << maxHeightIndex << " Volum : " << maxHeight << std::endl;

    // int currentHeight = height[0];
    int left = 1;
    int leftMaxHeight = height[0];
    int leftCurrentVolume = 0;

    int right = height.size() - 2;
    int rightMaxHeight = height[height.size() - 1];
    int rightCurrentVolume = 0;

    for (int index = 0; index < height.size(); index++)
    {
        if (left <= maxHeightIndex)
        {
            if (height[left] < leftMaxHeight)
            {
                leftCurrentVolume += leftMaxHeight - height[left];
                std::cout << " Left Volum : " << leftCurrentVolume << std::endl;
            }
            else //(height[index] > maxHeight)
            {
                leftMaxHeight = height[left];
                totalVolume += leftCurrentVolume;
                leftCurrentVolume = 0;
            }
            left++;
        }

        if (right >= maxHeightIndex)
        {
            if (height[right] < rightMaxHeight)
            {
                rightCurrentVolume += rightMaxHeight - height[right];
                std::cout << " Right Volum : " << rightCurrentVolume << std::endl;
            }
            else //(height[index] > maxHeight)
            {
                rightMaxHeight = height[right];
                totalVolume += rightCurrentVolume;
                rightCurrentVolume = 0;
            }
            right--;
        }
    }
    return totalVolume;
}

int main()
{
    std::cout << "Hello World" << std::endl;
    std::cout << TrappingRainWater(h4) << std::endl;

    return 0;
}