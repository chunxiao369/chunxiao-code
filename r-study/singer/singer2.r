library(igraph)
#读取数据，注意编码格式是utf-8
singer<-read.csv('/mnt/btrfs/chunxiao-code/r-study/singer/singers-sub2.csv', head=T, fileEncoding='UTF-8', stringsAsFactors=F)
#加载数据框
g<-graph.data.frame(singer)
#生成图片，大小是800*800px
jpeg(filename='singers.jpg', width=800, height=800, units='px')

plot(g,
    vertex.size=16,     #节点大小
    layout=layout.fruchterman.reingold,  #布局方式
    #vertex.shape='none',    #不带边框
    main="五个电话互相都有联系示意图",
    vertex.label.cex=1.6,    #节点字体大小
    vertex.label.color='blue',  #节点字体颜色
    edge.arrow.size=0.7)    #连线的箭头的大小
dev.off()
