#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
import os
import sys
import commands
import getopt
import redis

def test_redis(method):
    print "cxxu aaaaa"
    r=redis.Redis(host='172.17.0.3', port=6379, db=0)
    print "Connection to server successfully! \n"
    print r.set('13764769198','cxxu')
    print r.get('13764769198')
    print r.keys()
    print r.dbsize()
    r.delete('13764769198')
    r.save()
    r.flushdb()
    print r.keys()
    print r.dbsize()
    return

    print r.hset('13764769198c', 'cxxu abc embedway', 'weibo')
    print r.hgetall('13764769198c')
    print r.hset('13764769198c', 'addr', '陈行公路')
    print r.hgetall('13764769198c')
    print r.hget('13764769198c', 'cxxu abc embedway')
    print r.hkeys('13764769198c')

    print r.sadd('cir:game:lol', 'user:debug')
    print r.sadd('cir:game:lol', 'user:leo')
    print r.sadd('cir:game:lol', 'user:guo')
    print r.sadd('cir:game:football', 'user:aaaa')
    print r.sadd('cir:game:football', 'user:bbbb')
    print r.sadd('cir:game:football', 'user:cccc')
    print r.smembers('cir:game:lol')
    print r.smembers('cir:game:football')
    r.sadd('cir:game:football', 'user:guo')
    print r.sinter('cir:game:football', 'cir:game:lol')
    print r.sunion('cir:game:football', 'cir:game:lol')

    print r.keys()

    ## Stored data in list (lpush)
    listDict = {'tutorial-list':['Redis','Mangodb','MySQL','PostgreSQL']}
    for key in listDict.keys():
        print "Key ",key
        valueList=listDict[key]
        for val in valueList:
            r.lpush(key,val)

    #r.lpush("tutorial-list","Redis")
    #r.lpush("tutorial-list","Mangodb")
    #r.lpush("tutorial-list","MySQL")

    # Get data from Redis (lrange) 
    arrayList = r.lrange("tutorial-list",0,5)
    print 'Stored srting in redis: ',
    print arrayList
    # Get element from list with index(lindex)
    for i in xrange(4):
        ele = r.lindex("tutorial-list",i)
        print "The element with index number",i,"is",ele
    # Get list length (llen)
    l = r.llen("tutorial-list")
    print "The list tutorial-list length is",l

    # Get head element from tutorial-list(blpop)
    for i in xrange(2):
        ele = r.blpop("tutorial-list",10)
        print "The",(i+1),"element from left ",ele


    # Get tail element from tutorial-list(brpop)
    for i in xrange(2):
        ele = r.brpop("tutorial-list",10)
        print "The",(i+1),"element from right ",ele

    hashVal = r.hgetall('profile')
    print hashVal

    #Empty db
    r.flushdb()

def usage():
    print( "Usage:\n    %s " % (sys.argv[0]))

if __name__ == '__main__':
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hn:D:d:', ['help'])
        a = 0
        for option, value in opts:
            if  option in ["-h","--help"]:
                usage()
                sys.exit(0)
            elif option in ['-n']:
                a = 1
            elif option in ['-D']:
                a = 2
            elif option in ['-d']:
                a = 3
        test_redis(a)
    except:
        usage()
        sys.exit(0)
