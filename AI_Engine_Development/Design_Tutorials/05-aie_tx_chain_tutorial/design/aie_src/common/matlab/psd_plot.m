function [Pxx,Fxx]=psd_plot(X,NFFT,Fs,center_dc)
%DPD_PSD compute power spectral density of input.
%   DPD_PSD(X) compute power spectral density of input X.
%
%   DPD_PSD(X, NFFT) compute power spectral density of input X
%   using a NFFT-point FFT.
%
%   DPD_PSD(X, NFFT, Fs) compute power spectral density of input X
%   using a NFFT-point FFT with frequencies scaled to Fs.
%
%   DPD_PSD(X, NFFT, Fs, center_dc) perform fftshift to center the dc term.
%
%   [P,F]=DPD_PSD(...) power spectrum and frequency vectors are output and
%   no plot is generated

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
if(iscell(X))
	X = dfe_check_format(X, 'fea13298', 'combRecursive', 0);
    X = X{1};
    if(NFFT > 1024)
        NFFT = 1024;
    end
end

if(~exist('center_dc','var'))
    center_dc = 0;
end

if (min(size(X))~=1)
    disp('dpd_psd only operates on row or column vectors')
    return;
end
% Convert input to column vector
X=X(:);
% Set default values if not specified
if (nargin < 3)
    Fs = 2;
end
if (nargin < 2)
    if (length(X)<256)
        NFFT = length(X);
    else
        NFFT = 2^8;
    end
end
RealVal = isreal(X);
if (RealVal == 0)
    Fs=Fs*2;
end
% Zero pad input if needed
if (length(X) < NFFT)
    X = [X;zeros(NFFT-length(X),1)];
end
% Determine available averaging
if (length(X) > NFFT)
    AVE = floor(length(X)/NFFT);
    X = X(1:AVE*NFFT);
    X = [X;zeros(AVE*NFFT-length(X),1)];
else
    AVE = 1;
end
X=reshape(X,NFFT,AVE);
%Create Hanning Window
if ~rem(NFFT,2)
    % Even length window
    m=(NFFT)/2;
    w = .5*(1 - cos(2*pi*(1:m)'/(NFFT+1)));
    w = [w; w(end:-1:1)];
else
    % Odd length window
    m=(NFFT+1)/2;
    w = .5*(1 - cos(2*pi*(1:m)'/(NFFT+1)));
    w = [w; w(end-1:-1:1)];
end
%w = hann(length(w));

hm=kron(ones(1,AVE),w);
%Compute PSD
if(exist('no_window','var'))
    Ptmp = abs(fft(X));
    P=Ptmp.^2/((length(X))/2);
else
    P=abs(fft(X.*hm)).^2/((length(X))/2);
end
P=mean(P,2)*1.332;
if (RealVal == 1)
    P=P(1:floor(end/2),:);
    NFFT=NFFT/2;
end
F=linspace(0,1-1/NFFT,NFFT)*Fs/2;
F=F(:);
P=P(:);
if(center_dc)
    P = fftshift(P);
    F = F - (max(F)/2);
end

if (nargout == 0)
    hz='';
    if(max(F) > 1e6)
        F=F/1e6;
        hz=' (MHz)';
    end
    plot(F,10*log10(P))
    grid on
    xlabel(['Frequency' hz])
    ylabel('Power Spectrum Magnitude (dB)')
    axis([min(F) max(F) -inf inf])
else
    Pxx = P;
    Fxx = F;
end



