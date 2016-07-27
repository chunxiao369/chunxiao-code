args <- commandArgs(TRUE)
cat (args[1], "\n")
mydata <- read.table(args[1], sep = ",", header=F)
freq=t(mydata[2])
maxy=max(freq)
png("day.png")
plot(mydata,xlab="小时",ylab="通话次数",ylim=c(0,maxy),col="red",main="每天通话规律图",type="o")

