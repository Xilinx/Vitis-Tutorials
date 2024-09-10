clear all; close all; clc;
%%
dat = load('vector.dat');
diff = abs(dat(:,3) - dat(:,4));
diff2= abs(dat(:,4) - dat(:,5));
figure; plot(diff);  title 'error CORDIC vs. ATAN2'; grid;
figure; plot(diff2); title 'error2'; grid;
max_diff = max(diff)
sum_abs_err = sum(diff)
max_diff2 = max(diff2)
sum_abs_err2 = sum(diff2)
