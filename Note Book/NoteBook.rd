目录
第一章	2


 
Git命令集合:
Git中修改和更改的数据都在工作区,通过add可以将内容上传到当前分支的暂存区(add不是只一次,而是可以多次add统一commit),最后通过commit提交到本地仓库.其中暂存区不能随意切换仓库
	Untrack 		: 未跟踪(新创建没有被git管理的文件)
	Unmodified 	: 未修改(已经被git管理的文件,但是没有修改)
	Modified 	: 已修改(被git管理的文件,出现了数据层的变动,但是还没有加入到暂存区内)
	Staged		: 已暂存(修改后的文件通过add暂存的)

Init:
	git init 在当前目录下初始化仓库

Config:
	git config --local user.name "XIE" 加local在本地仓库.git内的config文件有相应的内容
	git config --local user.email m13953332167@163.com 在本地仓库.git内的config文件有相应的内容
	git config --local config --list 查看配置的本地库中的信息(这里输出时终端可能因为是分page进行输出会卡住,按q退出)
	git config --no-pager config --local --list 查看配置无分page输出

Status:
	git status 查看当前的状态(当前分支名称,当前分支commit的文件等等..)

Add:
	git add 未跟踪或修改的文件名 通过git status会显示需要add的文件添加至暂存区
	git add *.txt 会将所有.txt的未跟踪或已修改的文件add到暂存区中
	git add . 将所有未跟踪或已修改的文件add到暂存区中

Commit:
	git commit -m “提交注释” 提交暂存区的内容到本地仓库中

Push:
	git push -u <远端仓库名><本地分支名> 将现有本地分支推送到远端,并建立上下游关系,如果远端有同名分支则推送(更新)到同名分支下
	git push tag <tag内容>		将tag为<tag内容>的分支push到远端,远端库tag会有相应的tag信息
	git push <远端库名称> --delete <tag内容> 删除远端库的<tag内容>的tag

Fetch:
	git fetch 从关联的远端库上游分支中获取最新的内容,与pull不同的是,fetch只会获取内容可以查看差别,pull会更新下游分支的内容
	git fetch upstream 获取远端库所有分支的更新
	git fetch <远端库名> <远端库分支名> 获取远端库的指定分支更新

Pull:
	---pull到本地分支会合并本地分支的内容
	git pull 从该分支关联的上游分支,拉取最新资源
	git pull <远端库名> <远端库分支名> 从指定远端库的分支拉取最新资源,合并到本地分支;

Merge:
	--如果合并的两个分支没有重合部分,则不会出错,但是如果两个分支的相同文件,数据在新增上有不同,如两个script文件的基础代码上,a修改了script文件,b也修改了,两个相同文件数据不同,则会导致文件冲突Conflict.
	git merge <需要合并进来的分支名>		切换到需要合并的分支,可以将<需要合并进来的分支名>合并到当前分支.

Remote
 	git remote add <远端仓库名> SSH/HTTPS 添加远端分支到本地
	git remote -v 查看远端的关联库是什么
	git remote remove <远端仓库名> 删除本地与远端的关联

Reset
	git reset 将添加到暂存区中的文件回退

	git reset --soft	回退到某一版本,并保留工作区和暂存区的所有修改内容,只是回退版本commit的文件会回到暂存区,并保留当前版本工作区的内容

	gir reset --hard 	回退到某一版本,并丢弃工作区和暂存区的所有修改内容回退后,当前版本之后的commit工作区和暂存区的所有文件会被丢弃

	git reset --mixed 	回退到某一版本,[保留]工作区和[丢弃]暂存区的所有修改内容回退后,当前版本后的commit暂存区中所有文件会丢弃,但工作区内容保留,可重新add和commit

	git reset --soft/hard/mixed	HEAD^ 回退到上一版本

Ls-files:
	git ls-files 查看暂存区内容

Diff:
	git diff 查看两个相连版本的修改差异
	git diff ID ID 查看两个固定版本的差异
	git diff bash_name bash_name 两个分支间的差异 
	git diff .. filename 查看..版本filename的内容

Log:
	git log --oneline 查看提交历史
	git reflog 查看所有操作的历史记录,会有相应版本号,可以根据版本号再用reset回退

Rm:
	git rm -cached <filename> 会删除暂存区的文件,保留工作区的文件
	git rm -f <filename> 会删除工作区的文件,保留暂存区工作区的文件

Checkout:
	 git checkout --orphan <空分支名> 创建一个空分支,没有上游关联.创建后的新分支没有提交信息,git会自动视为丢弃分支,所以需要commit到本地分支才行

Switch:
	git switch -C <新分支名称> <本地分支名称> 	通过将现有的本地分支创建分支,本地分支建立,无上下游关联,与git branch <新分支名称>的区别是该操作跳转到新分支.
	git switch -C <新分支名称> <origin/分支名称> 通过将现有的远端分支创建分支,建立上下游关系

Branch
	git branch 查看当前本地所有分支,带有*号的为当前分支

git branch -r 查看远端分支
	git branch -a 查看所有分支包括远程分支,远程分支一般带remote/... 或者 origin/..
	git branch <新分支名称> 会根据当前所在分支创建新分支,创建后不跳转新分支,留在当前分支--!!!该分支无上下游
	git branch -d <分支名称> 删除分支
	git branch -m <新分支名称> 修改当前分支的分支名
	git branch -m <新分支名称> 强制修改当前分支的分支名,无论是否存在,存在则覆盖
	git branch -vv 查看所有分支的上下游关系
	git branch --set-upstream-to=<远端仓库名><远端分支名> <本地分支名>		使本地分支与远端仓库中的分支建立上下游关系

Tag
	tag是一个独立的存在,名称只能唯一.
	git tag <tag内容> -m “注释”		将本地当前分支添加一个名为<tag内容>的tag,可以用push推送tag
	git tag -d <tag内容>		删除本地<tag内容>的tag,删除远端tag可去Push查看

.gitignore
	.gitignore git忽略文件规则:
	一般忽略文件如:涉及身份,密码,口令等;编译生成的;系统自带文件;日志文件;等等...
	新建一个.gitignore(固定名称)
		添加忽略文件的名称或后缀:
			/FolderName				该文件夹会被忽略
			**/FolderName			所有名为该文件夹的文件夹都会被忽略
			.gitignore				该文件会被忽略
			test.txt				该文件会被忽略
			*.log					(通配符)所有后缀为.log文件会被忽略
			xx?x.log 				(通配符)匹配单个字符的相应文件
			~
			~
			~
	git默认不会将空文件夹添加到暂存区

SSH密钥
	需要在git bash下
		bash:
			ls -la ~/.ssh 查看本地ssh
			ssh-keygen -t ed25519 -C “@email.com”	用ed25519加密创建ssh
			cat ~/.ssh/id_ed25519.pub 查看公钥内容
		关联github:
			首先需要再账户下的add ssh key中配置本地的公钥;
				ssh -T git@github.com 建立连接

Windows终端命令集合:
	G: 或 g: 跳转盘符
	cd filename1/filename2... 在盘符下进入文件夹
	cd.. 返回上一级
	mkdir filename 在当前目录下,生成一个文件夹

	echo
		echo > “Contains that you can tap and write in the file which will be created” > README.rd
			在当前文件夹下创建一个文件,并写入“”内容

	ls 和 dir 输出当前文件夹下的所有非隐藏文件的名称和一些信息
	dir /a 显示所有文件和信息,但是在vscode中不行,可以在cmd中使用
	dir /b 只显示名称不显示其他信息

	“删除” 命令删除不会进入回收站为了以防万一使用,因此没有收录

通过终端编译c++文件:
	需要在被编译的文件夹下	
		g++ <filename>.cpp -o <filename>.exe会根据<filename>.cpp查询并编译cpp文件,然后通过-o生成二进制文件<filename>.exe.
 
CMake合集:
	终端命令:
		cmake -B <文件名> 当前目录下配置<文件名>(一般是build)的cmake文件,尤其是第一次配置cmake和修改后cmake要使用,重新配置cmake文件

		cmake -bulid <文件名> 构建cmake配置好的<文件名>(一般是build)文件,进行源文件和main文件以及链接头文件,生成可执行.exe文件
		CMakeLists.txt Code:
		#配置生成位置
		#${CMAKE_BINARY_DIR}是build文件夹,但build并不是固定名称,和一开始build cmake文件有关
		set_target_properties(nodefine PROPERTIES
		    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/main
		)

		add_custom_target(
		    run_auto ALL									#自定义名称
		    COMMAND “可执行文件的路径”  		#运行可执行文件
		    DEPENDS 编译文件名 						#依赖于 “编译文件名”构建完成
		    COMMENT "Auto-running myapp..."		#这个是类似注释一样的东西
		)





C++笔记:
	//我现在有一个class类vec3,内部有operator-()的实现,直接初始化,可以直接Negate a number:
			vec3 dir = new vec3(1,2,3)
			dir = -dir;
		但是如果是指针形式,需要对其解引用,再赋值:
			vec3 *dir = new vec3(2,3,4);
			*dir = -(*dir);

	//通过终端编译c++文件,生成.exe文件:
		需要在被编译的文件夹下	
			g++ <filename>.cpp -o <filename>.exe会根据<filename>.cpp查询并编译cpp文件,然后通过-o生成二进制文件<filename>.exe.
		
		只编译不生成可执行文件(.exe):
			g++ <filename>.cpp -o <filename>.obj

		如果有头文件则需要先编译头文件所对应的源代码.cpp文件,再生成.o文件,最后链接main的.o文件和头文件的.o文件
			g++ -c <headcpp>.cpp
			g++ -c <main>.cpp
			g++ <main>.o <headcpp>.o -o <executable file name>


		只编译不生成可执行文件:
			g++ -c <file.cpp> 这里的-c指的是compile only,所以只会编译成.o文件不会生成可执行文件,可以为了之后链接
			g++ -c <file.cpp> -o <folder/filename.o> -o指的是out,这段代码指的是将编译的文件输出到指定位置和指定名字

	//对于不同路径的头文件包含
		当编译时,包含头文件不是相对路径也不是绝对路径,只是#include”header.hpp”的时候,编译需要说明:
			g++ -c <main.cpp> -I <头文件目录(无需显示声明.hpp文件) -o <生成地址和文件.obj>


	//对于c++的线程休眠:
		_sleep(time) 这个函数需要包含#include<thread>,但是使用MinGW时在编译时会提示不推荐使用
				|--------------------------------------------------------------------------------------------|
				|E:/Environments/mingw64/x86_64-w64-mingw32/include/stdlib.h:689:24: note: declared here 689 |
				|   _CRTIMP void __cdecl _sleep(unsigned long _Duration) __MINGW_ATTRIB_DEPRECATED;		       |
				|--------------------------------------------------------------------------------------------|
 	//是因为这不是c++标准,是微软平台下的线程休眠的旧方法,已废弃,所以在不同平台下应使用相应平台的休眠:
		Windows 下: 包含#include<window.h> 使用Sleep(time)


	//类似C#中的变长参数:
		需要包含头文件#include<initializer_list>


		对于流文件的变长参数不可以使用上方,因为initializer_list<std::fstream>中的std::fstream默认为const形式,无法修改:
			所以使用变长参数,关闭流文件
				void CloseStreaming(){}	//这是关键不要漏下
				template<typename ... Args>
				void CloseStreaming(std::ofstream& first , Args& ... rest)
				{
					if(first.is_open())
					{
						first.close();
					}

					CloseStreaming(rest...);
				}

	//constexpr 编译期初始化:
		该关键字可以修饰使一些变量,自定义类型,函数,构造函数,for循环等在编译期就确定值,而不是在运行时获得值;
			class Point {
				private:
				    int x, y;
				public:
				    // constexpr 构造函数：确保对象可在编译期初始化
				    constexpr Point(int x_, int y_) : x(x_), y(y_) {}
			};
			如果构造函数的参数在编译前（编译期）没有确定（即参数是运行期变量），constexpr 构造函数会退化为普通构造函数，在运行期执行初始化逻辑 —— 这和非 constexpr 构造函数的行为完全一致
		constexpr和static的区别:
			static:	修饰的是在程序运行时初始化,全局生命周期,只有当程序结束时才销毁
			constexpr:	在编译期间就确定,所以运行期间也已确定,存储的生命周期,与局部变量相同,即当前域中(函数域/程序域/名称空间域...);

	
	//特化--c++特性
			特化时c++的一种特性,
				使用特化创建类似c#中的typeof判断:
					主模板:
						template<typename T, typename U> 
						struct is_same
						{
							static constexpr bool value = false;
						}
					偏特化:
						template<typename T>
						struc is_same<T,T>
						{
							static constexpr bool value = true;
						}
					当使用is_same<type1,type2> check;的时候,传入type1和type2如果相同会返回偏特化,则value为true,反之返回主模板false
					
	//类,特殊成员函数的默认版本:
		类中,如果无显示声明,则会自动生成一些特殊成员函数包括:
			默认构造函数（无参构造函数）；
			析构函数；
			拷贝构造函数；
			拷贝赋值运算符（operator=）；
			移动构造函数（C++11 后）；
			移动赋值运算符（C++11 后）。
		这些自动生成的成员函数会包含默认版本,比如析构函数会自动调用.但是如果显示的声明了特殊成员函数,则特殊成员函数不会包含默认版本
		所以如果声明了特殊成员函数,在某些情况下需要手动调用默认版本;
		所以c++提供了显式声明启用默认版本(=default);
		例如当有一个虚函数,等待被继承,当显式声明析构函数,则需要保证继承类能正确调用析构函数.
			virtual ~hittable() = default;
	
	//纯虚函数:
		表示一个函数没有默认实现,需要由派生类(子类)继承并实现;
		此外包含纯虚函数的类会变成抽象了,无法被直接实例化,只能被基类继承;
			virtual bool hit(const ray& r,double ray_tmin , double ray_tmax , hit_record& hitInfo)const = 0;
			这里的=0是声明纯虚函数的关键

	//public private protected三种类型的继承含义:
        class example1
        {
            private:
            	int num1;
            public:
            	int num2;
            protected:
            	int num3;
        };

        公共继承:
	        class example2 : public example1
	        {
	        	void func()
	        	{
	        		可以调用基类的公有和保护:
	        		this->num2;
	        		this->num3;
	        		
	        		基类中的私有无法调用
	        	}
	        };

	      私有继承:
	        class example3 : privated example1
	        {
	        	void func()
	        	{
	        		基类中所有的数据都私有化,无法调用
	        	}
	        };

       	保护继承:和公共继承几乎一样
	        class example4 : privated example1
	        {
	        	void func()
	        	{
	        		可以调用基类的公有和保护:
	        		this->num2;
	        		this->num3;

	        		基类中的私有无法调用
	        	}
	        };

	//函数中参数中的引用&:
		在一个函数中void func(int & para){}会有&这样的符号,这个符号表示引用,
		在使用函数func的时候传入para,参数列表中的形参会直接关联实参,也就是说para改变时,实参也会跟着改变,同样,参数对应的实参改变,para也会随之改变;
		引用的作用之一就是,避免传入参数时拷贝占用内存空间.此外对于非引用的重要一点就是不会改变实参,所以对于引用可以添加const修饰符进行修饰,避免para改变实参;


	//头文件前向声明:
		当头文件A.hpp中定义了B.hpp文件,并且在B.hpp文件中又包含了A.hpp文件,当main.cpp文件包含了这两个其中一个文件,都会导致循环定义,最后冲突无法正确构建:
		所以需要使用前向声明改变这种冲突:
		//没太搞明白//

	//通过define,如果包含一个特殊宏定义,该头文件内的方法才可以使用:
		头文件A:
			#ifndef DAH
			#define DAH

			#define MATH

			#endif
		头文件B:
			#ifndef DAH2
			#define DAH2

			#include"头文件A.hpp"

			#ifdef MATH	//如果包含了头文件A则查看该文件中有define MATH,所以该宏下可以使用
				int num = 0;

			#endif
			#endif

	->#const专栏#:
		//const是一个常量修饰符,总的来说是承诺不能修改修饰的变量;
		1.成员函数const:
			常量成员函数(const成员函数),主要用在自定义类型的类和结构体种中,修饰在函数最后,用以保证该函数不修改成员变量.
			对于一些可能只是保证不改变某一些变量,但是有些变量需要被改变的,需要加上mutable修饰符
			class c
			{
				private:
					int a = 0;
					mutable int b = 0;
				public:
					void func() const
					{
						a = 1;***!!!*** 不合法,无法修改成员
						b = 9;//合法,可以修改
					}
			}

		2.const变量:
			对于const变量,为保证该变量不在之后的逻辑中被修改,而添加const修饰.此外,由const修饰的变量,也称为常量,如一个自定义类型常量不只调用非const成员函数,只能调用const成员函数;
			class C
			{
				public:
					void func1() const{}
					void func2(){}
			}

			main()
			{
				const C c;//没有显式定义无参构造,初始化只不需要C c();
				c.func1();//合法
				c.func2();**!!不合法!!**

				//于此同时,非const变量,都可以调用两个func()
			}

	->#指针专栏#:
		//创建array需要用引用形式:
			int *num = new int[3];

		//new的简单使用:
			const int *number = new int;

		//智能指针:
			需要包含#include<memory>
			1.共享指针--shared_ptr<...>: