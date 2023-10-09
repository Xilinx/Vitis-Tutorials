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
% System Configuration
% ------------------------------------------------------------

app.fs_Gsps = 10.5;
app.Mchan = 16;
app.OSR = 3;
app.P = 8; 
app.Q = 7;
app.oversamp_ratio = app.Mchan*app.Q/app.P;
app.Ncyclic = lcm(app.Mchan,app.oversamp_ratio)/app.oversamp_ratio;
app.Nshift = app.Mchan / app.Ncyclic;
app.fc_Msps = 1000 * app.fs_Gsps / app.Mchan;

% ------------------------------------------------------------
% Create Polyphase Filter Taps
% ------------------------------------------------------------

% Design prototype filter to operate @ app.fc_Msps sampling rate:
app.Ntaps = 8;
app.bw_trans_oneside = 45;
fp = (1-app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
fs = (1+app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
tmp = firgr(app.Ntaps*app.Mchan-2,[0,fp,fs,500*app.fs_Gsps]/(500*app.fs_Gsps),[1,1,0,0]);
taps = [zeros(1,1),tmp];

% Set taps to unity passthrough for debug:
if (0)
  tmp = reshape(taps,app.Mchan,[]);
  tmp(:,:) = 0;
  tmp(1,4) = 1;                             % All have same pass-through impulse (passes)
% $$$   tmp(:,:) = repmat((1/16)*randn(1,8),16,1); % All have same random impulse response (fails)
% $$$   for ii = 1 : 16
% $$$     tmp([ii],randi([1,8],1)) = 1;    % Randomly placed pass-through impulse (fails)
% $$$   end
  taps = reshape(tmp,1,[]);
end

% Quantize taps:
shift = 0;
scale = 2.0^shift;
taps = fi(scale*taps,TT,FF);
fprintf(1,'An shift of %g has been applied to channelizer taps\n',shift);
app.taps = reshape(taps,app.Mchan,[]);

% ------------------------------------------------------------
% Write C++ Polyphase Filter Initialization
% ------------------------------------------------------------

% These select the proper polyphase taps for each tile and filter:
tile_idx = [];
for ii = 0 : 7
  tile_idx = cat(2,tile_idx,circshift([0:15]',-app.oversamp_ratio*ii));
end

% This computes the I/O permutation needed to drive the streams:
all_idx = [];
for ii = 0 : 7
  all_idx = cat(2,all_idx,circshift([0:15]',app.oversamp_ratio*ii));
end

fid = fopen('m16_ssr8_filterbank_taps_init.h','w');
fprintf(fid,'#pragma once\n');
write_taps_file(fid,'tile0_tapsA',app.taps(1+tile_idx( 1,:),:));
write_taps_file(fid,'tile0_tapsB',app.taps(1+tile_idx( 2,:),:));
write_taps_file(fid,'tile1_tapsA',app.taps(1+tile_idx( 3,:),:));
write_taps_file(fid,'tile1_tapsB',app.taps(1+tile_idx( 4,:),:));
write_taps_file(fid,'tile2_tapsA',app.taps(1+tile_idx( 5,:),:));
write_taps_file(fid,'tile2_tapsB',app.taps(1+tile_idx( 6,:),:));
write_taps_file(fid,'tile3_tapsA',app.taps(1+tile_idx( 7,:),:));
write_taps_file(fid,'tile3_tapsB',app.taps(1+tile_idx( 8,:),:));
write_taps_file(fid,'tile4_tapsA',app.taps(1+tile_idx( 9,:),:));
write_taps_file(fid,'tile4_tapsB',app.taps(1+tile_idx(10,:),:));
write_taps_file(fid,'tile5_tapsA',app.taps(1+tile_idx(11,:),:));
write_taps_file(fid,'tile5_tapsB',app.taps(1+tile_idx(12,:),:));
write_taps_file(fid,'tile6_tapsA',app.taps(1+tile_idx(13,:),:));
write_taps_file(fid,'tile6_tapsB',app.taps(1+tile_idx(14,:),:));
write_taps_file(fid,'tile7_tapsA',app.taps(1+tile_idx(15,:),:));
write_taps_file(fid,'tile7_tapsB',app.taps(1+tile_idx(16,:),:));
fclose(fid);

% ------------------------------------------------------------
% Create input signal to Polyphase filter
% ------------------------------------------------------------

Nsamp = 7*1024*app.Mchan;       % This gives 16K samples per stream
backoff_dB = -18;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nsamp),randn(1,Nsamp));

% Quantize:
sig_i = double(fi(sig_i,TT,FF));

% ------------------------------------------------------------
% Golden Model
% ------------------------------------------------------------

taps_rx = double(reshape(taps,app.Mchan,[]));
dac_o = sig_i;
Nblock = numel(dac_o) / app.oversamp_ratio;
data_i = reshape(dac_o,app.oversamp_ratio,[]);
data_i = data_i(end:-1:1,:);            % Reverse order so newer samples at lower indices
state = zeros(app.Mchan,app.Ntaps);             % Filter has Ntaps per phase
rx_filt_o = zeros(app.Mchan,Nblock);
collect_i = zeros(app.Mchan,Nblock);      % Collect input  data into filters
collect_o = zeros(app.Mchan,Nblock);      % Collect output data from filters
collect_s = zeros(app.Mchan,app.Ntaps);      % Collect state used in filters (debug)
perm = 0;
for bb = 1 : Nblock
  % Load next inputs into state:
  state(1:app.oversamp_ratio,1) = data_i(:,bb);
  % Compute next filter output:
  rx_filt_o(:,bb) = ...
      double(fi(sum(taps_rx .* state,2),TT,FF));
  % Collect inputs and outputs:
  % --> Permute to assign to the proper filter engines:
  collect_i(1+all_idx(:,1+perm),bb) = state(:,1);
  collect_s = circshift(collect_s,1,2);
  collect_s(:,1) = state(:,1);
  collect_o(1+all_idx(:,1+perm),bb) = rx_filt_o(:,bb);
  if (0)
    [collect_i(:,bb),state(:,1),all_idx(:,1+perm)]
    disp('here');
    pause;
  end
  if (0)
    aie = real(collect_s)
    act = real([state,all_idx(:,1+perm)])
    perm
    disp('here');
    pause;
  end
  perm = mod(perm+1,8);
  % Update state using serpentine shift:
  tmp = circshift(reshape(state,1,[]),app.oversamp_ratio);
  tmp(1:app.oversamp_ratio) = 0;
  state = reshape(tmp,app.Mchan,[]);
end

% Knock out all filters but the first one:
% $$$ collect_i(2:end,:) = 0;
% $$$ collect_o(2:end,:) = 0;

% Now, collect contains input into filter engines, need to assign to input streams:
sig0_i = reshape(cat(1,reshape(collect_i( 1,:),4,[]),reshape(collect_i( 2,:),4,[])),1,[]);
sig1_i = reshape(cat(1,reshape(collect_i( 3,:),4,[]),reshape(collect_i( 4,:),4,[])),1,[]);
sig2_i = reshape(cat(1,reshape(collect_i( 5,:),4,[]),reshape(collect_i( 6,:),4,[])),1,[]);
sig3_i = reshape(cat(1,reshape(collect_i( 7,:),4,[]),reshape(collect_i( 8,:),4,[])),1,[]);
sig4_i = reshape(cat(1,reshape(collect_i( 9,:),4,[]),reshape(collect_i(10,:),4,[])),1,[]);
sig5_i = reshape(cat(1,reshape(collect_i(11,:),4,[]),reshape(collect_i(12,:),4,[])),1,[]);
sig6_i = reshape(cat(1,reshape(collect_i(13,:),4,[]),reshape(collect_i(14,:),4,[])),1,[]);
sig7_i = reshape(cat(1,reshape(collect_i(15,:),4,[]),reshape(collect_i(16,:),4,[])),1,[]);

[~,~,~] = mkdir('data');
write_stimulus_file('data/sig0_i.txt',sig0_i,TT,FF);
write_stimulus_file('data/sig1_i.txt',sig1_i,TT,FF);
write_stimulus_file('data/sig2_i.txt',sig2_i,TT,FF);
write_stimulus_file('data/sig3_i.txt',sig3_i,TT,FF);
write_stimulus_file('data/sig4_i.txt',sig4_i,TT,FF);
write_stimulus_file('data/sig5_i.txt',sig5_i,TT,FF);
write_stimulus_file('data/sig6_i.txt',sig6_i,TT,FF);
write_stimulus_file('data/sig7_i.txt',sig7_i,TT,FF);

% Now, collect contains input into filter engines, need to assign to input streams:
gld0_o = reshape(cat(1,reshape(collect_o( 1,:),4,[]),reshape(collect_o( 2,:),4,[])),1,[]);
gld1_o = reshape(cat(1,reshape(collect_o( 3,:),4,[]),reshape(collect_o( 4,:),4,[])),1,[]);
gld2_o = reshape(cat(1,reshape(collect_o( 5,:),4,[]),reshape(collect_o( 6,:),4,[])),1,[]);
gld3_o = reshape(cat(1,reshape(collect_o( 7,:),4,[]),reshape(collect_o( 8,:),4,[])),1,[]);
gld4_o = reshape(cat(1,reshape(collect_o( 9,:),4,[]),reshape(collect_o(10,:),4,[])),1,[]);
gld5_o = reshape(cat(1,reshape(collect_o(11,:),4,[]),reshape(collect_o(12,:),4,[])),1,[]);
gld6_o = reshape(cat(1,reshape(collect_o(13,:),4,[]),reshape(collect_o(14,:),4,[])),1,[]);
gld7_o = reshape(cat(1,reshape(collect_o(15,:),4,[]),reshape(collect_o(16,:),4,[])),1,[]);

write_golden_file('data/sig0_o.txt',gld0_o,TT,FF);
write_golden_file('data/sig1_o.txt',gld1_o,TT,FF);
write_golden_file('data/sig2_o.txt',gld2_o,TT,FF);
write_golden_file('data/sig3_o.txt',gld3_o,TT,FF);
write_golden_file('data/sig4_o.txt',gld4_o,TT,FF);
write_golden_file('data/sig5_o.txt',gld5_o,TT,FF);
write_golden_file('data/sig6_o.txt',gld6_o,TT,FF);
write_golden_file('data/sig7_o.txt',gld7_o,TT,FF);

% ============================================================


function write_taps_file( fid, taps_name, taps_val )
   % For four polyphases at a time, We write c0 to c3 first followed by c4 to c7
   % Then we repeat this for the next four polyphases
   % This matches how we compute four lines at a time (ie. four polyphases at a time)
   fprintf(fid,'#define %s std::vector<int16>({\\\n ',taps_name);
   for pp = 1 : 4
     for cc = 1 : 4
       fprintf(fid,'%g,\\\n',taps_val.int(pp,cc));
     end
     fprintf(fid,'\\\n');
   end
   for pp = 1 : 4
     for cc = 5 : 8
       fprintf(fid,'%g,\\\n',taps_val.int(pp,cc));
     end
     fprintf(fid,'\\\n');
   end
   for pp = 5 : 8
     for cc = 1 : 4
       fprintf(fid,'%g,\\\n',taps_val.int(pp,cc));
     end
     fprintf(fid,'\\\n');
   end
   for pp = 5 : 8
     for cc = 5 : 8
       if ( pp == 8 && cc == 8 )
         fprintf(fid,'%g})\n',taps_val.int(pp,cc));
       else
         fprintf(fid,'%g,\\\n',taps_val.int(pp,cc));
       end
     end
     if ( pp < 8 )
       fprintf(fid,'\\\n');
     end
   end
end   

% ============================================================

% Write two 'cint16' samples (4 values) per line since we use plio_64_bits
function write_golden_file( fname, sig_o, TT, FF )
   sig_o = fi(sig_o,TT,FF);
   fid = fopen(fname,'w');
   for tt = 1 : 2 : numel(sig_o)
     fprintf(fid,'%g %g %g %g\n',...
             real(sig_o(tt+0)).int, imag(sig_o(tt+0)).int, ...
             real(sig_o(tt+1)).int, imag(sig_o(tt+1)).int );
   end
   fclose(fid);
end

% ============================================================

% Write two 'cint16' samples (4 values) per line since we use plio_64_bits
function write_stimulus_file( fname, sig_i, TT, FF )
   sig_i = fi(sig_i,TT,FF);
   fid = fopen(fname,'w');
   for tt = 1 : 2 : numel(sig_i)
     fprintf(fid,'%g %g %g %g\n',...
             real(sig_i(tt+0)).int, imag(sig_i(tt+0)).int, ...
             real(sig_i(tt+1)).int, imag(sig_i(tt+1)).int );
   end
   fclose(fid);
end
