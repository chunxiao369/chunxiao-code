#/bin/sh
# find the invalid link file and delete it 

file -b /home/pa2009/modules/include/* | grep "broken symbolic link" > temp.txt

sed -e "s%.*\`\(.*\)'%\1%" temp.txt > temp.sh
rm -f temp.txt
sed -e "s%^%rm -f %" temp.sh > temp2.sh
rm -f temp.sh
. temp2.sh

