
args <- commandArgs(TRUE)
cat (args[1], "\n")
mydata <- read.table(args[1], header=T)
freq=t(mydata[2])
maxy=max(freq)
png("week.png")
plot(mydata,xlab="星期",ylab="通话次数",ylim=c(0,maxy),col="red",main="每周通话规律图",type="o")
#title(xlab="星期")#横轴名字
#title(ylab="通话规律")#纵轴名字

