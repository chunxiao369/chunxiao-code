# filename: elem_http.py
import requests
import pytest 
import pat_common as patc

@pytest.mark.run(order=0)
def test_elem_dns_pre():
    url = patc.ip_port + "/rule/install?emdebug=1"
    data  = {"install": "uninstall"}
    res = requests.post(url, json=data)
    
    url = patc.ip_port + "/rule/add?emdebug=1"
    data  = {"rule_info":[{"rule_id":1,"entry_id":12,"elem_rule":{"name":"dns.domain", "regex":"facebook"}}]}
    res = requests.post(url, json=data)
    assert 0 == res.json()['status_code']
    assert 'ok' in res.json()['msg']
    
    url = patc.ip_port + "/action/set?emdebug=1"
    data  = {"action_rule":[{"rule_id":1, "entry_id":12, "mode":"packet_dump"}]}
    res = requests.post(url, json=data)
    assert 0 == res.json()['status_code']

@pytest.mark.run(order=2)
def test_install():
    patc.install()

@pytest.mark.run(order=-2)
def test_elem_dns_result():
    #loadpcap pcap in /tmp/elem_dns/
    url = patc.ip_port + "/pcap/load?emdebug=1"
    data  = {"path":"/tmp/elem_dns/"}
    res = requests.post(url, json=data)
    assert 0 == res.json()['status_code']

    url = patc.ip_port + "/rule/counter/entry?emdebug=1"
    data  = {"info":[{"rule_id":1,"entry_id":12}]}
    res = requests.post(url, json=data)
    assert 0 == res.json()['status_code']
    assert 0 == res.json()['data']['counter_info'][0]['match_pkts']
