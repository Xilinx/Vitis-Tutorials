function [dpd_out] = dpd_1gsps(dpd_sig_in, dpd_mag_in, params,me_params)
% Top level function to model 1 Gsps DPD example
% signal     : DPD input signal
% mag        : Magnitude of samples corresponding to DPD input
% params     : Parameters that hold DPD specific LUTs, magnitude, interpolation etc
% me_params  : Parameters to hold DPD terms

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
Nmod8 = floor(length(dpd_sig_in)/8)*8; % make sure number of samples processed is multiple of 8
dpd_sig_in = dpd_sig_in(1:Nmod8);
dpd_mag_in = dpd_mag_in(1:Nmod8);

CascadeOutData1 = zeros(length(me_params)-1,floor(length(dpd_sig_in)));
CascadeInData1 = zeros(length(me_params)-1,floor(length(dpd_sig_in)));

% mechanization to feed signal/magnitude samples through cores
[ CascadeOutData1(1,:)] = aie_dpd_kernel( dpd_sig_in, dpd_mag_in, params, me_params{1}, CascadeInData1(1,:) );
CascadeInData1(2,:) = CascadeOutData1(1,:);
[ CascadeOutData1(2,:)] = aie_dpd_kernel( dpd_sig_in, dpd_mag_in, params, me_params{2}, CascadeInData1(2,:) );
CascadeInData1(3,:) = CascadeOutData1(2,:);
[ CascadeOutData1(3,:)] = aie_dpd_kernel( dpd_sig_in, dpd_mag_in, params, me_params{3}, CascadeInData1(3,:) ); 
CascadeInData1(4,:) = CascadeOutData1(3,:);
[ CascadeOutData1(4,:)] = aie_dpd_kernel( dpd_sig_in, dpd_mag_in, params, me_params{4}, CascadeInData1(4,:) );
dpd_out = aie_srs(CascadeOutData1(end,:),me_params{end}.me_output_shift); % saturate and round output from last core
end



