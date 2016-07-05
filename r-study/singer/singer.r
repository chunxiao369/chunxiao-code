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

#关系图中某人的关系图
g<-graph.data.frame(singer)
gn<-graph.neighborhood(g, order=1)
#周杰伦的关系图，所有与周杰伦有关系的人，以及这些人之间的关系
plot(gn[[1]],layout=layout.fruchterman.reingold)
#刘若英的关系图，所有与刘若英有关系的人，以及这些人之间的关系
plot(gn[[2]],layout=layout.fruchterman.reingold)
#凤凰传奇的关系图，所有与凤凰传奇有关系的人，以及这些人之间的关系
plot(gn[[3]],layout=layout.fruchterman.reingold)

#某个人的两层关系，三层以上的不会显示
g<-graph.data.frame(singer)
gn<-graph.neighborhood(g, order=2)
plot(gn[[2]], layout=layout.fruchterman.reingold)

#某两个人的关联图
g<-graph.data.frame(singer)
gn<-graph.neighborhood(g, order=1)
plot(gn[[1]]+gn[[2]], layout=layout.fruchterman.reingold)

#根据联系人的多少决定节点的大小和色彩，连线设成弧线
source("http://michael.hahsler.net/SMU/ScientificCompR/code/map.R")
#将连线设成弧线，数值越大弧线越弯
E(g)$curved <- 0.2
layout=layout.fruchterman.reingold
plot(g, layout=layout, vertex.size=map(degree(g),c(1,20)), vertex.color=map(degree(g),c(1,20)))

dev.off()
