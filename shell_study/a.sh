#!/bin/bash  
cat notes邮箱.csv | awk -F, '{A=","};{print $2 }' | grep -v "E-mail"|tr -s '\n' > email-list.google
#cat email|grep -v "E-mail"|tr -s '\n'>email-list>email
cat email-list.google | awk -F@ '{A=","};{E="Email:"};{N="Name:"};{print $1 A A A A A $0}' > Evolution.csv
rm -f email-list.google
