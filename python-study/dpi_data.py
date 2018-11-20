#!/usr/bin/python
#import
#import 

import matplotlib
import datetime
import time
import matplotlib.pyplot as plt
#matplotlib.use('qt4agg')
#from matplotlib.font_manager import *
#myfont = FontProperties(fname='/usr/share/fonts/truetype/arphic/ukai.ttc')
#matplotlib.rcParams['axes.unicode_minus']=False
dpi_file = "/tmp/beijing_speed.txt"
#txt is http://10.0.1.11/redmine/attachments/download/578/beijing_speed.txt

tx_record = []
time_record = []
def process_data(file_name):
    line_count = 0
    tx_found = 0
    time_found = 0
    xdr_tx_found = 0
    current_file = open(file_name, 'r')
    for eachline in current_file:
        #print eachline
        line_count += 1
        if (eachline.find("Tx Bytes") != -1):
            if (tx_found % 3 == 0):
                #print eachline
                strings = eachline.split();
                xdr_tx = strings[4]
                tx_record.append(int(xdr_tx))
                xdr_tx_found += 1
            tx_found += 1
        if (eachline.find("CST 2018") != -1):
            new_time = datetime.datetime.strptime(eachline, '%a %b %d %H:%M:%S CST %Y\n')
            time_record.append(new_time.strftime("%H"))
            time_found += 1
    current_file.close()
    print "line num: %d, %d, %d" %(line_count, tx_found, xdr_tx_found)
    print tx_record

def show_data():
    plt.plot(tx_record)
    plt.show()

if __name__=='__main__':
    process_data(dpi_file)
    show_data()
