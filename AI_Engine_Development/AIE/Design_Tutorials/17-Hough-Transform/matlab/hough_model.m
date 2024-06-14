%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [H,theta,rho,rho_calc] = hough_model( BW, theta_res )
   if     (nargin == 1) theta_res = 180; 
   elseif (nargin ~= 2) error('hough_model(BW,theta_res)'); end
   [R,C] = size(BW);
   rho_max = ceil(sqrt((R-1)^2 + (C-1)^2));
   fprintf(1,'rho_max: %g\n',rho_max);
   % Initialize:
   tmp = linspace(-90,90,1+theta_res);
   theta = tmp(1:end-1);
   cos_theta = double(fi(cos(theta*pi/180),1,16,15,'RoundingMethod','Round','OverflowAction','Saturate'));
   sin_theta = double(fi(sin(theta*pi/180),1,16,15,'RoundingMethod','Round','OverflowAction','Saturate'));
   rho = [-rho_max:1:rho_max];
   Nt = numel(theta);
   Nr = numel(rho);
   H = zeros(Nr,Nt); 
   rho_calc = zeros(R,C,Nt);
   % Compute transform:
   for yy = 0 : R-1
     for xx = 0 : C-1
       % Compute the 'rho' value:
       rho_i = double(fi(xx*cos_theta + yy*sin_theta,...
                         1,16,0,'RoundingMethod','Round','OverflowAction','Saturate'));
       rho_calc(1+yy,1+xx,:) = rho_i;
       % Add offset to turn onto 'rho' address:
       addr = rho_i + rho_max;
       for ii = 1 : Nt
         H(1+addr(ii),ii) = H(1+addr(ii),ii) + BW(1+yy,1+xx);
     end % yy
   end %xx
end
