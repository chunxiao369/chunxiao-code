sudo c-icap -f /etc/c-icap/c-icap.conf -D -N
# c-icap-client -i 10.60.110.53 -p 1344 -s virus_scan -f /tmp/test.txt -req http://example.com/upload -v
virus scan 需要在服务端使用clamav，配置有点麻烦。

# c-icap-client -i 10.60.110.53 -p 1344  -s url_check -req http://www.badsite.com -v
ICAP server:10.60.110.53, ip:10.60.110.45, port:1344

No modification needed (Allow 204 response)

ICAP HEADERS:
	ICAP/1.0 204 Unmodified
	Server: C-ICAP/0.5.3
	Connection: keep-alive
	ISTag: CI0001-XXXXXXXXX
	X-ICAP-Profile: default
	X-Attribute: ALL
	X-Response-Info: ALLOWED
	X-Response-Desc: URL category ALL is ALLOWED

REQMOD HEADERS:
	GET http://www.badsite.com HTTP/1.0
	Date: Tue Jan 27 15:23:52 2026
	User-Agent: C-ICAP-Client/x.xx

ls /usr/lib/x86_64-linux-gnu/c_icap/
bdb_tables.so  clamd_mod.so     shared_cache.so           srv_echo.so   srv_url_check.so  virus_scan.so
clamav_mod.so  dnsbl_tables.so  srv_content_filtering.so  srv_ex206.so  sys_logger.so

