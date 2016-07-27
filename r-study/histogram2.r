
mydata <- read.table('13764769198.year', header=T)
freq <- mydata[,2]
days <- mydata[,1]
png("year.png")
barplot(freq,names.arg=days,col=c("purple","green3","blue","red"),density = c(7.5,12.5,17.5,22.5),)
title(xlab="月份")#横轴名字
title(ylab="通话次数")#纵轴名字

