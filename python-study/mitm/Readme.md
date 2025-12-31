iptables -t nat -A PREROUTING -i br0 -p tcp --dport 80 -j REDIRECT --to-port 8080
iptables -t nat -A PREROUTING -i br0 -p tcp --dport 443 -j REDIRECT --to-port 8080
sudo iptables -t nat -D PREROUTING -i br0 -p tcp --dport 80 -j REDIRECT --to-port 8080
sudo iptables -t nat -D PREROUTING -i br0 -p tcp --dport 443 -j REDIRECT --to-port 8080

sudo iptables -t nat -I OUTPUT -p tcp -m owner --uid-owner root --dport 80 -j RETURN
sudo iptables -t nat -I OUTPUT -p tcp -m owner --uid-owner root --dport 443 -j RETURN

curl failed to verify the legitimacy of the server and therefore could not
establish a secure connection to it. To learn more about this situation and
how to fix it, please visit the web page mentioned above.

CERT Tapioca 是一款用于使用中间人攻击（MITM）技术测试移动应用程序或任何其他应用程序的实用工具。CERT Tapioca 的开发由美国陆军武器研究、开发和工程中心 (ARDEC) 以及美国国土安全部 (DHS) 资助。

openssl x509 -in /tmp/mitmproxy-ca-cert.pem -pubkey -noout
openssl x509 -in /tmp/mitmproxy-ca-cert.pem -pubkey -noout > /tmp/mitmproxy-ca-public.key
openssl pkey -in /tmp/mitmproxy-ca-public.key -pubin -text -noout
python3.8 mitmdump --mode transparent --showhost -s a.py
sudo python3.8 /usr/bin/mitmdump --mode transparent -s a.py
