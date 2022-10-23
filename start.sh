FILE=$(basename $1 | rev | cut -c3- | rev)
gcc -I./include -L./lib/linux $1 -lm -lglfw3 -o $FILE
mkdir -p out
mv $FILE out/$FILE
if [ $# -eq 2 ]
then
    if [ $2 = "--run" ]
    then
        ./out/$FILE
    fi
fi
