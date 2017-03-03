setwd("/Users/eihabsalah/research/dm/CPP/Algorithms/Mule_MTh/Mule_MTh/")

f = readLines("time_results")
gr = grep("real\t", f, value = TRUE)
d1 = strsplit(gr, "\t")[[1]][2]
mins = strsplit(d1, "m")[[1]][1]
d2 = strsplit(d1, "m")[[1]][2]
secs = strsplit(d2, "s")[[1]][1]

total_time = as.numeric(mins) * 60 + as.numeric(secs)
total_time




fss <- paste0("GF_result.csv")
if (file.exists(fss))
  df <- read.csv2(fss, TRUE, stringsAsFactors=FALSE)

dfRead<-read.csv("GF_result.csv") # read the file
all<-rbind(dfRead, df2) # rbind both data.frames
# get only the non duplicate rows from the new data.frame
nonDuplicate <- all[!duplicated(all)&c(rep(FALSE, dim(dfRead)[1]), rep(TRUE, dim(df2)[1])), ]
# append the file with the non duplicate rows
write.table(nonDuplicate,"export.csv", row.names=F,na="NA",append=T, quote= FALSE, sep=",", col.names=F)


sink("all.txt", append = FALSE)
f
cat(file = "tt.txt", system("time ./mule_Mth ../../data/hs_datasetSup10SummaryGraphS1.txt ../../data/hs_datasetSup10AttributeMatrix.txt  res1.txt 18 8 2"))
sink()
out <- capture.output(file = "t.txt")
cat("My title", out, file="t.txt", sep="n", append=TRUE)

sink("sink-examp.txt")
i <- 1:10
outer(i, i, "*")
sink()



system("(time ./mule_Mth ../../data/hs_datasetSup10SummaryGraphS1.txt ../../data/hs_datasetSup10AttributeMatrix.txt  res1.txt 18 8 2) > V1 2> time_results1")

