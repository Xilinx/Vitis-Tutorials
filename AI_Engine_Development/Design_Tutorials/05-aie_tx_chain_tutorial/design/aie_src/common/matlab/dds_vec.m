function [dds_out]=dds_vec(fcin,fs,signal_length)
%   simple simulation using a scalar DDS + vector interpolation

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
global DATA ;
global DFE_SPEC ;
if fcin > 0
    fc = -fcin;
else
    fc = fcin;
end
v = 8;
n  = signal_length;
index_start = mod(round(2^32*(fc)*v/fs),2^32);
points = 0:n-1;
index = points * index_start;

% create sine cosine lookup table from ME reference data file.

    sc_tab = complex(double(DATA.aie_cosine_20b_arch17_1_1(1:1:end)),double(DATA.aie_sine_20b_arch17_1_1(1:1:end)))/2^15;
    index = index/2^12;
    %wrap around to fetch from 20-bit addresable cos/sine lookup table
    index = int32(mod(fix(index), 2^20));

dds_in = me_quant(sc_tab(index+1));

ww = round(exp(2j*pi*fc*(0:v-1)/fs)*2^15);
ww(ww ==32768) = ww(ww ==32768) -1 ;
ww = ww/2^15 ;

if fcin > 0
    ww = conj(ww);
    dds_in = conj(dds_in);
end
dds_out = kron(dds_in, ww);
return


function y = me_quant(x)

xr = real(x);  xi=imag(x);
yr = min(max(round(xr*2^15),-2^15),2^15);
yi = min(max(round(xi*2^15),-2^15),2^15);
y = complex(yr,yi)/2^15;
return

