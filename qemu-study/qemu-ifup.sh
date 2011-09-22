#/bin/sh

### 注释掉原来的脚本
## ifconfig $1 10.8.0.1
## 获取原来主系统 eth0 的 IP 和网关
IP=`ifconfig eth0|grep "inet addr"|awk '{print $2}'|sed 's/addr://'`
GATEWAY=`route -n|grep ^0.0.0.0|awk '{print $2}'`
## 删除 eth0 的 IP
ip addr del $IP dev eth0
## 激活 tap0 ($1 是qemu传递过来的网卡名称，即tap0)
ip link set $1 up
## 停止并删除存在的网桥
ip link set br0 down &>/dev/null
brctl delbr br0 &>/dev/null
## 添加网桥接口 br0
brctl addbr br0
## 将 eth0 加入网桥
brctl addif br0 eth0
## 将 tap0 加入网桥
brctl addif br0 $1
## 启动网桥
ifconfig br0 $IP up
## 添加默认路由
route add default gw $GATEWAY

