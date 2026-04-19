#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::string s1[] = {"cbaebabacd", "abc"};
std::string s2[] = {"abab", "ab"};
std::vector<int> FindAnagrams(std::string s, std::string p)
{
    std::unordered_map<char, int> pCter;
    std::vector<int> sCter;

    for (int i = 0; i < p.size(); i++)
        pCter[p[i]] += 1;

    int right = 0;
    // cbaebabacd
    for (int left = 0; left < s.size();)
    {
        std::unordered_map<char, int> pCterTemp;
        for (; right < p.size();)
        {
            if (left + right >= s.size())
            {
                break;
            }
            char c = s[left + right];
            std::cout << "left : " << left << " right : " << right << " character : " << c << std::endl;

            pCterTemp[c] += 1;

            if (pCterTemp[c] > pCter[c]) // 如果不正确left跳过当前不正确的right,并且直接跳过第二层循环,使left右移,并保证下一次right重新开始计数..
            {
                if (pCter[c] == 0)
                    left += right;
                else
                    left += right - 1; // 直接跳到不正确的内容下的索引,在离开第二层循环的时候++可以直接从不正确的下一个开始检测.
                right = 0;

                break;
            }

            if (right == p.size() - 1)
            {
                // std::cout << "left : " << left << " right : " << right << " character : " << c << std::endl;

                sCter.push_back(left);
                right = p.size() - 1; // 现在可以知道在p最后一个字符无问题,所以对于left当前,直接跳过两个字符,在下一次直接判断left+2判断新加的那一个字符是否正确.
                break;
            }
            right++;
        }
        left++;
    }
    return sCter;
}

int main()
{
    for (int n : FindAnagrams(s2[0], s2[1]))
    {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}