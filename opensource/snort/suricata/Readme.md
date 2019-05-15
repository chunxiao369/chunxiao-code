/home/lance/Project/open-source/suricata/install/bin/suricata -c /home/lance/Project/pcap_file/suricata/suricata-debian.yaml -r /home/lance/Project/pcap_file/suricata/meituanwaimai_android_V6.6.7_1218_open_less.pcap

/home/lance/Project/open-source/suricata/install/bin/suricata -c embed_iaa.yaml -r /home/lance/Project/pcap_file/suricata/meituanwaimai_android_V6.6.7_1218_open_less.pcap

/home/lance/Project/open-source/suricata/install/bin/suricata -c /home/lance/Project/chunxiao-code/opensource/snort/suricata/suricata-debian-extract.yaml -r /home/lance/Project/pcap_file/suricata/webmail_sina_send2.pcap -k none

./configure --enable-luajit --prefix=/home/lance/Project/open-source/suricata/install/
apt-get install libyaml-dev
apt-get install libpcap-dev
apt-get install libluajit-5.1-dev

