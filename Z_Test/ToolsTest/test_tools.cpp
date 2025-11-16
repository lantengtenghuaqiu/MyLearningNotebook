#include <iostream>
// #include"../Includer/Tools.hpp"
#include<fstream>
#define LOGER

void CloseStreamings() {}

template<typename... Args>//这里的...是固定写法
void CloseStreamings(std::ofstream& first , Args&... rest)
{
    if(first.is_open())
    {
        
        first.close();
        std::cout<<"file is closed"<<std::endl;
    }

    CloseStreamings(rest...);

}

// // 递归终止函数（处理参数包为空的情况）
// void CloseStreaming() {}

// // 变参模板：接收第一个文件流引用，关闭后递归处理剩余参数
// template<typename... Args>
// void CloseStreaming(std::ofstream& first, Args&... rest) {
//     if (first.is_open()) {  // 检查文件是否已打开
//         first.close();
//         std::cout << "文件流已关闭" << std::endl;
//     }
//     CloseStreaming(rest...);  // 递归处理剩余文件流
// }


int main()
{
    std::cout<<"Hello World"<<std::endl;
    std::ofstream f1("file1.txt");
    std::ofstream f2("file2.txt");
    std::ofstream f3("file3.txt");
    CloseStreamings(f1,f2);
    
    // V3_LOGE<int>(1,2,3);
    getchar();
    return 0;
}