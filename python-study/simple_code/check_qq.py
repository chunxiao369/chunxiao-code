#!/usr/bin/python
import time,datetime
import urllib2
from lxml import etree

qq = 187264059

def chk_qq(qqnum):
    chkurl = 'http://www.webxml.com.cn/webservices/qqOnlineWebService.asmx/qqCheckOnline?qqCode=+'+str(qqnum)
    print chkurl
    a = urllib2.urlopen(chkurl)
    page = a.read()
    length=a.headers.get("content-length")
    a.close()
    print "length is " + length 
    print datetime.datetime.now()
    print page
    root = etree.fromstring(page)
    result = root.xpath('text()')
    print result
    if str(result) == "['Y']":
        return 'Online'
    elif str(result) == "['N']":
        return 'Offline'
    elif str(result) == "['E']":
        return 'Unknown!'
def writestate(statenow):
    f=open(str(qq),'a')
    m=str(datetime.datetime.now())+"===state===="+statenow+"\n\r"
    f.write(m)
    f.close()
##state=1
if __name__=='__main__':
    stat = chk_qq(qq)
    writestate(stat)
    print `qq` + ' is ' + stat
