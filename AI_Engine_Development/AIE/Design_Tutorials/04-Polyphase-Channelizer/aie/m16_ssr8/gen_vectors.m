%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

rng(1);

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Nearest','OverflowAction','Saturate');

% ------------------------------------------------------------
% Channelizer Model
% ------------------------------------------------------------

SSR_I = 7;
SSR_O = 8;

% Specify # samples to match depth of DDR PL Data Movers:
% --> to yield desired 512 x 4 samples per stream, we need to scale down by 16 since channelizer model scales up 'dac_o' by this amount:
NSAMP_PER_CHAN = 512 / 16 * 4;  

% Initialize:
app = channelizer_init( NSAMP_PER_CHAN, TT, FF );

% Run model:
[data_i,fft_o] = channelizer_model( app, TT, FF );

% ------------------------------------------------------------
% Dump File I/O for HW Emulation / VCK190
% ------------------------------------------------------------

[~,~,~] = mkdir('data');

% Need to send these '4 samples at a time' for each stream, interleaving streams after each set of 4:
data_i = fi(data_i,TT,FF);
fid = fopen('data/sig_i.txt','w');
for ss = 1 : 2 : size(data_i,2)
  for xx = 1 : SSR_I
    fprintf(fid,'%d %d\n',real(data_i(xx,      ss+0)).int,imag(data_i(xx,      ss+0)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx+SSR_I,ss+0)).int,imag(data_i(xx+SSR_I,ss+0)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx,      ss+1)).int,imag(data_i(xx,      ss+1)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx+SSR_I,ss+1)).int,imag(data_i(xx+SSR_I,ss+1)).int);
  end
end
fclose(fid);

% Quantize for printing:
fft_o = fi(fft_o,TT,FF);

% Need to send these '4 samples at a time' for each stream, interleaving streams after each set of 4:
fid = fopen('data/sig_o.txt','w');
fft_o = reshape(fft_o,4,[]);
for ss = 1 : size(fft_o,2)
  fprintf(fid,'%d %d\n',real(fft_o(1,ss)).int,imag(fft_o(1,ss)).int);
  fprintf(fid,'%d %d\n',real(fft_o(2,ss)).int,imag(fft_o(2,ss)).int);
  fprintf(fid,'%d %d\n',real(fft_o(3,ss)).int,imag(fft_o(3,ss)).int);
  fprintf(fid,'%d %d\n',real(fft_o(4,ss)).int,imag(fft_o(4,ss)).int);
end
fclose(fid);

% ------------------------------------------------------------
% Channelizer Initialization
% ------------------------------------------------------------

function [app] = channelizer_init( NSAMP_PER_CHAN, TT, FF )

   app = [];
   app.OSR_0.Value = 0;
   app.OSR_1.Value = 0;
   app.OSR_2.Value = 0;
   app.OSR_3.Value = 1;
   app.SamplingFrequencySwitch.Value = 10.5;
   app.ChannelsMKnob.Value = 16;
   app.Active.Value = 5;
   app.fs_Gsps = 10.5;
   app.Mchan = 16;
   app.Ntaps = 8;
   app.Active.Limits = [1,app.Mchan-2];
   list = randi([1,app.Mchan],1,10000);
   list_use = setdiff(list-1,[0,app.Mchan/2],'stable');
   app.chan_id = sort(list_use(1:app.Active.Value));
   app.bw_trans_oneside = 45;                 % Single-sided transition bandwidth (percent)
                                              % Signal construction filter:
   app.sig_passband_ripple_dB = 0.10;            % Peak-to-peak ripple
   app.sig_stopband_attenu_dB = -80;
   app.sig_passband = 0.50;                    % Fraction of 0.5*fc_Msps
   app.sig_stopband = 0.65;                    % Fraction of 0.5*fc_Msps
   app.Nframe = 1024;
   app.Nfrac_bits = 11;
   if ( app.OSR_0.Value == 1 )
     app.OSR = 0;
     app.P = 1;
     app.Q = 1;
     app.oversamp_ratio = app.Mchan*app.Q/app.P;
     app.Ncyclic = 1;
     app.Nshift = 0;
   elseif ( app.OSR_1.Value == 1)
     app.OSR = 1;
     app.P = 2;
     app.Q = 1;
     app.oversamp_ratio = app.Mchan*app.Q/app.P;
     app.Ncyclic = lcm(app.Mchan,app.oversamp_ratio)/app.oversamp_ratio;
     app.Nshift = app.Mchan / app.Ncyclic;
   elseif (app.OSR_2.Value == 1)
     app.OSR = 2;
     app.P = 4;
     app.Q = 3;
     app.oversamp_ratio = app.Mchan*app.Q/app.P;
     app.Ncyclic = lcm(app.Mchan,app.oversamp_ratio)/app.oversamp_ratio;
     app.Nshift = app.Mchan / app.Ncyclic;
   elseif (app.OSR_3.Value == 1)
     app.OSR = 3;
     app.P = 8; 
     app.Q = 7;
     app.oversamp_ratio = app.Mchan*app.Q/app.P;
     app.Ncyclic = lcm(app.Mchan,app.oversamp_ratio)/app.oversamp_ratio;
     app.Nshift = app.Mchan / app.Ncyclic;
   end
   app.Nsamp = app.Q * NSAMP_PER_CHAN;
   app.fc_Msps = 1000 * app.fs_Gsps / app.Mchan;
   app.fs_Msps = 1000 * app.fs_Gsps / app.Mchan * app.P / app.Q;
   % Design prototype filter to operate @ app.fc_Msps sampling rate:
   fp = (1-app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
   fs = (1+app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
   tmp = firgr(app.Ntaps*app.Mchan-2,[0,fp,fs,500*app.fs_Gsps]/(500*app.fs_Gsps),[1,1,0,0]);
   taps = [zeros(1,1),tmp];
   taps = double(fi(taps,TT,FF));
   app.taps = reshape(taps,app.Mchan,[]);
   
   % Design prototype filter to operate @ app.fc_Msps sampling rate (since implemented at TX):
   fp = app.sig_passband * 0.5 * app.fc_Msps;
   fs = app.sig_stopband * 0.5 * app.fc_Msps;
   ripple = [0.5*(10^(0.05*app.sig_passband_ripple_dB)-1),10^(0.05*app.sig_stopband_attenu_dB)];
   app.taps_sig = firgr('minevn',[0,fp,fs,0.5*app.fc_Msps]/(0.5*app.fc_Msps),[1,1,0,0],ripple);
   
   % Compute channel centers:
   tmp = [-0.5*app.fs_Gsps:app.fs_Gsps/app.Mchan:0.5*app.fs_Gsps];
   tmp = circshift(tmp(1:end-1),-app.Mchan/2);
   app.chan_centers_GHz = tmp(1+app.chan_id);
end

% ------------------------------------------------------------
% Channelizer Model
% ------------------------------------------------------------

function [data_i,fft_o] = channelizer_model( app, TT, FF )

   % ------------------------------------------------------------
   % Create Input Channels
   % ------------------------------------------------------------

   backoff_dB = -15;
   sig_i = zeros(numel(app.chan_id),app.Nsamp);
   for mm = 1 : numel(app.chan_id)
     tmp = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,app.Nsamp),randn(1,app.Nsamp));
     sig_i(mm,:) = filter(app.taps_sig,1,tmp);
   end
   sig_i = double(fi(sig_i,1,13,12,'RoundingMethod','Nearest','OverflowAction','Saturate'));

   % ------------------------------------------------------------
   % Perform TX IFFT
   % ------------------------------------------------------------

   % Note: use of IFFT here matches 'tuning concept' for upconversion:
   ifft_i = zeros(app.Mchan,app.Nsamp);
   ifft_i(1+app.chan_id,:) = sig_i;
   ifft_o = zeros(app.Mchan,app.Nsamp);
   for ss = 1 : app.Nsamp
     tmp = ifft_i(:,ss);       
     ifft_o(:,ss) = app.Mchan*ifft(tmp);
   end

   % ------------------------------------------------------------
   % Perform Polyphase Filtering at DAC:
   % ------------------------------------------------------------

   taps_tx = reshape(app.Mchan*app.taps,app.Mchan,[]); % Note scale factor due to interpolation
   filt_o = zeros(app.Mchan,app.Nsamp);
   for mm = 1 : app.Mchan
     tx_filt_o(mm,:) = filter(taps_tx(mm,:),1,ifft_o(mm,:));
   end
   dac_o = reshape(tx_filt_o,1,[]);
   
   % Quantize for H/W implementation:
   dac_o = double(fi(dac_o,TT,FF));

   % ------------------------------------------------------------
   % Perform Polyphase Filtering at ADC:
   % ------------------------------------------------------------

   % Now we assume P/Q oversampling at the RX channelizer:
   taps_rx = reshape(app.taps,app.Mchan,[]);
   Nblock = numel(dac_o) / app.oversamp_ratio;
   data_i = reshape(dac_o,app.oversamp_ratio,[]);
   data_i = data_i(end:-1:1,:);            % Reverse order so newer samples at lower indices
   state = zeros(app.Mchan,app.Ntaps);             % Filter has Ntaps per phase
   rx_filt_o = zeros(app.Mchan,Nblock);
   for bb = 1 : Nblock
     % Load next inputs into state:
     state(1:app.oversamp_ratio,1) = data_i(:,bb);
     % Compute next filter output:
     rx_filt_o(:,bb) = sum(taps_rx .* state,2);
     % Update state using serpentine shift:
     tmp = circshift(reshape(state,1,[]),app.oversamp_ratio);
     tmp(1:app.oversamp_ratio) = 0;
     state = reshape(tmp,app.Mchan,[]);
   end

   % Quantize to match AIE model:
   rx_filt_o = double(fi(rx_filt_o,TT,FF));

   % Introduce 16 blocks of latency to match system:
   rx_filt_o = cat(2,zeros(app.Mchan,16),rx_filt_o(:,1:end-16));
   
   % ------------------------------------------------------------
   % Create Twiddle Matrix for IDFT
   % ------------------------------------------------------------

   M = 16;
   twiddle = zeros(M,M);
   omega = exp(1i*2*pi/M);   
   for rr = 1 : M
     for cc = 1 : M
       twiddle(rr,cc) = omega^((rr-1)*(cc-1));  
     end
   end
   % Take transpose since we formulate the diagrams this way:
   twiddle = transpose(twiddle);

   % Quantize:
   twiddle = double(fi(twiddle,TT,'RoundingMethod','Convergent','OverflowAction','Saturate'));
   
   % ------------------------------------------------------------
   % Cyclic Shift and RX IFFT
   % ------------------------------------------------------------

   state = 0;
   fft_o = zeros(app.Mchan,Nblock);
   all_o = zeros(app.Mchan,Nblock);
   for bb = 1 : Nblock
     % Compute shift amount:
     shift = state * app.Nshift;
     % Perform shift for next FFT input:
     fft_i = circshift(rx_filt_o(:,bb),shift);
     % Apply transform:
     all_o(:,bb) = transpose(fft_i) * twiddle;
     fft_o(:,bb) = app.Mchan * ifft( fft_i );
     % Update state:
     state = mod(state+1,app.Ncyclic);
   end

   % Quantize:
   all_o = double(fi(all_o,TT,FF));
   fft_o = double(fi(fft_o,TT,FF));

   % Check if DFT model matches FFT model:
   if (0)
     figure;
     for ii = 1 : app.Mchan
       subplot(4,4,ii); plot(2^15*real(fft_o(ii,:)-all_o(ii,:)),'b.-'); hold on; plot(2^15*imag(fft_o(ii,:)-all_o(ii,:)),'r.-'); hold off;
     end
   end
   % Return DFT model:
   fft_o = all_o;
end
