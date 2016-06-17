tshark -r radius_1123_2.pcap -Y 's1ap.ENB_UE_S1AP_ID == 4727413' -w enb_s1apid_4727413.pcap
tshark -r EPC_Initial.pcap -2 -R 's1ap.ENB_UE_S1AP_ID == 0' -w a.pcap
tshark -r packet_dump.pcap-04-00-09-0508-cdr-creat-sess.pcap -R "gtpv2.imsi" -V -T fields -e gtpv2.imsi -e gtpv2.address_digits > 04-00-09-0508.imsi_msisdn
#udp包包含asc字符366a0014的
tshark -r A11.pcap -Y 'udp contains 36:6a:00:14' -w /tmp/bbb.pcap
#从udp头向后偏移16字节, 前四个字节是ac:1a:f1:69的
tshark -r /home/chunxiao/Project/packet/A11.pcap -Y 'udp[16:4]==ac:1a:f1:69' -w /tmp/bbb.pcap
#每个包判断udp payload的第一个字节, 第一个等于0x02, 第四个等于0x00, 第五个等于x22, 最后一个等于0x03
#未测试成功
tshark -r /home/chunxiao/Project/packet/A11.pcap -Y 'udp[8:] matches "^\\x02[\\x00-\\xff][\\x00-\\xff]\\x00\\x22[\\x00-\\xff]+\\x03$"' -w /tmp/bbb.pcap

