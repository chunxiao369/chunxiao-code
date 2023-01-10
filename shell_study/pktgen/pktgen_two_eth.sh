#!/bin/sh
# pktgen.conf -- Sample configuration for send on two devices on a UP system

#rmmod pktgen
#sleep 2
#modprobe pktgen
#sleep 2

#ethtool -s eth0 speed 1000 duplex full autoneg on


if [ $# != 4 ] ; then 
echo "USAGE: ethx ethx delay count" 
exit 1; 
fi 

eth1=$1
eth2=$2
delay=$3
count=$4

ifconfig $eth1 promisc up
ifconfig $eth2 promisc up


function pgset() {
    local result
    echo $1 > $PGDEV
    result=`cat $PGDEV | fgrep "Result: OK:"`
    if [ "$result" = "" ]; then
         cat $PGDEV | fgrep Result:
    fi
}

function pg() {
    echo inject > $PGDEV
    cat $PGDEV
}

# On UP systems only one thread exists -- so just add devices 
# We use eth6

echo "Adding devices to run". 

PGDEV=/proc/net/pktgen/kpktgend_0
echo "Removing all devices"
pgset "rem_device_all" 
echo "add_device ${eth1}"
pgset "add_device ${eth1}" 

PGDEV=/proc/net/pktgen/kpktgend_1
echo "Removing all devices"
pgset "rem_device_all" 
echo "add_device ${eth2}"
pgset "add_device ${eth2}" 

pgset "max_before_softirq 10000"

#pgset "max_before_softirq 10000"


# Configure the individual devices
echo "Configuring devices"
PGDEV=/proc/net/pktgen/${eth1}
#sets the number of copies of the same packet
pgset "clone_skb 10000" 
echo "dst 192.168.0.2"
pgset "dst 192.168.0.2"
echo "pkt_size 64"
pgset "pkt_size 64"
echo "count ${count}"
pgset "count ${count}"
pgset "delay ${delay}"

PGDEV=/proc/net/pktgen/${eth2}
#sets the number of copies of the same packet
pgset "clone_skb 10000" 
echo "dst 192.168.0.3"
pgset "dst 192.168.0.3"
echo "pkt_size 64"
pgset "pkt_size 64"
echo "count ${count}"
pgset "count ${count}"
pgset "delay ${delay}"

#pgset "min_pkt_size 60" 
#pgset "max_pkt_size 60" 
#pgset "dst 10.10.10.2" 
#pgset "dst_mac 00:02:B3:9F:B4:25" 
#pgset "count 1000000"
# Time to run

PGDEV=/proc/net/pktgen/pgctrl
echo "Running... ctrl^C to stop"
pgset "start" 
echo "Done"

