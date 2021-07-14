function [dpd_out] = dpd_noipol_aie(dpd_sig_in, dpd_mag_in, params,aie_params)
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

if(~isfield(aie_params,'phases'))   
    aie_params.phases = 1;  
end
if(~isfield(aie_params,'lane_config'))   
    aie_params.lane_config = 2;  
end
Nmod8 = floor(length(dpd_sig_in)/8)*8; % make sure number of samples processed is multiple of 8
dpd_sig_in = [dpd_sig_in(1:Nmod8) zeros(1,64)];
dpd_mag_in = [dpd_mag_in(1:Nmod8) zeros(1,64)];

if (aie_params.phases == 1)
    CascadeOutData1 = zeros(aie_params.aie_dpd_cores,floor(length(dpd_sig_in)),4);
    CascadeInData1 = zeros(aie_params.aie_dpd_cores,floor(length(dpd_sig_in)),4);

    % mechanization to feed signal/magnitude samples through cores
    for i = 1:aie_params.aie_dpd_cores
        if (aie_params.aie_dpd_cores ~= 1)
            if (i == 1)
                dpd_sig_in_t = circshift(dpd_sig_in,-6);
                dpd_mag_in_t = circshift(dpd_mag_in,-6);
            elseif (i == 2)
                dpd_sig_in_t = circshift(dpd_sig_in,-3);
                dpd_mag_in_t = circshift(dpd_mag_in,-6);
            elseif (i == 3)
                dpd_sig_in_t = circshift(dpd_sig_in,-3);
                dpd_mag_in_t = circshift(dpd_mag_in,0);
            else
                dpd_sig_in_t = circshift(dpd_sig_in,-2);
                dpd_mag_in_t = circshift(dpd_mag_in,-2);
            end
        else
            dpd_sig_in_t = dpd_sig_in;
            dpd_mag_in_t = dpd_mag_in;
        end
        [CascadeOutData1(i,:,:),dpd_out(i,:)] = aie_dpd_noipol_kernel( dpd_sig_in_t, dpd_mag_in_t, params, aie_params, reshape(CascadeInData1(i,:,:),[],4), i);
        CascadeInData1(i+1,:,:) = CascadeOutData1(i,:,:);
    end
    dpd_out = aie_srs(dpd_out(end,:),aie_params.aie_output_shift); % saturate and round output from last core
else    
    aie_cores_perphase = aie_params.aie_dpd_cores/aie_params.phases;
    dpd_sig_in_e = dpd_sig_in(1:2:end);
    dpd_sig_in_o = dpd_sig_in(2:2:end);
    dpd_mag_in_e = dpd_mag_in(1:2:end);
    dpd_mag_in_o = dpd_mag_in(2:2:end);
    CascadeOutData1_e = zeros(aie_cores_perphase,floor(length(dpd_sig_in_e)),4);
    CascadeInData1_e = zeros(aie_cores_perphase,floor(length(dpd_sig_in_e)),4);
    CascadeOutData1_o = zeros(aie_cores_perphase,floor(length(dpd_sig_in_o)),4);
    CascadeInData1_o = zeros(aie_cores_perphase,floor(length(dpd_sig_in_o)),4);
    for i = 1:aie_cores_perphase
        if (i == 1)
            dpd_sig_in_te = circshift(dpd_sig_in_o,-8);
            dpd_mag_in_te = circshift(dpd_mag_in_o,-8);
            dpd_sig_in_to = circshift(dpd_sig_in_e,-9);
            dpd_mag_in_to = circshift(dpd_mag_in_e,-9);
        elseif (i == 2)
            dpd_sig_in_te = circshift(dpd_sig_in_o,-5);
            dpd_mag_in_te = circshift(dpd_mag_in_e,-6);
            dpd_sig_in_to = circshift(dpd_sig_in_e,-6);
            dpd_mag_in_to = circshift(dpd_mag_in_o,-6);
        elseif (i == 3)
            dpd_sig_in_te = circshift(dpd_sig_in_e,-3);
            dpd_mag_in_te = circshift(dpd_mag_in_o,-2);
            dpd_sig_in_to = circshift(dpd_sig_in_o,-3);
            dpd_mag_in_to = circshift(dpd_mag_in_e,-3);
        else
            dpd_sig_in_te = dpd_sig_in_e;
            dpd_mag_in_te = dpd_mag_in_e;
            dpd_sig_in_to = dpd_sig_in_o;
            dpd_mag_in_to = dpd_mag_in_o;
        end
        [CascadeOutData1_e(i,:,:),dpd_out_e(i,:)] = aie_dpd_noipol_kernel( dpd_sig_in_te, dpd_mag_in_te, params, aie_params, reshape(CascadeInData1_e(i,:,:),[],4), i);
        CascadeInData1_e(i+1,:,:) = CascadeOutData1_e(i,:,:);
        [CascadeOutData1_o(i,:,:),dpd_out_o(i,:)] = aie_dpd_noipol_kernel( dpd_sig_in_to, dpd_mag_in_to, params, aie_params, reshape(CascadeInData1_o(i,:,:),[],4), i);
        CascadeInData1_o(i+1,:,:) = CascadeOutData1_o(i,:,:);
    end
    dpd_out = aie_srs(reshape([dpd_out_e(end,:); dpd_out_o(end,:)],1,[]),aie_params.aie_output_shift); % saturate and round output from last core    
end

end




