% 
% © Copyright 2023 Advanced Micro Devices, Inc.
%
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%     http:%www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.

clear all;
close all;
rng(1);

[~,~,~] = mkdir('png');

% ------------------------------------------------------------
% Generate Filter Taps
% ------------------------------------------------------------

% First use case is running @ 400 Msps
Fs1_Msps = 400;
fp_MHz = 100;
fs_MHz = 150;

% Generate a 'two-tone' signal with one tone in filter stopband:
A1 = 0.2;
A2 = 0.4;
A = A1 + A2;
Ftone1_MHz = 20;
Ftone2_MHz = 150;

% Design a filter with 20*log10(1e-3) = -60 dB stop band attenuation:
taps1_orig = firgr('mineven',[0,fp_MHz,fs_MHz,0.5*Fs1_Msps]/(0.5*Fs1_Msps),[1,1,0,0],[1e-3,1e-3]);

% Quantize to 'int16':
taps1_fxp = fi(taps1_orig,1,16,15,'RoundingMethod','Nearest','OverflowAction','Saturate');
taps1_flt = double(taps1_fxp);

% Plot filter response
[H1,f1] = freqz(taps1_flt,1,8192,Fs1_Msps);
figure;
subplot(2,1,1); stem(taps1_flt);
xlabel('Samples'); ylabel('Magnitude'); grid on;
title(sprintf('Filter #1 Impulse Response (%g taps)',length(taps1_orig)));
subplot(2,1,2); plot(f1,20*log10(abs(H1)),'b-'); 
xlabel('Frequency (MHz)'); ylabel('Magnitude'); grid on;
title('Filter #1 Frequency Response');
xline(Ftone1_MHz,'r');
xline(Ftone2_MHz,'r');
v = axis; axis([v(1),v(2),-80,v(4)]);
saveas(gcf,'png/filter1.png');

% Store taps for AIE implementation:
fid = fopen('fir1_coeff.h','w');
fprintf(fid,'#define FIR1_COEFF {\\\n');
last = (length(taps1_orig)+1)/2;
for tt = 1 : last
  if ( tt == last ) fprintf(fid,'%d }\n',taps1_fxp.int(tt));
  else              fprintf(fid,'%d,\\\n',taps1_fxp.int(tt));
  end
end
fclose(fid);

% ------------------------------------------------------------
% Generate Simulus I/O
% ------------------------------------------------------------

% Note: Total number of samples will be 4 x WIN_SIZE, so we are generating data for 4 AIE graph cycles:
WIN_SIZE = 2048;
Nsamp1 = 4 * WIN_SIZE;
tone1 = A1 * exp(1i*2*pi*Ftone1_MHz/Fs1_Msps*[0:Nsamp1-1]);
tone2 = A2 * exp(1i*2*pi*Ftone2_MHz/Fs1_Msps*[0:Nsamp1-1]);
sig_i = tone1 + tone2;

% Quantize:
sig_i_fxp  = fi(sig_i,1,16,15,'RoundingMethod','Nearest','OverflowAction','Saturate');
sig_i_flt = double(sig_i_fxp);

% Apply filter to generate expected outputs:
sig_o = filter(taps1_flt,1,sig_i_flt);

% Quantize:
sig_o_fxp = fi(sig_o,1,16,15,'RoundingMethod','Nearest','OverflowAction','Saturate');
sig_o_flt = double(sig_o_fxp);

% Plot inputs and outputs:
figure;
subplot(2,1,1); plot(real(sig_i_flt),'b.-');
xlabel('Sample Index'); ylabel('Real part'); title('Filter #1 Input');
axis([1,128,-A,+A]);
subplot(2,1,2); plot(real(sig_o_flt),'b.-');
xlabel('Sample Index'); ylabel('Real part'); title('Filter #1 Output');
axis([1,128,-A,+A]);
saveas(gcf,'png/filter1_io.png');

% Save the signals for AIE simulation (assuming plio_64_bits):
[~,~,~] = mkdir('data');
fid = fopen('data/sig_i.txt','w');
for ii = 1 : 2 :Nsamp1
  fprintf(fid,'%d %d %d %d\n',...
          real(sig_i_fxp(ii+0)).int,imag(sig_i_fxp(ii+0)).int,...
          real(sig_i_fxp(ii+1)).int,imag(sig_i_fxp(ii+1)).int);
end
fclose(fid);
fid = fopen('data/fir_o.txt','w');
for ii = 1 : 2 : Nsamp1
  fprintf(fid,'%d %d %d %d\n',...
          real(sig_o_fxp(ii+0)).int,imag(sig_o_fxp(ii+0)).int,...
          real(sig_o_fxp(ii+1)).int,imag(sig_o_fxp(ii+1)).int);
end
fclose(fid);

% ------------------------------------------------------------
% FFT
% ------------------------------------------------------------

scale = 0.5^6;
Nfft = 256;
% Compute FFT of two-tone input:
tmp = reshape(sig_i_flt,Nfft,[]);
fft_i = scale*fft(tmp,[],1);
fft_i_fxp = fi(fft_i,1,16,15,'RoundingMethod','Nearest','OverflowAction','Saturate');
fft_i_flt = double(fft_i_fxp);

% Compute FFT of filtered output:
tmp = reshape(sig_o_flt,Nfft,[]);
fft_o = scale*fft(tmp,[],1);
fft_o_fxp = fi(fft_o,1,16,15,'RoundingMethod','Nearest','OverflowAction','Saturate');
fft_o_flt = double(fft_o_fxp);

figure;
ff = linspace(0,Fs1_Msps,1+Nfft); ff = ff(1:end-1);
subplot(2,1,1);
plot(ff,abs(fft_i_flt(:,2)),'b.-');
xlabel('Frequency (MHx)'); ylabel('Magnitude'); grid on;
title('Filter #1 Input Spectrum');
subplot(2,1,2);
plot(ff,abs(fft_o_flt(:,2)),'b.-');
xlabel('Frequency (MHx)'); ylabel('Magnitude'); grid on;
title('Filter #1 Output Spectrum');
saveas(gcf,'png/fir1_spectrum.png');

% Save the signals for AIE simulation (assuming plio_64_bits):
fid = fopen('data/fft_o.txt','w');
data = reshape(fft_o_fxp,1,[]);
for ii = 1 : 2 : Nsamp1
  fprintf(fid,'%d %d %d %d\n',...
          real(data(ii+0)).int,imag(data(ii+0)).int,...
          real(data(ii+1)).int,imag(data(ii+1)).int);
end
fclose(fid);
