function [mag tp_out] = cordic(x,ITERATIONS,TP_ITER)
% Function to model the CORDIC abs() function implemented in RTL. This
% function is highly specific to the current RTL implementation, and it
% should be updated whenever modifications are made to the RTL.
% Input parameters:
%   x - array of complex samples. Assumed all integer.
%   ITERATIONS - number of CORDIC iterations to perform. This is
%   paramterized the same was as the RTL where actual number of CORDIC
%   iterations is one more that specified in the ITERATIONS generic.
%   TP_ITER - used for debugging. This can be used to extact intermediate
%   data form a specific CORDIC iteration.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
if ~exist('ITERATIONS','var')
    ITERATIONS = 8; % default to match RTL 
end

if ~exist('TP_ITER','var')
    TP_ITER = ITERATIONS; 
end

% hard-coded CORDIC gain constant from RTL
K = bin2dec('010011011011101010');

% force input to be in first quadrant (RTL uses 2's complement to find
% arithmetic complement)
abs_I = abs(real(x));
abs_Q = abs(imag(x));

% first CORDIC rotation
% arrange in 1st octant (flip IQ if Q > I) and do 1st step, rotation by -45 degress
X_step = (abs_I + abs_Q)*2^ITERATIONS;
Y_step = (abs_Q - abs_I)*2^ITERATIONS;

X_step_tp = X_step; % test point for 1 iteration only; other TPs are set inside the CORDIC iteration loop

% match RTL which does ITERATIONS+1 total 
for i = 1:ITERATIONS 
    % discard LSBs outside of RTL dynamic range
    next_X_step = X_step - floor(Y_step/2^i);
    next_Y_step = Y_step + floor(X_step/2^i);
    
    ix = (Y_step >= 0); % rotate by a negative angle
    % discard LSBs outside of RTL dynamic range
    next_X_step(ix) = X_step(ix) + floor(Y_step(ix)/2^i);
    next_Y_step(ix) = Y_step(ix) - floor(X_step(ix)/2^i);
    
    X_step = next_X_step;
    Y_step = next_Y_step;
    
    % for debugging
    if i == TP_ITER-1
        X_step_tp = X_step;
    end
end

% this models the loss of the LSB in the RTL when fitting output of last
% CORDIC step into the 25-bit multiplier signal to match width in DSP48
X_step_bin = dec2bin(X_step);
X_step_bin(:,end) = '0'; 
X_step_dec = bin2dec(X_step_bin);

% apply CORDIC gain and shift results down to match bits kept in the RTL
% implementation
mag = floor(X_step_dec*K/2^25);

% test point data
tp_out = X_step_tp;

