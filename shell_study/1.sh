
#!/bin/sh
#WORD="a b c d e f g h i j l m n o p q r s t u v w x y z"
#WORD=$@
#echo "$@"
#for i in $WORD ; do 
#    echo $i 
#done
echo $3
for i in `seq 10`;
    do echo $i
done
echo $SHLVL
return
<< chunxiao
while [$i -lt 10 ];
    do 
        echo "$i"
        let i=i+1
    done
chunxiao


<< chunxiao
WORD=$*
echo "$*"
for i in $WORD ; do 
    echo $i 
done
chunxiao

