#coding:utf-8
import matplotlib
matplotlib.use('qt4agg')
from matplotlib.font_manager import *
import matplotlib.pyplot as plt
myfont = FontProperties(fname='/usr/share/fonts/truetype/arphic/ukai.ttc')
matplotlib.rcParams['axes.unicode_minus']=False 
plt.plot((1,2,3),(4,3,-1))
plt.xlabel(u'横坐标')
plt.ylabel(u'纵坐标')
plt.show()



