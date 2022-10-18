#install
clone libhtp to current dir
ubuntu: sudo apt-get install libcap-ng-dev libmagic-dev libluajit-5.1-dev
centos: 
yum install file-devel
yum install luajit-devel.x86_64

autogen.sh
./configure --enable-luajit --prefix=/home/lance/Project/open-source/suricata/install
make; make install
#how to use
suricata -c /etc/suricata/suricata-debian.yaml  -r /home/lance/Project/pcap_file/suricata/http_kv_json.pcap  -k none --init-errors-fatal

cargo install --force cbindgen
