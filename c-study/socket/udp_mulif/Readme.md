sudo ./a.out 9876
UDP socket bound to enp0s31f6:9876
UDP socket bound to br0:9876
[IN:br0] From 192.168.3.1:59346 -> local port 0, len=9, data="hello..."
[IN:enp0s31f6] From 10.60.110.53:34489 -> local port 0, len=7, data="hello0

another vm or host
echo "hello0" | sudo nc -u 10.60.110.45 9876
