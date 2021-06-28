function [ CoreOutput] = aie_dpd_kernel(signal, mag, params, me_params, cascade_in)
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


if isfield(me_params,'me_dpd_phases')
    num_phases = me_params.me_dpd_phases;
else
    num_phases = 1;
end

% Get the required parameters from parameter struct

 LUTs_me        = params.LUTs_me;
 interp         = params.interp ;
 mag_width      = params.mag_width; 
 lut_addr_width = params.lut_addr_width ;
 scale_width    = params.scale_width ;
% allocate output array
CoreOutput = zeros(1,length(signal));

% model of data path filter with ME finite precision
% process all active terms allocated to core
for t = 1:length(me_params.terms)

    m = me_params.terms(t,1); % signal delay
    r = me_params.terms(t,2); % magnitude delay
    idx = me_params.terms(t,3);  % index into full array of LUTs but only with active terms
    if m > 0
        delayed_mag = circshift(mag,[0 r]);

        % do interpolation only if requested
        if(interp == 0)
            % non-interplated LUTs (round-up)
            LUT_index = tc_round(delayed_mag, mag_width-lut_addr_width);
            LUT_frac = zeros(size(LUT_index));
        else
            LUT_index = floor(delayed_mag / 2^(mag_width-lut_addr_width));
            LUT_frac = bitand(delayed_mag,(2^(mag_width-lut_addr_width)-1));
        end
        LUT_res = LUTs_me(idx, 1 + LUT_index);
        LUT_res_p1 = LUTs_me(idx, 1 + LUT_index + 1);
        % compute linear fractional interpolation value at full
        % precision
        intLUT(t, :) = (2^scale_width)*LUT_res(1,:)+(LUT_res_p1(1,:)-LUT_res(1,:)).*LUT_frac;
    else
        % m <= 0 indicates no active term; add zero column
        intLUT(t,:) = zeros(1,length(mag));
    end

end

% model post-add combining adjacent 'lanes' of interpolated LUT
% values; precision of result limited to 16 bits in ME with SRS
% instruction
for t2 = 1:length(me_params.terms)/2
    % get m values for adjacent terms and add only if appropriate
    m1 = me_params.terms((t2-1)*2+1,1);
    m2 = me_params.terms((t2-1)*2+2,1);
    if (m1==m2 || m1==0 || m2==0)
        AdjLUTPostAdd(t2,:) = aie_srs((intLUT((t2-1)*2+1,:) + intLUT((t2-1)*2+2,:)), me_params.post_add_shift);
    else
        error('Adjacent terms not suitable for interpolation post-add');
    end
end

% multiply post-added LUT values by signal with corresponding delay
for t2 = 1:length(me_params.terms)/2
    m1 = me_params.terms((t2-1)*2+1,1);
    m2 = me_params.terms((t2-1)*2+2,1);
    if (m1~=0 || m2~=0)
        % not both columns were zero; pick delay for non-zero column
        m = max(m1,m2);
        delayed_data = circshift(signal,[0 m]);
        CoreActTerms(t2,:) = delayed_data.*AdjLUTPostAdd(t2,:);
    else
        CoreActTerms(t2,:) = AdjLUTPostAdd(t2,:);
    end
end

% add terms contributions to core output based on number of phases with 
% full precision in ME accumulators
if num_phases == 1
    % 1 output every 2 clock cycles (1/2 rate) in ME (0.5 GSPS)
    CoreOutput(:) = sum(CoreActTerms,1); 
elseif num_phases == 2
    % 1 output every 1 clock cycle in ME (1 GSPS)
    CoreOutput(1:num_phases:end) = sum(CoreActTerms([1 2 5 6],1:num_phases:end),1);
    CoreOutput(2:num_phases:end) = sum(CoreActTerms([3 4 7 8],2:num_phases:end),1);
elseif num_phases == 4
    % 2 outputs every 1 clock cycle in ME (2 GSPS)
    CoreOutput(1:num_phases:end) = sum(CoreActTerms([1 2],1:num_phases:end),1);
    CoreOutput(2:num_phases:end) = sum(CoreActTerms([3 4],2:num_phases:end),1);
    CoreOutput(3:num_phases:end) = sum(CoreActTerms([5 6],3:num_phases:end),1);
    CoreOutput(4:num_phases:end) = sum(CoreActTerms([7 8],4:num_phases:end),1);
else
    error('Illegan number of DPD phases');
end

CoreOutput = CoreOutput + cascade_in;

end
