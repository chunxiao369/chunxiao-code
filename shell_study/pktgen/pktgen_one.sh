#!/bin/sh
# pktgen.conf -- Sample configuration for send on two devices on a UP system

#modprobe pktgen

#ethtool -s eth0 speed 1000 duplex full autoneg on

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
# We use eth3

echo "Adding devices to run". 

PGDEV=/proc/net/pktgen/kpktgend_0
echo "Removing all devices"
pgset "rem_device_all" 
echo "add_device eth3"
pgset "add_device eth3" 
pgset "max_before_softirq 10000"

#pgset "max_before_softirq 10000"


# Configure the individual devices
echo "Configuring devices"
PGDEV=/proc/net/pktgen/eth3

#sets the number of copies of the same packet
pgset "clone_skb 10000" 
pgset "dst 10.0.1.2" 
#echo "dst 192.168.0.**"
#pgset "dst 192.168.0.**"
echo "pkt_size 512"
pgset "pkt_size 512"
echo "count 10000"
pgset "count 10000"

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

