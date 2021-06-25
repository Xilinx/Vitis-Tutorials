function [L,D] = aie_chol_bf(A)
% function [L,D] = me_chol(A)
% Cholesky Decomposition Model of AI Engine/ME Cholesky kernel
% A   : Input Hermitian matrix, 16-bit precision
% L   : Output lower triangular matrix (A=L*L')
% D   : Inverse of diagonal values of L, these have 32 bit precision
%       Scale factor for inverse diagonal values is hardcoded to 31
% This is related to the inv intrinsic in MI Engine as follows:
% d = inv(123456,0,31) ; % AI Engine inv 
% d = round(1/sqrt(123456)*2^31); % Matlab equivalent 
% 
% d = inv(123456,4,31) ; % AI Engine inv 
% d = round(1/sqrt(123456/2^4)*2^31); % Matlab equivalent 

%  (c) Copyright 2021 Xilinx, Inc. All rights reserved.
%
%  This file contains confidential and proprietary information
%  of Xilinx, Inc. and is protected under U.S. and
%  international copyright and other intellectual property
%  laws.
%
%  DISCLAIMER
%  This disclaimer is not a license and does not grant any
%  rights to the materials distributed herewith. Except as
%  otherwise provided in a valid license issued to you by
%  Xilinx, and to the maximum extent permitted by applicable
%  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
%  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
%  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
%  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
%  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
%  (2) Xilinx shall not be liable (whether in contract or tort,
%  including negligence, or under any other theory of
%  liability) for any loss or damage of any kind or nature
%  related to, arising under or in connection with these
%  materials, including for any direct, or any indirect,
%  special, incidental, or consequential loss or damage
%  (including loss of data, profits, goodwill, or any type of
%  loss or damage suffered as a result of any action brought
%  by a third party) even if such damage or loss was
%  reasonably foreseeable or Xilinx had been advised of the
%  possibility of the same.
%
%  CRITICAL APPLICATIONS
%  Xilinx products are not designed or intended to be fail-
%  safe, or for use in any application requiring fail-safe
%  performance, such as life-support or safety devices or
%  systems, Class III medical devices, nuclear facilities,
%  applications related to the deployment of airbags, or any
%  other applications that could lead to death, personal
%  injury, or severe property or environmental damage
%  (individually and collectively, "Critical
%  Applications"). Customer assumes the sole risk and
%  liability of any use of Xilinx products in Critical
%  Applications, subject only to applicable laws and
%  regulations governing limitations on product liability.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

n = length(A);
L = zeros(n,n);
D = zeros(1,n);
shift=16;
% shift_s=15;
% shift_invsqrt=31;
shift_s=14;
shift_invsqrt=30;

for j=1:n % col-sweep
   d=A(j,j)*2^shift;
   if(j>1)
      d=d-L(j,1:j-1)*L(j,1:j-1)';
   end %if
   invd=round(1/sqrt(d)*2^shift_invsqrt);
   D(j)=invd;     %%Updation here...%%
   L(j,j)=floor(d/2^shift)*invd/2^shift_s;
   L(j,j)=aie_sat(L(j,j));
%    L(j,j)=floor(L(j,j));
   L(j+1:n,j)=A(j+1:n,j);
   if(j>1) %L(i,j)
      L(j+1:n,j)=L(j+1:n,j)-L(j+1:n,1:j-1)*L(j,1:j-1)'/2^shift; 
      L(j+1:n,j)=aie_sat(floor(L(j+1:n,j)));
%       L(j+1:n,j)=floor(L(j+1:n,j));
   end %if    
%    L(j+1:n,j)=floor(L(j+1:n,j)*invd/2^shift_s);    
   L(j+1:n,j)=aie_sat(floor(L(j+1:n,j)*invd/2^shift_s));    
end % col-sweep   
end


