<<Shell Script>>
    ->#Mac:
        chmod +x FileName.sh 打开权限
        echo "内容" 将文本输出到终端
        SCRIPT_PATH=$(realpath "$0") 获取脚本自身绝对路径(SCRIPT_PATH是自定义名称)
        SCRIPT_DIR=$(dirname "$(realpath "$0")") 获取脚本所在目录的路径,不包含当前文件的名称(SCRIPT_PATH是自定义名称)
        变量:
            字符串
                ParameterName="文本字符串内容"
        函数:
            shell函数没有参数列表,在函数体内用$1,$2...表示参数,在使用函数时调用函数时紧跟的内容就会成为参数传入函数.
            定义函数:
                function 函数名 {
                     echo "$1"
                     return 状态码(return为可选部分)
                }    
            使用函数:
                函数名 参数1 参数2...
