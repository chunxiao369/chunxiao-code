
#sed -e '1,10d' -e 's/version/xuchunxiao/g' a.txt 
#sed -n -e '/version/p' a.txt

#abc=`sed -e "s%.*version=\(\[0-9\]\{3\}\).*%\1%" a.txt`
abc=`cat a.txt | grep version` 
def=`grep version a.txt | sed -e "s%version=\([0-9]\{2\}\).*%\1%"` 
echo $abc
echo $def
