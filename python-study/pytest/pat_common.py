
#!/usr/bin/python
import requests
import time 
ip_port = "http://192.168.3.3:11000"
pat_install = 0

def install():
    global pat_install
    if pat_install == 1:
        return
    url = ip_port + "/rule/install?emdebug=1"
    data  = {"install": "install"}
    res = requests.post(url, json=data)
    assert 0 == res.json()['status_code']
    time.sleep(1)
    pat_install = 1


