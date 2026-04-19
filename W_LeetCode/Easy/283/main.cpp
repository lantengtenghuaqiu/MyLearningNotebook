#include <iostream>
#include <vector>
#include <algorithm>
std::vector<int> nums = {0, 1, 0, 3, 12};
std::vector<int> nums2 = {0};
std::vector<int> nums3 = {2, 1};
std::vector<int> nums4 = {0, 1, 0, 2, 9, 0, 7, 2};

void MoveZeroPositiveSequence(std::vector<int> nums)
{
    std::sort(nums.begin(), nums.end(), std::greater<int>());

    int zeroCount = 0;
    for (int i = nums.size() - 1; i > 0; i--)
    {
        if (nums[i] == 0)
        {
            zeroCount++;
        }
        else
        {
            break;
        }
    }

    std::sort(nums.begin(), nums.end() - zeroCount);
    for (int n : nums)
    {
        std::cout << n << std::endl;
    }
}

void MoveZero(std::vector<int> nums)
{
    int zeroCount = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        if (i + zeroCount < nums.size() && nums[i + zeroCount] == 0)
        {
            zeroCount++;
            while (i + zeroCount < nums.size() && nums[i + zeroCount] == 0)
            {
                zeroCount++;
            }
        }
        if (i + zeroCount >= nums.size())
        {
            nums[i] = 0;
        }
        else
        {
            nums[i] = nums[i + zeroCount];
        }
    }
    
    for (int i = 0; i < nums.size(); i++)
    {
        std::cout << nums[i] << std::endl;
    }
}

int main()
{

    MoveZero(nums);

    return 0;
}