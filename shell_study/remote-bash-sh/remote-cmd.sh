#/bin/sh

#sshpass -f ./passwd -- ssh root@10.0.74.42 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00001_20211011121113.pcap' &
#sshpass -f ./passwd -- ssh root@10.0.74.43 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00002_20211011122110.pcap' &
#sshpass -f ./passwd -- ssh root@10.0.74.44 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00003_20211011123147.pcap' &
#sshpass -f ./passwd -- ssh root@10.0.74.45 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00004_20211011124228.pcap' &
#sshpass -f ./passwd -- ssh root@10.0.74.46 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00005_20211011125349.pcap' &
#sshpass -f ./passwd -- ssh root@10.0.74.47 -- 'tcpreplay -i ens9f1 -M 5000 /mnt/disk0/per_100G_00006_20211011130624.pcap' &

sshpass -f ./passwd -- ssh root@10.0.74.42 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00001_20211011121113.pcap' &
sshpass -f ./passwd -- ssh root@10.0.74.43 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00002_20211011122110.pcap' &
sshpass -f ./passwd -- ssh root@10.0.74.44 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00003_20211011123147.pcap' &
sshpass -f ./passwd -- ssh root@10.0.74.45 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00004_20211011124228.pcap' &
sshpass -f ./passwd -- ssh root@10.0.74.46 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00005_20211011125349.pcap' &
sshpass -f ./passwd -- ssh root@10.0.74.47 -- 'tcpreplay -i ens9f1 -t /mnt/disk0/per_100G_00006_20211011130624.pcap' &
