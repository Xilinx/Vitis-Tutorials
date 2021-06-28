function [ output ] = aie_srs( input, shift )
%ME_SRS Applies shift-round-saturate operation as in ME to input data
%   Applies shift-round-saturate operation as per ME 16-bit finite
%   precision arithmetic. Input is assumed to be integer-valued. The
%   processing consists of three parts
%       - Shifting down of the data by the number of bits specified in
%       shift
%       - Rounding to nearest with 'half' rounded up (towars +infinity). 
%       - Saturation of values exceeding the 16-bit dynamic range
%

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
DATA_WIDTH = 16;
output_re = real(input); output_im = imag(input);
% round to nearest, round half up (towards +infinity)
output_re = round(output_re/2^shift);
output_im = round(output_im/2^shift);
% detect negative values at the exact half point (-0.5)
neg_half_re = find((real(input)/2^shift - fix(real(input)/2^shift)) == -0.5);
neg_half_im = find((imag(input)/2^shift - fix(imag(input)/2^shift)) == -0.5);
% add -0.5 before rounding
output_re(neg_half_re) = round((real(input(neg_half_re))/2^shift) + 0.5);
output_im(neg_half_im) = round((imag(input(neg_half_im))/2^shift) + 0.5);

% saturate
% positive values
re_idx = find(output_re>2^(DATA_WIDTH-1) - 1);
output_re(re_idx) = 2^(DATA_WIDTH-1) - 1;
im_idx = find(output_im>2^(DATA_WIDTH-1) - 1);
output_im(im_idx) = 2^(DATA_WIDTH-1) - 1;
% negative values
re_idx = find(output_re<-2^(DATA_WIDTH-1) - 1);
output_re(re_idx) = -2^(DATA_WIDTH-1);
im_idx = find(output_im<-2^(DATA_WIDTH-1) - 1);
output_im(im_idx) = -2^(DATA_WIDTH-1);

% combine real-imag parts to form output
output = complex(output_re,output_im);
end

