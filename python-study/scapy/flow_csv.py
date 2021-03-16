import socket
from scapy import *
from scapy.utils import rdpcap

edges_file = "flows.csv"
pcap_file = "wechat.pcap"

try:
    print('[+] Reading and parsing pcap file: %s' % pcap_file)
    a = rdpcap(pcap_file)
    #print(a.summary())
    for k, v in a.sessions().items():
        print(k,v)


except Exception as e:
    print('Something went wrong while opening/reading the pcap file.' \
          '\n\nThe error message is: %s' % e)
    exit(0)

sessions = a.sessions()

print('[+] Writing to edges.csv')
f1 = open(edges_file, 'w')
f1.write('source,target,protocol,sourceport,destinationport,'
         'num_of_packets\n')

for k, v in sessions.items():
    print(k, "xxxxxxxxxx", v)
    tot_packets = len(v)
    if "UDP" in k:
        proto, source, flurp, target = k.split()
        srcip, srcport = source.split(":")
        dstip, dstport = target.split(":")
        f1.write('%s,%s,%s,%s,%s,%s\n' % (srcip, dstip, proto, srcport,
                                          dstport, tot_packets))
        continue

    elif "TCP" in k:
        proto, source, flurp, target = k.split()
        srcip, srcport = source.split(":")
        dstip, dstport = target.split(":")
        f1.write('%s,%s,%s,%s,%s,%s\n' % (srcip, dstip, proto, srcport,
                                          dstport, tot_packets))
        continue

    elif "ICMP" in k:
        continue  # Not bothered about ICMP right now

    else:
        continue  # Or any other 'weird' pacakges for that matter ;)

print('[+] Closing the edges file')
f1.close()
