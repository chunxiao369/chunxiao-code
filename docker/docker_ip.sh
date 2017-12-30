#!/bin/sh

ipname=`docker inspect --format='{{.NetworkSettings.IPAddress}}' namenode`
ip1=`docker inspect --format='{{.NetworkSettings.IPAddress}}' datanode01`
ip2=`docker inspect --format='{{.NetworkSettings.IPAddress}}' datanode02`
ip3=`docker inspect --format='{{.NetworkSettings.IPAddress}}' datanode03`

echo $ipname namenode
echo $ip1    datanode01
echo $ip2    datanode02
echo $ip3    datanode03
