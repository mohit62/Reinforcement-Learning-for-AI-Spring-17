# Author: Mohit Saraf


x = read.table("/Users/mohitsaraf/Applications/Q2_B/alpha.dat")
y = read.table("/Users/mohitsaraf/Applications/Q2_B/DYNAQ.dat")


plot(x$V1,y$V1,type = "l", ylab="Average No Of Steps per Run", xlab="Alpha Values", col="blue")

