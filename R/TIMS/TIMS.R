library(MASS)
library(e1071)
data <- read.table("data.txt", fileEncoding="latin1")
dataVect <- unlist(data, use.names = FALSE)
freq <- table(dataVect)
freqPlot <- plot(freq)
barPlot <- barplot(freq)         # apply the barplot function

standart <- sd(dataVect)
variance <- standart * standart
mean <- mean(dataVect)
variation <- standart / mean
deviation <- variance * (length(dataVect) - 1)
quantile(dataVect)
median <- median(dataVect)
decLocations <- quantile(dataVect, probs = seq(0.1,0.9,by=0.1))
scope <- max(dataVect) - min(dataVect)
ac <- moment(dataVect, order=3, center=TRUE) / (moment(dataVect, order=2, center=TRUE) ^ (2/3))
ex <- moment(dataVect, order=4, center=TRUE) / (moment(dataVect, order=2, center=TRUE) ^ 2) - 3
ecdf = ecdf(dataVect)
funcPlot <- plot(ecdf)