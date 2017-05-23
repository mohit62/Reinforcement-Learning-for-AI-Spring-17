# Author: Matthew Schlegel
# Purpose: Just a simple plotting util to read in data from StateValues.dat

dat = read.table("/Users/mohitsaraf/StateValues.dat")


plot(x = 1:length(dat[,1]), y = dat[,1], type="l", ylab="Value Estimates", xlab="Capital state", col="purple")


