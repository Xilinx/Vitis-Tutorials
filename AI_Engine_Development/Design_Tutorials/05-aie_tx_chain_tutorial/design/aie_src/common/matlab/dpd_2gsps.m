function [dpd_out] = dpd_2gsps(dpd_in, mag_input, dpd_kernel_params,dpd_aie_params)
% dpd_2g_filter 
% This is the top level matlab function for modeling AIEngine based 2 Gsps DPD.
% Inputs:
% dpd_in    : DPD input samples at 2 Gsps rate
% mag_input : Magnitude of the DPD input samples
% dpd_kernel_params : DPD kernel parameters from dpd_params.mat file
% dpd_me_params    : AIE engine specific implementation parameters from dpd_param.mat file
% Output :
% dpd_out : DPD output at 2 Gsps

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
t_sig_in = dpd_in; t_mag_in = mag_input;

CascadeOutData1 = zeros(length(dpd_aie_params)-1,floor(length(dpd_in)/2));
CascadeInData1 = zeros(length(dpd_aie_params)-1,floor(length(dpd_in)/2));
CascadeOutData2 = zeros(length(dpd_aie_params)-1,floor(length(dpd_in)/2));
CascadeInData2 = zeros(length(dpd_aie_params)-1,floor(length(dpd_in)/2));


sig_in = t_sig_in(1:2:end); mag_in = t_mag_in(1:2:end);
[ CascadeOutData1(1,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{1}, CascadeInData1(1,:) );
sig_in = aie_circshift(t_sig_in(2:2:end),1); mag_in = t_mag_in(1:2:end); % even/odd
CascadeInData1(2,:) = CascadeOutData1(1,:);
[ CascadeOutData1(2,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{2}, CascadeInData1(2,:) );
sig_in = t_sig_in(1:2:end); mag_in = aie_circshift(t_mag_in(2:2:end),1); % odd/even
CascadeInData1(3,:) = CascadeOutData1(2,:);
[ CascadeOutData1(3,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{3}, CascadeInData1(3,:) );
sig_in = aie_circshift(t_sig_in(2:2:end),1); mag_in = aie_circshift(t_mag_in(2:2:end),1); % even/even
CascadeInData1(4,:) = CascadeOutData1(3,:);
[ CascadeOutData1(4,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{4}, CascadeInData1(4,:) );
dpd_out_p2 = aie_srs(CascadeOutData1(end,:),dpd_aie_params{end}.me_output_shift); % saturate and round output from last core

% compute phase2 of polyphase output
t_sig_in = aie_circshift(dpd_in,1); t_mag_in = aie_circshift(mag_input,1); % delay both inputs
sig_in = t_sig_in(1:2:end); mag_in = t_mag_in(1:2:end); % odd/odd
[ CascadeOutData2(1,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{1}, CascadeInData2(1,:) );
sig_in = aie_circshift(t_sig_in(2:2:end),1); mag_in = t_mag_in(1:2:end); % even/odd
CascadeInData2(2,:) = CascadeOutData2(1,:);
[ CascadeOutData2(2,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{2}, CascadeInData2(2,:) );
sig_in = t_sig_in(1:2:end); mag_in = aie_circshift(t_mag_in(2:2:end),1); % odd/even
CascadeInData2(3,:) = CascadeOutData2(2,:);
[ CascadeOutData2(3,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{3}, CascadeInData2(3,:) );
sig_in = aie_circshift(t_sig_in(2:2:end),1); mag_in = aie_circshift(t_mag_in(2:2:end),1); % even/even
CascadeInData2(4,:) = CascadeOutData2(3,:);
[ CascadeOutData2(4,:)] = aie_dpd_kernel( sig_in, mag_in, dpd_kernel_params, dpd_aie_params{4}, CascadeInData2(4,:) );
dpd_out_p1 = aie_srs(CascadeOutData2(end,:),dpd_aie_params{end}.me_output_shift); % saturate and round output from last core

% interleave output phases to form final output
dpd_out = zeros(1,length(dpd_in));
dpd_out(1:2:end) = dpd_out_p1;
dpd_out(2:2:end) = dpd_out_p2;
end



