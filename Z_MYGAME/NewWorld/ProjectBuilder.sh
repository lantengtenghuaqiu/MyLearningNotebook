echo "🚀 Start compiling...."

COMPILEMODE="$1";

set -e

error_exit() {
    echo -e "\n❌ Script compiling with something wrong!"
    echo -e "❌ Failed command:$BASH_COMMAND"
    echo -e "⛔ Script is terminated!\n"
    exit 1
}

trap error_exit ERR
# -------------------------------------------------------------------
# 变量声明-------------------------------------------------------------
SCRIPT_PATH=$(dirname $(realpath "$0"))
echo $SCRIPT_PATH
SRC_PATH="$SCRIPT_PATH/Src"

# Source Files
SRC_GLAD_PATH="$SRC_PATH/glad.c"
SRC_WINDOW_FILES_PATH="$SRC_PATH/Window/WINDOWALL.cpp"
SRC_XTOOLS_PATH="$SRC_PATH/Xtools.cpp"

#
MAINCPP_PATH="$SCRIPT_PATH/A_MAIN/Main.cpp"
MAIN_NAME="NewWorld"

#
SRC_OUTPUT="$SCRIPT_PATH/A_MAIN/build"

#
INCLUDE_PATH="$SCRIPT_PATH/Include"
LIB_PATH="$SCRIPT_PATH/Lib/Mac"

#
OBJECT_FILES_PATH="$SRC_OUTPUT/"*.o
if [ "$COMPILEMODE" = "release" ];then
EXE_OUTPUT="$SCRIPT_PATH/B_Release/Main"
EXECUTABLEFILE="../../B_Release/Main"
else
EXE_OUTPUT="$SCRIPT_PATH/B_Debug/Main"
EXECUTABLEFILE="../../B_Debug/Main"
fi

# MAIN_OUTPUT_PATH="$SCRIPT_PATH/"
# 变量结尾-------------------------------------------------------------
#
#
#
# -------------------------------------------------------------------
# 函数声明-------------------------------------------------------------
function CompilingGladFile {
    clang -c $1 -I $2 -o $3
}

function CompilingCppFile_DEBUG {
    clang++ -std=c++17 -Wall -Wextra \
    -c $1 \
    -D DEBUG \
    -D BASEPATH=\"/$SCRIPT_PATH\" \
    -I $2 \
    -o $3
}

function CompilingCppFile_RELEASE {
    clang++ -std=c++17 -Wall -Wextra -Wpedantic -Wshadow \
    -c $1 \
    -D RELEASE \
    -I $2 \
    -o $3
}

function LinkingMainCppFile {
    clang++  -std=c++17 -Wall -Wextra \
    $1 \
    -L $2 -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo \
    -o $3
}

function BatchCompileCppFiles {
    if [ "$COMPILEMODE" = "release" ];then
    for cpp in $SRC_PATH/*.cpp; do
    CPPNAME=${cpp%.cpp}.o
    CompilingCppFile_RELEASE "$cpp" "$INCLUDE_PATH" "$CPPNAME"
    done
    CompilingCppFile_RELEASE "$MAINCPP_PATH" "$INCLUDE_PATH" "$SRC_OUTPUT/Main.o"

    else
    for cpp in $SRC_PATH/*.cpp; do
    CPPNAME=$(basename ${cpp%.cpp}).o
    echo $cpp
    CompilingCppFile_DEBUG "$cpp" "$INCLUDE_PATH" "$SRC_OUTPUT/$CPPNAME"
    done
    CompilingCppFile_DEBUG "$MAINCPP_PATH" "$INCLUDE_PATH" "$SRC_OUTPUT/Main.o"
    fi
}
# 函数结尾-------------------------------------------------------------
# -------------------------------------------------------------------
#
#
# -------------------------------------------------------------------
 # 脚本入口------------------------------------------------------------
CompilingGladFile "$SRC_GLAD_PATH" "$INCLUDE_PATH/Window/Glad" "$SRC_OUTPUT/glad.o"

#批处理编译Cpp文件
BatchCompileCppFiles

#链接
cd $SRC_OUTPUT
LinkingMainCppFile "$OBJECT_FILES_PATH" "$LIB_PATH" "$EXE_OUTPUT"

#执行
$EXECUTABLEFILE

 # 脚本结尾------------------------------------------------------------
# -------------------------------------------------------------------
