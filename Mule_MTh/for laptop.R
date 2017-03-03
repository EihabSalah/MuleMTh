setwd("/Users/eihabsalah/research/dm/CPP/Algorithms/Mule_MTh/Mule_MTh")
threads = c(1,4,8)
minsup = c(20)
rows = length(minsup)
cols = length(threads) +1
X = matrix(0, rows, cols)
colnames(X) <-c("FileName", "1 Thread", "2 Threads", "4 Threads", "8 Threads")

row_val = 1
for (v in minsup){
  outfile = paste("results/res_",v,".txt", sep = "")
  col_val = 1
  X[row_val, col_val] = outfile
  
  for(t in threads){
    command = paste("./mule_MTh ../../data/random graphs/summaryGraphThr10.txt ../../data/summaryAttributeThr10.txt ", outfile, " ", v, " ", t, " ", 2, " >V",v,"_",t, sep = "")
    cat(command)
    cat("\n")
    system(command)
    file = readLines(paste("V",v,"_",t, sep = ""))
    col_val = col_val +1
    X[row_val, col_val] = strsplit(grep("Algorithm's time:", file, value = TRUE), ": ")[[1]][2]
    
  }
  row_val = row_val+ 1
}

write.csv(X, "SG10_Final_result_2.csv", quote = FALSE)
