main.obj : error LNK2019: 无法解析的外部符号 "void __cdecl V3_LOGE<int>(int,int,int)" (??$V3_LOGE@H@@YAXHHH@Z)，
函数 main 中引用了该符号 [G:\...\main.vcxproj] 
G:\...\main.exe : fatal error LNK1120: 1 个无法解析的外部命令 [G:\...\main.vcxproj]

案例描述:c++模板声明在Tools.hpp文件,但是实现是在Tools.cpp文件,

尝试办法:单独编译Tools.cpp和main.cpp文件为obj文件,然后将Tools.obj与main.obj文件进行链接,生成可执行文件依旧报错

解决方法:c++模板不可以将声明和实现在不同文件下.