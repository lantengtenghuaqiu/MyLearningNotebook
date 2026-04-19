set -e

echo "当前脚本运行目录：$(pwd)"

echo -e "🚀Compiling......."
clang++ main.cpp \
-I ../includes \
-o test \
-Wl,-rpath,"/Users/bytedance/Desktop/C++/IOLS&%@HS/Ray-Tracing-One-Week/D_OpenGL/libs/Assimp/Mac"
echo -e "🚀Compiling is finished."

./test

