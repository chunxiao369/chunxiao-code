#!/bin/sh
arp_mac1=`arp | sed -n '2p' | awk '{print $3}'`;echo $arp_mac1
