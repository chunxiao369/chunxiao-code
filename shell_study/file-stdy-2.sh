
file -b * | grep "broken symbolic link" > temp.txt
sed -e "s%.*\`\(.*\)'%\1%" temp.txt > temp2.txt
rm -f temp.txt
cat temp2.txt

sed -e "s%.*\/\(.*\)%\1%" temp2.txt > temp.txt
#sed -e "s%.*\/\(.*\)%\1%" temp.txt > temp2.txt
cat temp.txt
sed -e "s%^%rm -f %" temp.txt > temp.sh

. temp.sh
rm -f temp.txt
rm -f temp2.txt
rm -f temp.sh

