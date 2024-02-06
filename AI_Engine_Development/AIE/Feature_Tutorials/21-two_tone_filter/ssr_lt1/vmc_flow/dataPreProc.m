function processedData = dataPreProc(inpdataFile)
%a= load('data/data/sig_i.txt');
readInSamples= load(inpdataFile);
[k,l]=size(readInSamples);
res_tmp = reshape(readInSamples', 2, k*l/2);
processedData = res_tmp';