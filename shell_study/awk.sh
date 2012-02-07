#!/bin/sh
awk -F\  '{ sum += $2 }; {print sum;} END { print sum }' b.txt
