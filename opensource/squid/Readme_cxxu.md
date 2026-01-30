pxy_bev_readcb pxy_bev_writecb
pxyconn.c pxy_conn_autossl_peek_and_upgrade函数是自动协议升级
pxy_conn_autossl_peek_and_upgrade 是实现“自动协议升级”的核心逻辑。

1. pxy_conn_autossl_peek_and_upgrade 函数的作用
该函数的主要作用是在不破坏原始 TCP 流的前提下，通过“偷看”（Peeking）数据包内容，探测是否存在 TLS 握手特征。

具体逻辑如下：

非破坏性查看：它使用 evbuffer_peek 从输入缓冲区读取数据，但不会从缓冲区中删除这些数据。这样如果发现不是 SSL 流量，数据仍能以明文形式继续转发。

特征匹配：它调用 ssl_tls_clienthello_parse 来尝试解析数据包头。如果检测到了 TLS 的 ClientHello 消息，说明客户端正尝试发起加密握手。

动态升级：一旦确定是 SSL 流量，它会设置 ctx->clienthello_found = 1 并触发“升级”流程——即在现有的 TCP 连接上叠加一个 OpenSSL 过滤器（bufferevent），从而将普通 TCP 代理转变为 SSL 拦截代理。

2. 普通 SSL 与 AutoSSL 的差别及代码处理
两者的本质区别在于加密开始的时机和处理逻辑的初始状态。

A. 概念上的差别
普通 SSL (https/ssl 模式)：属于“强制加密”。sslsplit 默认连接建立的第一刻起就是加密的，会立即发起 SSL 握手。

AutoSSL 模式：属于“机会性加密”或“延迟加密”。它允许连接先以明文 TCP 方式存在（例如 SMTP 协议初始的 220 欢迎语和 STARTTLS 指令），直到探测到 TLS 握手包时才进行拦截。

B. 代码中的区分处理逻辑
在 pxyconn.c 中，这种区分是通过上下文结构体 pxy_conn_ctx_t 中的几个关键标志位实现的：

初始化区分： 在 pxy_conn_ctx_new 函数中，系统会根据 proxyspec（代理配置）初始化标志位：

如果配置是 autossl，则 ctx->clienthello_search = 1。

这意味着系统进入“搜索模式”，暂时不启动 SSL 引擎，而是将连接视为普通 TCP。

读回调中的逻辑分流： 在读数据回调（通常是 pxy_bev_readcb 或相关逻辑）中，代码会检查这些标志：

搜索阶段：如果 clienthello_search 为 1，代码会反复调用 pxy_conn_autossl_peek_and_upgrade 检查每个新到达的数据块。

透明转发：在未找到 ClientHello 之前，所有数据被视为普通 TCP 载荷，直接透明转发给目标服务器。

触发升级：一旦探测成功，clienthello_search 被置为 0，clienthello_found 置为 1，随后代码会调用 bufferevent_openssl_filter_new 动态插入 SSL 处理层。

邮件/通用协议 (25/110/143/587)：非常适合使用 autossl，因为这些协议经常在明文交互一段时间后才通过 STARTTLS 切换到加密模式。

cd /home/lance/Project/xdp-ebpf/mitm
sudo /home/lance/Project/open-source/sslsplit/sslsplit -D -k ca.key -c ca.pem -l connect.log -S /tmp/sslsplit_log/ autossl 0.0.0.0 8587
sudo /home/lance/Project/open-source/sslsplit/sslsplit -D -k ca.key -c ca.pem -l connect.log -S /tmp/sslsplit_log/ autossl 0.0.0.0 8587 -I enp0s31f6 -T 10.60.110.53

sudo iptables -t nat -A PREROUTING -i br0 -p tcp --dport 587 -j REDIRECT --to-port 8587
sudo iptables -t nat -I OUTPUT -p tcp -m owner --uid-owner root --dport 587 -j RETURN

sudo iptables -t nat -D PREROUTING -i br0 -p tcp --dport 587 -j REDIRECT --to-port 8587
sudo iptables -t nat -D OUTPUT -p tcp -m owner --uid-owner root --dport 587 -j RETURN

https://github.com/DionGanteng/squid-openssl-ubuntu

# add diladele apt key
wget -qO - https://packages.diladele.com/diladele_pub.asc | sudo apt-key add -

# add new repo
echo "deb https://squid57.diladele.com/ubuntu/ focal main" \
    > /etc/apt/sources.list.d/squid57.diladele.com.list

# and install
apt-get update && apt-get install -y \
    squid-common \
    squid-openssl \
    squidclient \
    libecap3 libecap3-dev

sudo systemctl status squid.service
iptables -t nat -A PREROUTING -i br0 -p tcp --dport 443 -j REDIRECT --to-port 3129
iptables -t nat -I OUTPUT -p tcp -m owner --uid-owner root --dport 443 -j RETURN

iptables -t nat -D PREROUTING -i br0 -p tcp --dport 443 -j REDIRECT --to-port 3129
iptables -t nat -nL OUTPUT --line-numbers

squid FATAL: The sslcrtd_program helpers are crashing too rapidly, need help!
 /usr/lib/squid/security_file_certgen -c -s /var/spool/squid/ssl_db -M 4MB
Initialization SSL db...
Done

