#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
struct Strings
{
public:
    std::vector<std::string> strs1{"flower", "flow", "flight"};
    std::vector<std::string> strs2{"dog", "racecar", "car"};
    std::vector<std::string> strs3{"a"};
    std::vector<std::string> strs4{""};

};

inline std::string LongestCommonPrefix(std::vector<std::string> &strs)
{
    size_t size = strs.size();
    std::string str = "";
    size_t miniumSize = 0;
    for (size_t i = 0; i < size; i++)
    {
        miniumSize = miniumSize < strs[i].size() ? strs[i].size() : miniumSize;
        // printf("%lld\n", strs[i].size());
    }
    size_t j = 0;
    char ch;
    bool start = true;
    bool isSame = false;
    while (start)
    {
        ch = strs[0][j];
        if (j >= miniumSize)
            break;
        for (size_t i = 0; i < size; i++)
        {
            if (ch == strs[i][j])
            {
                isSame = true;
            }
            else
            {
                isSame = false;
                start = false;
                break;
            }
        }
        if (isSame)
            str += ch;
        j++;
    }
    std::cout << str << std::endl;
    return str;
}

int main()
{
    Strings strs;
    LongestCommonPrefix(strs.strs1);
    LongestCommonPrefix(strs.strs2);
    LongestCommonPrefix(strs.strs3);
    LongestCommonPrefix(strs.strs4);

    return 0;
}