ls -1 > a.sh
vim a.sh
%s/^/ tcprewrite --dlt=enet --enet-dmac=00:11:22:33:44:55 --enet-smac=66:77:88:99:AA:BB --infile=/g
%s/--infile=\(.*pcap\)$/--infile=\1 --outfile=\1/g
再修改名字
