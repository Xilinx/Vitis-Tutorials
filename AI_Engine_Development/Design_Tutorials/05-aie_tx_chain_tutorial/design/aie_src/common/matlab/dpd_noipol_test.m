% dpd_noipol_filter
% Top level script to run dpd noipol filter model

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
close all;
clear all;
disp('Top level script for testing DPD (No LUT Iterpolation)')

% load the Noipol DPD params s-> dpd_params dpd_aie_params
 
% Load reference input for DPD
load('dpd_noipol_refin.mat');
% dpd_sig_in = zeros(1,1024);
% dpd_sig_in(65:64+19) = 8192;
% dpd_mag_in = abs(dpd_sig_in);

use_case=2;
if (use_case == 1)
    load('dpd_noipol_params_13taps.mat');
    dpd_params.term_selection    = [1 1 0 0 0;...
                                    1 1 1 0 0;...
                                    0 1 1 1 0;...
                                    0 0 1 1 1;...
                                    0 0 0 1 1];   
    aie_delay = 5;
    dpd_aie_params.phases = 1;        
elseif (use_case == 2)
    load('dpd_noipol_params_52taps.mat');    
    dpd_params.term_selection    = [1 3 1 3 0 0 0 0 0 0;...
                                    2 4 2 4 0 0 0 0 0 0;...
                                    1 3 1 3 1 3 0 0 0 0;...
                                    2 4 2 4 2 4 0 0 0 0;...
                                    0 0 1 3 1 3 1 3 0 0;...
                                    0 0 2 4 2 4 2 4 0 0;...
                                    0 0 0 0 1 3 1 3 1 3;...
                                    0 0 0 0 2 4 2 4 2 4;...
                                    0 0 0 0 0 0 1 3 1 3;...
                                    0 0 0 0 0 0 2 4 2 4];    
    dpd_aie_params.phases = 2;    
    aie_delay =11;
end
[dpd_params,dpd_aie_params] = assign_noipolterms_toaie(dpd_params,dpd_aie_params);
% dpd_params = rmfield(dpd_params,'mmp_LUTs');
% M=size(dpd_params.term_selection,2);
% R=size(dpd_params.term_selection,1);
% for i=1:M
%     for j=1:R
%         if (dpd_params.term_selection(j,i) ~= 0)
%             dpd_params.mmp_LUTs(j,i,1:256) = randi([-2^6 2^6-1],1,256)+1i*randi([-2^6 2^6-1],1,256);
%         end
%     end
% end
dpd_aie_params.aie_dpd_cores = max(max(dpd_params.term_selection))*dpd_aie_params.phases;
dpd_out     = dpd_noipol(dpd_sig_in,dpd_mag_in,dpd_params,dpd_aie_params);
dpd_out_aie = dpd_noipol_aie(dpd_sig_in,dpd_mag_in,dpd_params,dpd_aie_params);

plot(real(dpd_out-dpd_out_aie(1+aie_delay:length(dpd_out)+aie_delay)),'r')
figure;
plot(real(dpd_out)); hold on; plot(real(dpd_out_aie(1+aie_delay:length(dpd_out)+aie_delay)),'r')



