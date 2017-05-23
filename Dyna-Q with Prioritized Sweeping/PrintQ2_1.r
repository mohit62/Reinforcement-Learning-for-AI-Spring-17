# Author: Mohit Saraf


dat = read.table("/Users/mohitsaraf/Applications/Assignment5/MountainCar.dat")
plot(dat$V1, type="l", ylab="No Of Steps per Episode", xlab="No of Episodes", col="green")
mean1=mean(dat$V1)
SE=sd(dat$V1)/sqrt(50)
