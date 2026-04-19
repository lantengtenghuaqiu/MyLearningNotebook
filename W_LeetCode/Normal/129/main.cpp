#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> array1 = {100, 4, 200, 1, 3, 2};
std::vector<int> array2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
std::vector<int> array3 = {1, 0, 1, 2};

std::vector<int> array4 = {1, 2, 3, 4, 0, 6, 7, 8, 9, 10, 11, 12};

std::vector<int> array5 = {};

int LongestConsecutive(std::vector<int> nums)
{
    int currentTimes = 1;
    int currentMax = 1;
    int differ;
    bool firstLoop = true;

    if (nums.size() <= 0)
        return 0;

    std::sort(nums.begin(), nums.end());
    int previousNum = nums[0];

    for (int i = 0; i < nums.size(); i++)
    {
        // std::cout << num << std::endl;
        if (firstLoop)
        {
            firstLoop = false;
            continue;
        }

        differ = nums[i] - previousNum;
        if (differ <= 1)
        {
            if (differ == 1)
                currentTimes++;
        }
        else
        {
            currentTimes = 1;
        }

        if (currentTimes > currentMax)
        {
            currentMax = currentTimes;
        }

        previousNum = nums[i];
    }
    return currentMax;
}

int main()
{

    int Number = LongestConsecutive(array5);

    std::cout << Number << std::endl;

    return 0;
}