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

mkdir -p ./objs;


if [ "$COMPILEMODE" = "release" ];then
echo "🔧 Compile in the debug mode"
 
elif [ "$COMPILEMODE" = "debug" ];then
echo "🔧 Compile in the debug mode"
clang -c ../../src/glad.c  -I ../../includes -o ./objs/glad.o 
clang++ -c ../main.cpp -I ../../includes -D DEBUG -std=c++17 -o ./objs/main.o 
clang++ ./objs/main.o ./objs/glad.o \
        -L ../../libs/mac -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo \
        -std=c++17 \
        -o ../main \
        -Wl,-rpath,"/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/libs/mac"
else
echo "🔧 Compile in the debug mode"
clang -c ../../src/glad.c  -I ../../includes -o ./objs/glad.o 
clang++ -c ../main.cpp -I ../../includes -std=c++17 -o ./objs/main.o 
clang++ ./objs/main.o ./objs/glad.o \
        -L ../../libs/mac -lglfw -lassimp.6 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo \
        -o ../main \
        -std=c++17 \
        -Wl,-rpath,"/Users/bytedance/Desktop/C++/IOLSHS/RayTracingOne/D_OpenGL/libs/mac"
fi



echo -e "🎉 Successfully, to compile the files....";

echo -e "🥰 Runing the executable file...";

# Runing--------------------
../main