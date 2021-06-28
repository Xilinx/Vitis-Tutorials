function [cascade_out, dpd_out] = aie_dpd_noipol_kernel(signal, mag, params, aie_params, cascade_in, core_num)
% dpd_filter_me_kernel
% Top level function to model functionality  of DPD kernels in Math Engine / AI Engine
% signal     : DPD input signal
% mag        : Magnitude of samples corresponding to DPD input
% params     : Parameters that hold DPD specific LUTs, magnitude, interpolation etc
% me_params  : Parameters to hold DPD terms
% cascade_in : Cascade input from adjoining ME core

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
data_v8  = zeros(1,8);
v8cacc   = zeros(1,8);
M=size(params.term_selection,2);
R=size(params.term_selection,1);
mag_ss=zeros(1,6);
signal = circshift(signal,R-M);

if ~isfield(aie_params,'lutsw_loc')
    aie_params.lutsw_loc = length(signal)+1;
end

for i = 1:8:length(signal)
    if (i >= aie_params.lutsw_loc)
        LUTS0 = aie_params.core2(core_num).LUTS0;
        LUTS1 = aie_params.core2(core_num).LUTS1;
        LUTS0=LUTS0.';
        LUTS1=LUTS1.';
    else
        LUTS0 = aie_params.core(core_num).LUTS0;
        LUTS1 = aie_params.core(core_num).LUTS1;
        LUTS0=LUTS0.';
        LUTS1=LUTS1.';
    end    
    for j = 1:8
        if (j == 1 || j == 5)
            data_v8(j:j+3) = signal(i-1+j:i-1+j+3);            
        end
        mag_ss = [mag_ss(2:end) mag(i-1+j)];
        lut_v16_1 = get_luts(mag_ss(5:6),LUTS0,LUTS1,params);  
        lut_v16_2 = get_luts(mag_ss(1:2),LUTS0,LUTS1,params);
        scd(:) = cascade_in(i-1+j,:).';
        if (aie_params.lane_config == 2)
            v8cacc = mac4_preadd_rot(v8cacc,scd,1,lut_v16_1,0,[0 2 4 6],1,8,1,data_v8,j-1,[0 0 0 0],1);
        else
            lut_v32 = [lut_v16_1(1:8) lut_v16_2(1:8) lut_v16_1(9:16) lut_v16_2(9:16)];
            v8cacc = mac4_preadd_rot(v8cacc,scd,1,lut_v32,0,[0 1 2 3],12,16,1,data_v8,j-1,[0 0 0 0],4);
        end
        mcd(i-1+j,:) = v8cacc(5:8).';        
        if (j == 4 || j == 8)
            dpd_out(i-1+j-3:i-1+j) = v8cacc(1:4);
        end        
    end    
end    
cascade_out = mcd;
end

function lut_v16 = get_luts(mag,LUTS0,LUTS1,params)
    mag_width = params.mag_width;
    lut_addr_width = params.lut_addr_width;
    indx = floor(mag/2^(mag_width-lut_addr_width));      
    l1 = LUTS1(indx(2)+1,:);
    l0 = LUTS0(indx(1)+1,:);
    lut_v16 = [l0 l1];
end
