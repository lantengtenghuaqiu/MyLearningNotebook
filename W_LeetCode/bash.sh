echo "🚀Start Compiling...."

set -e

DIFFICULTY="$1"

REALPATH=$(realpath "$0")
PATHDIR=$(dirname "$REALPATH")

error_exit() {
    echo -e "\n❌ Script compiling with something wrong!"
    echo -e "❌ Failed command:$BASH_COMMAND"
    echo -e "⛔ Script is terminated!\n"
    exit 1
}
trap error_exit ERR

read -p "FIle name : " filename


if [ "$DIFFICULTY" = "easy" ];then
    BASEPATH="$PATHDIR/Easy"
    FINALPATH="$BASEPATH/$filename"

    if [ -d "$FINALPATH" ]; then
        echo -e "$FINALPATH"
        clang++ $FINALPATH/main.cpp -o $FINALPATH/main
        cd $FINALPATH
        clang++ ./main.cpp -o ./main
        ./main
    else 
        echo "None of the file...$FINALPATH"
    fi
    
elif [ "$DIFFICULTY" = "normal" ];then
    BASEPATH=$PATHDIR"/Normal"
    FINALPATH="$BASEPATH/$filename"

    if [ -d "$FINALPATH" ]; then
        echo -e "$FINALPATH"
        clang++ $FINALPATH/main.cpp -o $FINALPATH/main
        cd $FINALPATH
        clang++ ./main.cpp -o ./main
        ./main
    else 
        echo "None of the file...$FINALPATH"
    fi
fi
