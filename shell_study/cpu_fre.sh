#!/bin/bash

case "$2" in
1)
        echo "Change to High performance.."
        echo "performance" > /sys/devices/system/cpu/cpu$1/cpufreq/scaling_governor
        ;;

0)
        echo "Change to normal performance.."
        echo "conservative" > /sys/devices/system/cpu/cpu$1/cpufreq/scaling_governor
        ;;

*)
        printf "\nCPU Freq adjust tools v0.1 by xuchunxiao369@gmail.com\n"
        printf "usage:\n"
        printf "\t cpufreq.sh cpu_num 1 --> change to high performance mode\n"
        printf "\t cpufreq.sh cpu_num 0 --> change to normal performance mode\n"
        ;;

esac

