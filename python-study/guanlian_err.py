#!/usr/bin/env python
# -*- coding: cp936 -*-
#hello
import os
from struct import unpack,pack
import socket
import sys,getopt
from copy import deepcopy
import time
    

def decode_pkt(data):
    pkt_num=0
    pkt_data=[]
    pkt_head={}
    offset=24
#print len(data)
    while (offset<len(data)):
        pkt_head['GMTtime']=data[offset:offset+4]
        pkt_head['Microtime']=data[offset+4:offset+8]
        pkt_head['caplen']=data[offset+8:offset+12]
        pkt_head['len']=data[offset+12:offset+16]
        pkt_len=unpack('I',pkt_head['len'])[0]

        hex_data=display_hex(data[offset+16:offset+16+pkt_len])
        pkt_data.append(hex_data)
        
        
        offset=offset+pkt_len+16
        pkt_num+=1
#print pkt_num    
    return pkt_data

        

def fw(string):
    fw=open(result,'a')
    fw.write(string+'\n')
    fw.close()

def display_hex(frame):
    hex_str=''
    for j in frame:
        temp=ord(j)
        temp=hex(temp)
        if len(temp)==3:
            temp = temp.replace('0x','0x0')
        hex_str=hex_str+temp
    hex_str=hex_str.replace('0x',' ')
    return hex_str


def check_pkt(pkt):
    msisdn=time=userip=action=imsi=imei=cgi=pro=apn=guti=tai=tai_list=0
    index=0
    while (index<len(pkt)):
        pkt_offset=108
        pkt[index]=pkt[index].replace(' ','')
        str_pcap=pkt[index][pkt_offset:]
        list_type=check_type(str_pcap,msisdn,time,userip,action,imsi,imei,cgi,pro,apn,guti,tai,tai_list)
        msisdn=list_type[0]
        time=list_type[1]
        userip=list_type[2]
        action=list_type[3]
        imsi=list_type[4]
        imei=list_type[5]
        cgi=list_type[6]
        pro=list_type[7]
        apn=list_type[8]
        guti=list_type[9]
        tai=list_type[10]
        tai_list=list_type[11]
        
                                            
        index+=1


    print 'the pkts of msisdn is %s'%msisdn
    fw('the pkts of msisdn is %s'%msisdn)
    print 'the pkts of time is %s'%time
    fw('the pkts of time is %s'%time)
    print 'the pkts of userip is %s'%userip
    fw('the pkts of userip is %s'%userip)
    print 'the pkts of action is %s'%action
    fw('the pkts of action is %s'%action)
    print 'the pkts of imsi is %s'%imsi
    fw('the pkts of imsi is %s'%imsi)
    print 'the pkts of imei is %s'%imei
    fw('the pkts of imei is %s'%imei)
    print 'the pkts of ecgi is %s'%cgi
    fw('the pkts of ecgi is %s'%cgi)
    print 'the pkts of EUTRAN is %s'%pro
    fw('the pkts of EUTRAN is %s'%pro)
    print 'the pkts of apn is %s'%apn
    fw('the pkts of apn is %s'%apn)
    print 'the pkts of guti is %s'%guti
    fw('the pkts of guti is %s'%guti)
    print 'the pkts of tai is %s'%tai
    fw('the pkts of tai is %s'%tai)
    print 'the pkts of tai_list is %s'%tai_list
    fw('the pkts of tai_list is %s'%tai_list)
    print 'the total pkts is %s'%index
    fw('the total pkts is %s'%index)

    '''
    print 'the percent of imsi is %.2f%%'%(float(imsi)/index*100)
    fw('the percent of imsi is %.2f%%'%(float(imsi)/index*100))
    print 'the percent of msisdn is %.2f%%'%(float(msisdn)/index*100)
    fw('the percent of msisdn is %.2f%%'%(float(msisdn)/index*100))
    print 'the percent of IMEI is %.2f%%'%(float(imei)/index*100)
    fw('the percent of IMEI is %.2f%%'%(float(imei)/index*100))
    print 'the percent of GUTI is %.2f%%'%(float(guti)/index*100)
    fw('the percent of GUTI is %.2f%%'%(float(guti)/index*100))
    print 'the percent of TAI is %.2f%%'%(float(tai)/index*100)
    fw('the percent of TAI is %.2f%%'%(float(tai)/index*100))
    print 'the percent of ECGI is %.2f%%'%(float(cgi)/index*100)
    fw('the percent of ECGI is %.2f%%'%(float(cgi)/index*100))
    '''



def hex2dec(string_num):
    return str(int(string_num.upper(), 16))

def hex2ip(string):
    str1=hex2dec(string[0:2])+'.'+hex2dec(string[2:4])+'.'+hex2dec(string[4:6])+'.'+hex2dec(string[6:8])
    return str1


def ascii2str(string):
        str_2=''
        dic_1={'61':'a','62':'b','63':'c','64':'d','65':'e','66':'f'}
        while 1:
                str_1=string[0:2]
        #       print "str_1 is:",str_1
                if str_1[0]=='3':
                        str_2=str_2+str_1[1]
        #               print "str_2 is:",str_2
                else:
                        str_2=str_2+dic_1[str_1]
        #               print "str_2 is:",str_2
                string=string[2:]
                if string == '':
                        break
        return str_2


def check_type(string,msisdn,time,userip,action,imsi,imei,cgi,pro,apn,guti,tai,tai_list):
    imsi_num=0
    str_origial=string
    dic=[{'msisdn':'01'},{'time':'02'},{'userip':'03'},{'action':'05'},
        {'imsi':'09'},{'imei':'oa'},{'cgi':'0d'},{'pro':'0e'},{'apn':'0f'},{'guti':'10'},
         {'tai':'11'},{'tai_list':'12'}]
    list_value=['01','02','03','05','06','07','1d','06','07','1d','09','0a','0c','0e','0f','10','11','12','13','14','15','16','17','18','19','1a','1b','1c','1e','1f']
    list_key=['msisdn','time','userip','action','sgw-teid','eNB-teid','EBI','imsi','imei','cgi','pro','apn','guti','tai','tai_list',
             'master-slave','sms','smsID','plmn ID','cellID','sms-time','eNBip','MMEip','sgwip','pgwip','src_port','dst_port']
    



    dic_action={'01':'01-PDN online','02':'02-PDN release','03':'03-periodic TAU','05':'05-force TAU','06':'06-SMS','07':'07-CSFB','08':'08-Initial Attach',
                 '09':'09-Detach','0a':'0a-Handover','0b':'0b-Bearer Modify','0c':'0c-PDN active','0d':'0d-dedicated bearer update','0e':'0e-Bearer Deactive',
                 '0f':'0f-Service Request'}

    dic_net={'01':'3G-UTRAN','02':'2G-GERAN','06':'4G-EUTRAN'}

    print "#############################################################"
    for i in range(len(list_value)):
        if string!='':
            type_tlv=string[0:2]
            len_tlv=string[2:4]
            len_tlv=int(len_tlv,16)*2

            if type_tlv=='01':
                msisdn+=1
                print "------------------------------>"
                print "the msisdn is:",hex2dec(string[4:len_tlv])
                print "------------------------------>"
            elif type_tlv=='02':
                time+=1
            elif type_tlv=='03':
                userip+=1
            #    print "------------------------------>"
                print "the userip is:",hex2ip(string[4:len_tlv])
                print "------------------------------>"
        
        
        #    elif type_tlv=='05':
        #        action+=1
        #        print "the action type is:",dic_action[string[4:len_tlv]]
        #        print "------------------------------>"

        #    elif type_tlv=='06':
        #        print "the sgw-teid  is:",string[4:len_tlv]
        #        print "------------------------------>"
        #    elif type_tlv=='07':
        #        print "the eNB-teid  is:",string[4:len_tlv]
        #        print "------------------------------>"
        #    elif type_tlv=='1d':
        #        print "the EBI  is:",string[4:len_tlv]
        #        print "------------------------------>"
        #    elif type_tlv=='06':
        #        print "the sgw-teid-1  is:",string[4:len_tlv]
        #        print "------------------------------>"
        #
        #    elif type_tlv=='07':
        #        print "the eNB-teid-2  is:",string[4:len_tlv]
        #        print "------------------------------>"
        
        #    elif type_tlv=='1d':
        #        print "the EBI  is:",string[4:len_tlv]
        #        print "------------------------------>"
            
        

            elif type_tlv=='09':
                imsi+=1
                imsi_num+=1
                print "the imsi is:",hex2dec(string[4:len_tlv])
                print "------------------------------>"
            elif type_tlv=='0a':
                imei+=1
                print "the imei is:",ascii2str(string[4:len_tlv])
                print "------------------------------>"

            '''

            elif type_tlv=='0c':
                cgi+=1
                print "the 3G CGI is:",ascii2str(string[4:len_tlv])
                print "------------------------------>"
            elif type_tlv=='0e':
                pro+=1
                print "the Access Network Type is:",dic_net[string[4:len_tlv]]
                print "------------------------------>"
            elif type_tlv=='0f':
                apn+=1
                print "the APN is:",string[4:len_tlv]
                print "------------------------------>"
            elif type_tlv=='10':
                guti+=1
                print "the guti is:",string[4:len_tlv]
                print "------------------------------>"
            elif type_tlv=='11':
                tai+=1
                print "the tai is:",string[4:len_tlv]
                print "------------------------------>"
            elif type_tlv=='12':
                tai_list+=1
                print "the tai_list is:",string[4:len_tlv]
                print "------------------------------>"
            elif type_tlv=='13':
                print "the master-slave mode is:",string[4:len_tlv]
                print "------------------------------>"

            elif type_tlv=='14':
                print "the SMS content is:",string[4:len_tlv]
                print "------------------------------>"
            
            elif type_tlv=='15':
                print "the SMS msisdn is:",hex2dec(string[4:len_tlv])
                print "------------------------------>"

            elif type_tlv=='16':    
                print "the PLMN id is:",string[4:len_tlv]
                print "------------------------------>"

            elif type_tlv=='17':
                print "the Cell id is:",string[4:len_tlv]
                print "------------------------------>"

            elif type_tlv=='18':
                print "the sms time is:",string[4:len_tlv]
                print "------------------------------>"

            elif type_tlv=='19':
                print "the eNB ip is:",hex2ip(string[4:len_tlv])
                print "------------------------------>"

            elif type_tlv=='1a':
                print "the MME ip is:",hex2ip(string[4:len_tlv])
                print "------------------------------>"

            elif type_tlv=='1b':
                print "the SGW ip is:",hex2ip(string[4:len_tlv])
                print "------------------------------>"

            elif type_tlv=='1c':
                print "the PGW ip is:",hex2ip(string[4:len_tlv])
                print "------------------------------>"
            elif type_tlv=='1e':
                print "the src_port ip is:",hex2dec(string[4:len_tlv])
                print "------------------------------>"
            elif type_tlv=='1f':
                print "the dst_port ip is:",hex2dec(string[4:len_tlv])
                print "------------------------------>"
            '''

            string=string[len_tlv:]
    
    if imsi_num==0:
        pass
        #print str_origial
    return msisdn,time,userip,action,imsi,imei,cgi,pro,apn,guti,tai,tai_list

def Usage():
    print "***.py Usage:"
    print "-r --read:   input pcap  which need to decap"
    print "-w --write:   output file  which record the result "
    print "-h --help:    print help message."


if __name__=="__main__":
    shortargs = "hr:w:"
    longargs = ["help","read=",'write=']

    try:
        opts, args = getopt.getopt(sys.argv[1:], shortargs, longargs) 
        print "opts:",opts
        print "args:",args
    except getopt.GetoptError , err:
        print str(err)
        sys.exit(2)

    for o, value in opts: 
        if o in ("-h","--help"):
            Usage()
            sys.exit()
        elif o in ("-r","--read"):
            print "the read pcap is:%s"%value
        elif o in ("-w","--write"):
            print "the write result file is:%s"%value
        else:
            print "command line error: no such option: %s" % value

    start=time.ctime()
    print 'the start time is %s'%start
    dire=os.getcwd()
    s1mme_cdr=os.path.join(dire,sys.argv[2])
    result=os.path.join(dire,sys.argv[4])
    fp=open(s1mme_cdr,'rb')
    data=fp.read()
    fp.close()
    pkt=decode_pkt(data)
#    print pkt
    if os.path.isfile(result):
        os.remove(result)
        fw('the start time is %s'%start)
        check_pkt(pkt)
    else:
        fw('the start time is %s'%start)
        check_pkt(pkt)
    
    print 'the end time is %s'%time.ctime()
    fw('the end time is %s'%time.ctime())
