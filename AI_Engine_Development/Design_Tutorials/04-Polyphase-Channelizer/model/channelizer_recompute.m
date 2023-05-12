%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function channelizer_recompute(app)
   app.fs_Gsps = app.SamplingFrequencySwitch.Value;
   app.Mchan = app.ChannelsMKnob.Value;
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
   app.Nsamp = app.Q * 4096;
   app.fc_Msps = 1000 * app.fs_Gsps / app.Mchan;
   app.fs_Msps = 1000 * app.fs_Gsps / app.Mchan * app.P / app.Q;
   % Design prototype filter to operate @ app.fc_Msps sampling rate:
   fp = (1-app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
   fs = (1+app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
   tmp = firgr(app.Ntaps*app.Mchan-2,[0,fp,fs,500*app.fs_Gsps]/(500*app.fs_Gsps),[1,1,0,0]);
   app.taps = [zeros(1,1),tmp];
   
   % Design prototype filter to operate @ app.fc_Msps sampling rate (since implemented at TX):
   fp = app.sig_passband * 0.5 * app.fc_Msps;
   fs = app.sig_stopband * 0.5 * app.fc_Msps;
   ripple = [0.5*(10^(0.05*app.sig_passband_ripple_dB)-1),10^(0.05*app.sig_stopband_attenu_dB)];
   app.taps_sig = firgr('minevn',[0,fp,fs,0.5*app.fc_Msps]/(0.5*app.fc_Msps),[1,1,0,0],ripple);
   % Plot Proto Filter:
   ff = linspace(-1.5*app.fs_Msps,1.5*app.fs_Msps,1e5);
   [Hf,ff] = freqz(app.taps,1,ff,1000*app.fs_Gsps);
   xx = linspace(-0.5*app.fc_Msps,0.5*app.fc_Msps,1e5);
   [Hx,xx] = freqz(app.taps_sig,1,xx,app.fc_Msps);
   stem(app.UIAxes_ProtoFilter,app.taps);
   grid(app.UIAxes_ProtoFilter,'on');
   title(app.UIAxes_ProtoFilter,sprintf('Proto Filter (%g taps, %g taps/phase)',numel(app.taps),app.Ntaps));
   plot(app.UIAxes_FreqDomain,ff,20*log10(abs(Hf)),'r-',xx,20*log10(abs(Hx)),'b-');
   grid(app.UIAxes_FreqDomain,'on');
   xlabel(app.UIAxes_FreqDomain,'Frequency (MHz)');
   ylabel(app.UIAxes_FreqDomain,'Mag (dB)');
   title(app.UIAxes_FreqDomain,sprintf('fc=%.1f Msps, fs=%.1f Msps, P/Q=%g/%g',app.fc_Msps,app.fs_Msps,app.P,app.Q));
   xlim(app.UIAxes_FreqDomain,'auto')
   ylim(app.UIAxes_FreqDomain,[-100,10]);
   legend(app.UIAxes_FreqDomain,'proto','sig');
   % Create Panel to display channelizer output:
   col = ceil(sqrt(numel(app.chan_id)));
   row = ceil(numel(app.chan_id)/col);
   app.tile_o_hdl = tiledlayout(app.ChannelizerOutputsPanel,row,col);
   app.tile_i_hdl = tiledlayout(app.ChannelizerInputsPanel,1,1);
   % Compute channel centers:
   tmp = [-0.5*app.fs_Gsps:app.fs_Gsps/app.Mchan:0.5*app.fs_Gsps];
   tmp = circshift(tmp(1:end-1),-app.Mchan/2);
   app.chan_centers_GHz = tmp(1+app.chan_id);
end
