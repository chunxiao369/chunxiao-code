docker run -t -i -d -p 9877:9877 diceone/capanalysis

docker run -v /path/to/host/pcap:/data/pcap:rw k0st/moloch
docker run -d -v /home/lance/Project/pcap_file/:/data/pcap:rw -p 8005:8005 k0st/moloch
docker run -v /home/lance/Project/pcap_file/:/data/pcap:rw -p 8005:8005 --name moloch_test k0st/moloch
docker exec containerid /data/moloch/bin/moloch-capture -r /data/pcap/sniff.pcap -t mysniff
