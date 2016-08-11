
#!/bin/bash
i=0
rm -fr j_1.out
while [ $i -lt 10 ];
    do 
        echo "$i" >> j_1.out
        i=`expr $i + 1`
        #let i=i+1
    done



