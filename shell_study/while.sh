#!/bin/sh

i=4000
while [ $i -le 5000 ];do
		ssh -p $i cxxu@61.183.0.93
		let i+=1
done
