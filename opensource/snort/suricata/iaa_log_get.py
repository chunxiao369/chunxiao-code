#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
import os
import sys
import commands
import getopt
import csv

config = {
    "server":"",
    "client":"",
    "verbose":"",
}

def ver_print(eachline, verbose):
    if (verbose):
        print eachline,
    return

def check_cs(server, client, verbose):
    if (server == '' and client == ''):
        print "server and client is NULL!!!"
        return
    fd = open("/var/log/embed_iaa/dns.log", 'r')
    match = 0
    for eachline in fd:
        try:
            #print eachline
            str_split = eachline.split(' || ', 9)
            if (client == ''):
                if (str_split[7].strip() != "" and str_split[7].find(server) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
            elif (server == ''):
                if (str_split[1].find(client) >= 0 or str_split[3].find(client) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
            else:
                if ((str_split[1].find(client) >= 0 or str_split[3].find(client) >= 0) \
                    and str_split[7].find(server) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
        except:
            continue
    fd.close()
    print "IAA  dns log match", match, "times"

    match = 0
    fd = open("/var/log/embed_iaa/http.log", 'r')
    for eachline in fd:
        try:
            #print eachline
            str_split = eachline.split(' || ', 7)
            if (client == ''):
                if (str_split[5].strip() != "" and str_split[5].find(server) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
            elif (server == ''):
                if (str_split[1].find(client) >= 0 or str_split[3].find(client) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
            else:
                if ((str_split[1].find(client) >= 0 or str_split[3].find(client) >= 0) \
                    and str_split[5].find(server) >= 0):
                    match += 1
                    ver_print(eachline, verbose)
        except:
            continue
    fd.close()
    print "IAA http log match", match, "times"

    return


def usage():
    print( "Usage:\n    %s -s servername -c client_ip -v" % (sys.argv[0]))
    print( "example: %s -a" % (sys.argv[0]))
    print( "         %s -s weixin.com" % (sys.argv[0]))
    print( "         %s -c 192.168.3.4 -v" % (sys.argv[0]))
    print( "         %s -c 192.168.3.4 -s weixin.com" % (sys.argv[0]))

if __name__ == '__main__':
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hs:c:v', ['help'])
        for option, value in opts:
            if  option in ["-h","--help"]:
                #usage()
                sys.exit(0)
            elif option in ['-s']:
                config["server"] = value
            elif option in ['-c']:
                config["client"] = value
            elif option in ['-v']:
                config["verbose"] = 1 
        check_cs(config["server"], config["client"], config["verbose"])
    except:
        usage()
        sys.exit(0)
