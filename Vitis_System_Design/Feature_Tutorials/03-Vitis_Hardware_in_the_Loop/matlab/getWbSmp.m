%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz
%
% Generates a composite wideband signal by summing up to 16 sub-channels.
% Each sub-channel can be independently enabled, QAM-modulated, and
% frequency-swept. Persistent state allows seamless sample continuity
% across successive calls.
%

function smp = getWbSmp(Nsmp,upsmp,chan_en,qam_en,swp_en,swp_rate)

persistent qam_coef fir_state prv_carrier_ph prv_sweep_ph frq_nrm nvar gain
if isempty(qam_coef) || isempty(fir_state) || isempty(prv_carrier_ph) || ...
        isempty(prv_sweep_ph) || isempty(frq_nrm) || isempty(nvar) || isempty(gain)
    coef = rcosdesign(0.22,4,upsmp,'normal');
    qam_coef = upsmp * coef / sum(coef(:));
    fir_state = zeros(length(qam_coef)-1,16);
    prv_carrier_ph = randi(Nsmp,1,16);
    prv_sweep_ph = zeros(1,16);
    frq_nrm = [0 1 2 3 4 5 6 7 -8 -7 -6 -5 -4 -3 -2 -1];
    nvar = 0.05;
    gain = 1/6;
end

% array for individual channel samples
wbsmp = complex(zeros(upsmp*Nsmp,16));

% generate baseband QAM samples
I = find(chan_en == 1 & qam_en == 1);
if ~isempty(I)
    num_qam = length(I);
    iq = 2*randi([0 3],num_qam*Nsmp,2)-3;
    cplx = (iq(:,1) + 1i*iq(:,2)) / sqrt(10);
    qsym = reshape(cplx,Nsmp,num_qam);
    [qsamp, fir_state(:,I)] = filter(qam_coef,1,upsample(qsym,upsmp),fir_state(:,I));
    wbsmp(:,I) = qsamp;
end

% assign ones to enabled channels with no QAM
I = find(chan_en == 1 & qam_en == 0);
if ~isempty(I)
    wbsmp(:,I) = ones(upsmp*Nsmp,length(I));
end

% get carrier phase samples
I = find(chan_en == 1);
J = find(chan_en == 1 & swp_en == 1);
if ~isempty(I)
    cr_cntr = mod(repmat((1:upsmp*Nsmp)',1,length(I)) + prv_carrier_ph(I),2^32);
    prv_carrier_ph(I) = cr_cntr(end,:);
    cr_cntr = cr_cntr .* frq_nrm(I) / 16;
    % if sweep is enabled
    if ~isempty(J)
        swp_cntr = mod(repmat((1:upsmp*Nsmp)',1,length(J)) + prv_sweep_ph(J),2^32);
        prv_sweep_ph(J) = swp_cntr(end,:);
        swp_cntr = swp_cntr.^2 .* swp_rate(J) * 1e-8;
        [~, ia, ~] = intersect(I,J);
        cr_cntr(:,ia) = cr_cntr(:,ia) + swp_cntr;
    end
    carr_smp = exp(1i*2*pi*cr_cntr);
    wbsmp(:,I) = wbsmp(:,I) .* carr_smp;
end

% add noise and set signal level
noise = nvar * (randn(upsmp*Nsmp,1) + 1i* randn(upsmp*Nsmp,1));
smp = gain * (sum(wbsmp,2) + noise);
end
