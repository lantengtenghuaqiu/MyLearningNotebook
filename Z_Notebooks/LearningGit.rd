目录
第一章	2
 
Git命令集合:
Git中修改和更改的数据都在工作区,通过add可以将内容上传到当前分支的暂存区(add不是只一次,而是可以多次add统一commit),最后通过commit提交到本地仓库.其中暂存区不能随意切换仓库
	Untrack 	: 未跟踪(新创建没有被git管理的文件)
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
 







