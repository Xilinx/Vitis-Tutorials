%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function channelizer_simple( app )

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

   % Plot channelizer inputs:
   bw = 0.5*(app.sig_passband+app.sig_stopband)*numel(app.chan_id)/app.Mchan;
   [Pxx,fxx] = pwelch(dac_o/std(dac_o)*sqrt(app.fs_Gsps*bw),hanning(1e3),[],1e3,app.fs_Gsps,'centered');
   ax = nexttile(app.tile_i_hdl);
   plot(ax,fxx,10*log10(Pxx),'b-'); 
   xlabel(ax,'Frequency (GHz)'); 
   ylabel(ax,'PSD (dBc)'); 
   grid(ax,'on');
   axis(ax,[-0.5*app.fs_Gsps,0.5*app.fs_Gsps,-60,10]);
   title(ax,sprintf('Channelizer Input (Mchan=%g,%g Gsps)',app.Mchan,app.fs_Gsps));
   if (app.Active.Value <= 14)
     for cc = 1 : numel(app.chan_id)
       text(ax,app.chan_centers_GHz(cc),5,sprintf('%g',app.chan_id(cc)),'HorizontalAlignment','center',...
            'FontWeight','Bold');
     end
   end

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

   % ------------------------------------------------------------
   % Cyclic Shift and RX IFFT
   % ------------------------------------------------------------

   state = 0;
   fft_o = zeros(app.Mchan,Nblock);
   for bb = 1 : Nblock
     % Compute shift amount:
     shift = state * app.Nshift;
     % Perform shift for next FFT input:
     fft_i = circshift(rx_filt_o(:,bb),shift);
     % Apply transform:
     fft_o(:,bb) = app.Mchan * ifft( fft_i );
     % Update state:
     state = mod(state+1,app.Ncyclic);
   end
   sig_o = fft_o(1+app.chan_id,:);
   
   % ------------------------------------------------------------
   % Print Output Channels vs. Input Channels
   % ------------------------------------------------------------
   
   % Note: there is a phase offset on each channel that has not yet been compensated

   % Compensate for group delay and resample input to see if it matches channelizer output:
   del = length(app.taps)/app.Mchan - 1;
   data = cat(2,zeros(numel(app.chan_id),del),sig_i(:,1:end-del));
   sig_x = transpose(resample(transpose(data),app.P,app.Q));

   % Plot channelizer outputs:
   for ii = 1 : numel(app.chan_id)
     ax = nexttile(app.tile_o_hdl);
     plot(ax,abs(sig_x(ii,:)),'b.-'); hold(ax,'on'); 
     plot(ax,abs(sig_o(ii,:)),'r.-'); hold(ax,'off');
     xlim(ax,[1000,1100]);
     ylim(ax,'auto');
     title(ax,sprintf('Channel: %g',app.chan_id(ii))); 
     grid(ax,'on'); 
     ylabel(ax,'abs');
   end
end

