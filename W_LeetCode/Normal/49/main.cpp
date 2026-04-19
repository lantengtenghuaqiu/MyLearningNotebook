#include <algorithm>
#include <vector>
#include <iostream>

typedef std::string string;

bool StrChecker(string str1, string str2)
{
    if (str1.size() != str2.size())
        return false;
    std::sort(str1.begin(), str1.end());
    std::sort(str2.begin(), str2.end());

    return str1 == str2;
}

std::vector<std::vector<string>> groupAnagrams(std::vector<std::string> &strs)
{

    int currentIndex = 0;
    std::vector<std::vector<string>> finalContainer;

    std::vector<int> indexContainer;
    indexContainer.resize(strs.size(), -1);

    std::vector<string> word1;

    for (; currentIndex < strs.size(); currentIndex++)
    {
        word1.clear();

        if (currentIndex == indexContainer[currentIndex])
            continue;
        // std::cout << currentIndex << std::endl;

        for (int i = currentIndex; i < strs.size(); i++)
        {

            if (StrChecker(strs[currentIndex], strs[i]))
            {
                indexContainer[i] = i;
                word1.emplace_back(strs[i]);
                // std::cout << "Push Index " << i << std::endl;
            }
        }

        finalContainer.emplace_back(word1);
    }
    return finalContainer;
}
//-----------------------------------------------------------------------------
#include <unordered_map>
std::vector<std::vector<string>> GroupAnagramsII(std::vector<std::string> &strs)
{
    std::vector<std::vector<string>> finalContainer;
    std::unordered_map<std::string, std::vector<string>> orderedContainer;

    for (std::string str : strs)
    {
        std::string tmpStr{str};
        std::sort(tmpStr.begin(), tmpStr.end());
        orderedContainer[tmpStr].emplace_back(str);
    }
    for (auto ele : orderedContainer)
    {
        finalContainer.emplace_back(ele.second);
    }

    return finalContainer;
}

int main()
{
    printf("hello world\n");
    std::vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat", "eat"};
    std::vector<string> strs2 = {""};

    std::vector<std::vector<string>> result = GroupAnagramsII(strs);

    for (int l = 0; l < result.size(); l++)
    {
        for (int k = 0; k < result[l].size(); k++)
        {
            std::cout << result[l][k] << std::endl;
        }
        std::cout << std::endl;
    }
    // std::cout << "Index size : " << indexContainer.size() << std::endl;

    return 0;
}