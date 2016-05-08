#!/bin/bash
function rand(){
    min=$1
    max=$(($2-$min+1))
    num=$(($RANDOM+1000000000))
    echo $(($num%$max+$min))
}

i=0
while [ $i -lt $1 ]
    do
    rnd=$(rand 0 5000000000)
    printf "%x\n" $rnd
    let i+=1
done
