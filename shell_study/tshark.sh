tshark -r radius_1123_2.pcap -Y 's1ap.ENB_UE_S1AP_ID == 4727413' -w enb_s1apid_4727413.pcap
tshark -r EPC_Initial.pcap -2 -R 's1ap.ENB_UE_S1AP_ID == 0' -w a.pcap
tshark -r packet_dump.pcap-04-00-09-0508-cdr-creat-sess.pcap -R "gtpv2.imsi" -V -T fields -e gtpv2.imsi -e gtpv2.address_digits > 04-00-09-0508.imsi_msisdn
