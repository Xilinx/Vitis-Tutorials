%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Baseline Image
% ------------------------------------------------------------

RGB = imread('../images/amd-instinct.png');
I   = im2gray(RGB);
BW  = edge(I,'canny');

fprintf(1,'Image size: %g x %g\n',size(BW,1),size(BW,2));

% Baseline Matlab model:
res = 128;
theta = linspace(-90,90,1+res); theta = theta(1:end-1);
[Hgld,Tgld,Rgld] = hough(BW,'Theta',theta);

% My model:
[Hact,Tact,Ract] = hough_model(BW,res);


figure;
set(gcf,'Position',[50,50,1200,400]);
subplot(1,3,1);
imshow(RGB);
title(sprintf('(R,C)=(%d,%d)',size(BW,1),size(BW,2)));
xlabel(sprintf('max(\\rho)=%d',max(Rgld)));

subplot(1,3,2);
imshow(imadjust(rescale(Hgld)),'XData',Tgld,'YData',Rgld,'InitialMagnification','fit');
title('Matlab hough()');
xlabel('\theta')
ylabel('\rho');
axis on, axis square;
colormap(gca,jet);

subplot(1,3,3);
imshow(imadjust(rescale(Hact)),'XData',Tact,'YData',Ract,'InitialMagnification','fit');
title('AMD 16-bit Model');
xlabel('\theta')
ylabel('\rho');
axis on, axis square;
colormap(gca,jet);
axis square;
print(gcf,'../images/hough_model','-dpng');
