% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%%
#https://www.nxp.com/docs/en/application-note/AN4318.pdf
close all;
clear all;
clc

%%
images_histogram = zeros(1, 256);
cdf = zeros(1, 256);
h   = zeros(1, 256);
my_eq_Y = zeros(1080, 1920);

%%
RGB = imread('test_1080p.bmp', 'BMP');
f1=figure; imshow(RGB, []); title(f1.CurrentAxes, 'RGB input')
YCBCR = rgb2ycbcr(RGB);
%f2=figure; imshow(YCBCR, []); title(f2.CurrentAxes, 'YCBCR input')

Y = YCBCR(:,:, 1);
f3=figure; imshow(Y, []); title(f3.CurrentAxes, 'input Y ch')
eq_Y = histeq(Y, 256);
f4=figure; imshow(eq_Y, []); title(f4.CurrentAxes, 'MATLAB equalized Y ch')

eq_YCBCR = YCBCR;
eq_YCBCR(:,:,1)=eq_Y;
%f5=figure; imshow(eq_YCBCR, []); title(f5.CurrentAxes, 'MATLAB equalized YCBCR')

eq_I = ycbcr2rgb(eq_YCBCR);
f6=figure; imshow(eq_I, []); title(f6.CurrentAxes, 'MATLAB equalized YCBCR2RGB')



%%
% Getting histogram
for grey_level = 0:1:255
    images_histogram(grey_level + 1) = 0;
    for i = 1:1:1920
        for j = 1:1:1080
            if Y(j,i) == grey_level
                images_histogram(grey_level + 1) = images_histogram(grey_level + 1) + 1;
            end
        end
    end
end

%Cumulative distribution function (cdf)
for grey_level = 1:1:256
    cdf(grey_level) = 0;
    for i = 1:1:grey_level
        cdf(grey_level) = cdf(grey_level) + images_histogram(i);
    end
end

gain = 255 /(1920*1080-1)
for i = 1:1:256
    h(i) = (cdf(i)-1) * gain;
end

for i = 1:1:1920
    for j = 1:1:1080
        y = Y(j,i);
        my_eq_Y(j,i) = uint8(h(y+1));
    end
end
f7=figure; imshow(my_eq_Y, []);   title(f7.CurrentAxes, 'my Equalized Y   Image')
eq_YCBCR(:,:,1) = my_eq_Y;
eq_I = ycbcr2rgb(eq_YCBCR);
f8=figure; imshow(eq_I, []); title(f8.CurrentAxes, 'my equalized YUV2RGB')

save -ascii -double ref_hist.txt images_histogram
save -ascii -double ref_cdf.txt cdf

diff = abs(double(eq_Y) - my_eq_Y);
f9=figure; imshow(diff, []); title(f9.CurrentAxes, 'DIFF equalized Y')

max_min = [max(diff(:)) min(diff(:))]

%%
if 0
C = imread('c_out_1080p.bmp', 'BMP');
figure; imshow(C(:,:, 2), []); title 'HLS output image'
dC = double(C);

R = imread('golden_out_1080p.bmp', 'BMP');
dR = double(R);
figure; imshow(R(:,:,2), []); title 'HLS Golden Image'

diff_y = abs(dR - dC);
diff_y(1:3, :,:) = 0; diff_y(:, 1:3,:) = 0; diff_y(end-3:end, :,:) = 0; diff_y(:, end-3:end, :) = 0;
index_y = diff_y >0;
sum_index_rgb2y_diff = sum(index_y(:))
figure; imshow(diff_y); title 'DIFF'
end
