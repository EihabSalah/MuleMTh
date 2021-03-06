# setwd("~/research/dm/CPP/Algorithms/Mule_MTh/")
setwd("/Users/eihabsalah/research/dm/CPP/Algorithms/Mule_MTh/Mule_MTh")
system("make clean; make")
threads = c(1,4,8)
minsup = c(13:13)
rows = length(minsup)
cols = length(threads) +1
X = matrix(0, rows, cols)
colnames(X) <-c("FileName", "1 Thread", "4 Threads", "8 Threads")

row_val = 1
for (v in minsup){
  outfile = paste("hs_res_",v,".txt", sep = "")
  col_val = 1
  X[row_val, col_val] = outfile
  
  for(t in threads){
    command = paste("./mule_MTh ../../data/hs_datasetSup10SummaryGraphS1.txt ../../data/hs_datasetSup10AttributeMatrix.txt ", outfile, " ", v, " ", t, " ", 2, " >hs_V",v,"_",t, sep = "")
    cat(command)
    cat("\n")
    system(command)
    file = readLines(paste("hs_V",v,"_",t, sep = ""))
    col_val = col_val +1
    X[row_val, col_val] = strsplit(grep("Algorithm's time:", file, value = TRUE), ": ")[[1]][2]
    
  }
  row_val = row_val+ 1
}

write.csv(X, "hs10_Final_result_2.csv", quote = FALSE)
