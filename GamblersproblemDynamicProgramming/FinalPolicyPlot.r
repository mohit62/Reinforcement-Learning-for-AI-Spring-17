# Author: Matthew Schlegel
# Purpose: Just a simple plotting util to read in data from StateValues.dat


dat = read.table("/Users/mohitsaraf/PolicyValue.dat")



plot(x = 1:length(dat[,1]), y = dat[,1], type="l", ylab="Final Policy", xlab="Capital state", col="green")

