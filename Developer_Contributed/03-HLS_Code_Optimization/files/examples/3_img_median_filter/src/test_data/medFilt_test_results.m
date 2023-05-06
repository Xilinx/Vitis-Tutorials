% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%%
close all;
clear all;
clc

%% input test image

I = imread('../../../5_img_histEq/common/test_data/test_1080p.bmp', 'BMP'); % original RGB image
figure; imshow(I, []); title 'Input RGB Image'
G = rgb2gray(I); % black and white image
figure; imshow(G, []); title 'Input GRAY Image'

%% add noise

J = imnoise(G,'salt & pepper',0.08); % add noise
figure; imshow(J, []); title 'Noisy Input Image'

%% median filter the noisy image

M3 = medfilt2(J, [3,3]);
M5 = medfilt2(J, [5,5]);
M7 = medfilt2(J, [7,7]);
figure; imshow(M5, []); title 'M5 Filtered Image'
figure; imshow(M3, []); title 'M3 Filtered Image'
figure; imshow(M7, []); title 'M7 Filtered Image'

%% write images

imwrite(G, 'gray_test.bmp');
imwrite(J, 'noisy_rgb2y_1080p.bmp');
imwrite(M3, 'm_golden_median3x3_1080p.bmp');
imwrite(M5, 'm_golden_median5x5_1080p.bmp');
imwrite(M7, 'm_golden_median7x7_1080p.bmp');


%
% %% MEDIAN 3x3
% II3 = imread('c_out_median3x3_1080p.bmp', 'BMP');
% dII3 = double(II3);
%
% RR3 = imread('m_golden_median3x3_1080p.bmp', 'BMP');
% dRR3 = double(RR3);
%
% figure; imshow(II7); title 'C        output MEDIAN 3x3'
% figure; imshow(RR7); title 'M GOLDEN output MEDIAN 3x3'
%
% diff_m7 = abs(dRR7 - dII7);
% index_m7 = diff_m7 >0;
% sum(index_m7(:))
% diff_m7(1:6, :,:) = 0; diff_m7(:, 1:6,:) = 0; diff_m7(end-6:end, :,:) = 0; diff_m7(:, end-6:end, :) = 0;
% index_m7 = diff_m7 >0;
% sum(index_m7(:))
% figure; imshow(diff_m7,[]); title 'DIFF MEDIAN 7x7 C vs. MATLAB'
% sum_abs_diff_m7 = sum(diff_m7(:))
%
%
% %% MEDIAN 7x7
% II7 = imread('c_out_median7x7_1080p.bmp', 'BMP');
% dII7 = double(II7);
%
% RR7 = imread('m_golden_median7x7_1080p.bmp', 'BMP');
% dRR7 = double(RR7);
%
% figure; imshow(II7); title 'C        output MEDIAN 7x7'
% figure; imshow(RR7); title 'M GOLDEN output MEDIAN 7x7'
%
% diff_m7 = abs(dRR7 - dII7);
% index_m7 = diff_m7 >0;
% sum(index_m7(:))
% diff_m7(1:6, :,:) = 0; diff_m7(:, 1:6,:) = 0; diff_m7(end-6:end, :,:) = 0; diff_m7(:, end-6:end, :) = 0;
% index_m7 = diff_m7 >0;
% sum(index_m7(:))
% figure; imshow(diff_m7,[]); title 'DIFF MEDIAN 7x7 C vs. MATLAB'
% sum_abs_diff_m7 = sum(diff_m7(:))
%
% %% MEDIAN 5x5
% II5 = imread('c_out_median5x5_1080p.bmp', 'BMP');
% dII5 = double(II5);
%
% RR5 = imread('m_golden_median5x5_1080p.bmp', 'BMP');
% dRR5 = double(RR5);
%
% figure; imshow(II5); title 'C        output MEDIAN 5x5'
% figure; imshow(RR5); title 'M GOLDEN output MEDIAN 5x5'
%
% diff_m5 = abs(dRR5 - dII5);
% index_m5 = diff_m5 >0;
% sum(index_m5(:))
% diff_m5(1:4, :,:) = 0; diff_m5(:, 1:4,:) = 0; diff_m5(end-4:end, :,:) = 0; diff_m5(:, end-4:end, :) = 0;
% index_m5 = diff_m5 >0;
% sum(index_m5(:))
% figure; imshow(diff_m5,[]); title 'DIFF MEDIAN 5x5 C vs. MATLAB'
% sum_abs_diff_m5 = sum(diff_m5(:))
%
% %% MEDIAN 3x3
% II3 = imread('c_out_median3x3_1080p.bmp', 'BMP');
% dII3 = double(II3);
%
% RR3 = imread('m_golden_median3x3_1080p.bmp', 'BMP');
% dRR3 = double(RR3);
%
% figure; imshow(II3); title 'C        output MEDIAN 3x3'
% figure; imshow(RR3); title 'M GOLDEN output MEDIAN 3x3'
%
% diff_m3 = abs(dRR3 - dII3);
% index_m3 = diff_m3 >0;
% sum(index_m3(:))
% diff_m3(1:2, :,:) = 0; diff_m3(:, 1:2,:) = 0; diff_m3(end-2:end, :,:) = 0; diff_m3(:, end-2:end, :) = 0;
% index_m3 = diff_m3 >0;
% sum(index_m3(:))
% figure; imshow(diff_m3,[]); title 'DIFF MEDIAN 3x3 C vs. MATLAB'
% sum_abs_diff_m3 = sum(diff_m3(:))
% %%
