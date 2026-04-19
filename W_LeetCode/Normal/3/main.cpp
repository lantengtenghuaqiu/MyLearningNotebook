#include <iostream>

std::string str1{"abcabcbb"};
std::string str2{"bbbbb"};
std::string str3{"pwwkew"};
std::string str4{"dvdf"};
std::string str5{"biidygcc"};
std::string str6{"ohvhjdml"};
std::string str7{"wobgrovw"};
std::string str8{"au"};

// 11 12 12 9 5 12
//  最大重复字符长度查询
int RepeatSameCharacterChecker(std::string s)
{
    int currentLength = 1;
    int maxLength = 1;
    for (int i = 1; i < s.size(); i++)
    {
        int diff = (int)s[i] - (int)s[i - 1];
        if (diff == 1)
        {
            currentLength++;
        }
        else
        {
            if (currentLength > maxLength)
            {
                maxLength = currentLength;
            }
            currentLength = 1;
        }
    }
    return maxLength;
}

// 最大不重复字符长度查询
#include <unordered_map>
#include <unordered_set>
// Wrong
//  int RepeatDifferentCharacterChecker(std::string s)
//  {
//      int currentLength = 0;
//      int maxLength = 1;

//     std::unordered_map<char, bool> container{};

//     for (int i = 0; i < s.size(); i++)
//     {
//         if (!container[s[i]])
//         {
//             container[s[i]] = true;
//             currentLength++;
//             if (currentLength > maxLength)
//             {
//                 maxLength = currentLength;
//             }
//         }
//         else
//         {

//             container.clear();
//             container[s[i]] = true;
//             currentLength = 1;
//         }
//     }
//     return maxLength;
// }

// 暴力解法
int RepeatDifferentCharacterChecker(std::string s)
{
    int currentLength = 0;
    int maxLength = 0;

    std::unordered_map<char, int> container{};

    for (int MainLoop = 0; MainLoop < s.size(); MainLoop++)
    {
        for (int EachCharLoop = MainLoop; EachCharLoop < s.size(); EachCharLoop++)
        {
            if (EachCharLoop == s.size() - 1)
                MainLoop = s.size() - 1;
            std::cout << s[EachCharLoop] << " ";
            if (container[s[EachCharLoop]] == 0)
            {
                currentLength++;
                container[s[EachCharLoop]] = EachCharLoop + 1;
                if (currentLength > maxLength)
                    maxLength = currentLength;
            }
            else
            {
                currentLength = 0;
                std::cout << " : ";

                if (s[EachCharLoop] == s[MainLoop])
                {
                    container.clear();

                    break;
                }
                // std::cout << "[Same : " << s[EachCharLoop] << "] ";

                MainLoop = container[s[EachCharLoop]] - 1;
                std::cout << s[EachCharLoop] << " : " << container[s[EachCharLoop]] << "   ";
                container.clear();

                break;
            }
        }
    }
    return maxLength;
}

// abcabcbb
// wobgrovw
int RepeatDifferentCharacterCheckerII(std::string s)
{
    if (s.size() > 0)
    {
        int currentLength = 0;
        int maxLength = 0;
        // abcabcbb
        // au
        std::unordered_map<char, int> container{};
        int LeftP = 0;
        for (int RightP = 0; RightP < s.size(); RightP++)
        {

            LeftP = std::max(LeftP, container[s[RightP]]);
            currentLength = RightP - LeftP + 1;

            // std::cout << " Right : " << RightP << " Left : " << LeftP << " Current Length : " << currentLength << std::endl;
            container[s[RightP]] = RightP+1;

            if (currentLength > maxLength)
                maxLength = currentLength;
        }

        return maxLength;
    }
    return 0;
}

int main()
{
    std::cout << "Hello World" << std::endl;
    // 最大重复字符长度查询
    // std::cout << "最大重复字符长度查询" << std::endl;
    // std::cout << "Max Length : " << RepeatSameCharacterChecker(str1) << std::endl;
    // std::cout << "Max Length : " << RepeatSameCharacterChecker(str2) << std::endl;
    // std::cout << "Max Length : " << RepeatSameCharacterChecker(str3) << std::endl;

    // 最大不重复字符长度查询
    // std::cout << "最大不重复字符长度查询(暴力解法)" << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str1) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str2) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str3) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str4) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str5) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str6) << std::endl;
    // std::cout << "Max Length : " << RepeatDifferentCharacterChecker(str7) << std::endl;

    // 最大不重复字符长度查询
    std::cout << "最大不重复字符长度查询(优化1)" << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str1) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str2) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str3) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str4) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str5) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str6) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str7) << std::endl;
    std::cout << "Max Length : " << RepeatDifferentCharacterCheckerII(str8) << std::endl;


    return 0;
}