
slices <- c(3,1,4,2)
names <- c("甲","乙","丙","丁")
png("r-graph-sample.png")
barplot(beside=TRUE,
    slices,#纵轴取值
    names.arg=names,#边框名字
    border="black",#边框颜色
    col=c("purple","green3","blue","red"),#框内线条颜色
    density = c(7.5,12.5,17.5,22.5),#框内线条密度
    angle = c(45,60,120,135),#框内线条倾斜角度
    width = c(4,2.2,2.2,3),#边框宽度
    space = c(1.5,0.5,0.5,1),#边框间距
    ylim=c(0,5),)#纵轴取值范围
title(xlab="横")#横轴名字
title(ylab="竖")#纵轴名字
lbls <- round(slices/sum(slices)*100)
lbls <- paste(lbls,"%",sep="")# ad % to labels
lbls <- paste(names, lbls)# add percents to labels
#图例
legend("topright",lbls,
    fill=c("purple","green3","blue","red"),
    density = c(7.5,12.5,17.5,22.5),
    angle = c(45,60,120,135),
)
