%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;


% ------------------------------------------------------------
% Generate SINCOS LUT for AIE design
% ------------------------------------------------------------

NT = 32;
theta_num = 4;
theta_res = 128;
theta = linspace(-90,+90,1+theta_res);
theta = theta(1:end-1);
tmp = exp(1i*pi/180*theta);
sincos = fi(tmp,1,16,15,'RoundingMethod','Round','OverflowAction','Saturate');
sincos = reshape(sincos,theta_num,[]);
assert(size(sincos,2) == NT);
fid = fopen('../aie/hough_tile_sincos_lut.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'//\n');
fprintf(fid,'std::vector<std::vector<hough::TT_THETA> > theta_init = { \n');
for pp = 1 : size(sincos,2)
  fprintf(fid,'{ \n');
  for ii = 1 : size(sincos,1)
    if ( ii == size(sincos,1) )
      fprintf(fid,'{ %d, %d } \n',real(sincos(ii,pp)).int,imag(sincos(ii,pp)).int);
    else
      fprintf(fid,'{ %d, %d },\n',real(sincos(ii,pp)).int,imag(sincos(ii,pp)).int);
    end
  end
  if ( pp == size(sincos,2) ) fprintf(fid,'} \n');
  else                        fprintf(fid,'},\n');
  end
end
fprintf(fid,'};\n');
fclose(fid);

% ------------------------------------------------------------
% Baseline Image & Golden Data
% ------------------------------------------------------------

RGB = imread('../images/amd-instinct.png');
I   = im2gray(RGB);
BW  = edge(I,'canny');

[R,C] = size(BW);
fprintf(1,'Image size: %g x %g\n',R,C);

% Baseline Matlab model:
[Hmat,Tmat,Rmat] = hough(BW,'Theta',theta);

[Hgld,Tgld,Rgld,rho_calc] = hough_model(BW,theta_res);

figure;
subplot(1,2,1);
imshow(imadjust(rescale(Hmat)),'XData',Tmat,'YData',Rmat,'InitialMagnification','fit');
title('Matlab Version');
xlabel('\theta')
ylabel('\rho');
axis on, axis normal;
colormap(gca,jet);

subplot(1,2,2);
imshow(imadjust(rescale(Hgld)),'XData',Tgld,'YData',Rgld,'InitialMagnification','fit');
title('AMD Version');
xlabel('\theta')
ylabel('\rho');
axis on, axis normal;
colormap(gca,jet);
pause(1);

% ------------------------------------------------------------
% Temporary -- Compute 'rho' outputs
% ------------------------------------------------------------

[~,~,~] = mkdir('../aie/data');

for tt = 1 : NT
  fid = fopen(sprintf('../aie/data/rho_o_%d.txt',tt-1),'w');
  data = rho_calc(:,:,(tt-1)*theta_num+[1:theta_num]);
  for yy = 1 : R
    for xx = 1 : C
      fprintf(fid,'%d %d %d %d\n',data(yy,xx,1),data(yy,xx,2),data(yy,xx,3),data(yy,xx,4));
    end
  end
  fclose(fid);
end

% ------------------------------------------------------------
% Image Source
% ------------------------------------------------------------

for tt = 1 : NT
  fid = fopen(sprintf('../aie/data/sig_i_%d.txt',tt-1),'w');
  for yy = 1 : R
    for xx = 1 : 4 : C
      fprintf(fid,'%d %d %d %d\n',BW(yy,xx),BW(yy,xx+1),BW(yy,xx+2),BW(yy,xx+3));
    end
  end
  fclose(fid);
end

% ------------------------------------------------------------
% Golden Outputs
% ------------------------------------------------------------

for tt = 1 : NT
  fid = fopen(sprintf('../aie/data/sig_o_%d.txt',tt-1),'w');
  data = reshape(transpose(Hgld(:,(tt-1)*theta_num+[1:theta_num])),1,[]);
  for rr = 1 : 4 : numel(data)
    fprintf(fid,'%d %d %d %d\n',data(rr),data(rr+1),data(rr+2),data(rr+3));
  end
  fclose(fid);
end
