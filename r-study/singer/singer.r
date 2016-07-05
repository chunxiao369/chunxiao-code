library(igraph)
#读取数据，注意编码格式是utf-8
singer<-read.csv('/mnt/btrfs/chunxiao-code/r-study/singer/singers-sub.csv', head=T, fileEncoding='UTF-8', stringsAsFactors=F)
#加载数据框
g<-graph.data.frame(singer)
#生成图片，大小是800*800px
jpeg(filename='singers.jpg', width=800, height=800, units='px')

plot(g,
    vertex.size=5,     #节点大小
    layout=layout.fruchterman.reingold,  #布局方式
    vertex.shape='none',    #不带边框
    vertex.label.cex=1.5,    #节点字体大小
    vertex.label.color='red',  #节点字体颜色
    edge.arrow.size=0.7)    #连线的箭头的大小
#关闭图形设备，将缓冲区中的数据写入文件
dev.off()
