
mydata <- read.table('13764769198.week', header=T)
freq <- mydata[,2]
days <- mydata[,1]
png("13764769198.week.png")
barplot(freq,names.arg=days,col=c("purple","green3","blue","red"),density = c(7.5,12.5,17.5,22.5),)
#title(xlab="星期")#横轴名字
title(ylab="通话规律")#纵轴名字

