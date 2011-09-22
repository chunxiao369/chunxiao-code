#!/bin/sh
#service --status-all | grep "fped" | grep "started"

usage()
{
    echo "This is a test in function!!!"
}

ls | grep "fnc_std" | grep "sh"

if [ $? -eq 0 ]; then
    echo " fped service has found! " 
    usage
else 
    echo 2
fi



