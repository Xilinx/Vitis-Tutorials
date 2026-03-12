%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

clear all;
close all;
rng(1);
% ------------------------------------------------------------
% System Parameters
% ------------------------------------------------------------

NPORTS_I=2;
NPORTS_O=2;

TP_SSR = 32;
Niter = 1;
Nsamp = Niter * 4096;

% ------------------------------------------------------------
% Create Channelizer Taps
% ------------------------------------------------------------

TT_16 = numerictype(1,16,15);
TT_32 = numerictype(1,32,31);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% ------------------------------------------------------------
% I/O Samples
% ------------------------------------------------------------

backoff_dB = -15;
sig_i = complex(0:1:Nsamp-1,zeros(1,Nsamp))/2^15;
sig_i = fi(sig_i,TT_16,FF);
sig_i = reshape(sig_i,TP_SSR,[],Niter);

% ------------------------------------------------------------
% Store I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

for nn = 1 : NPORTS_I
  fid_i = fopen(sprintf('data/sig_i_%d.txt',nn-1),'w');
  tmp = reshape(sig_i(nn:2:end),1,[]);
  fprintf(fid_i,'%d\n',tmp.int);
  fclose(fid_i);
end

% Packet headers should be alined with Work/reports/packet_switching_report.json
packet_header = ["0xFFF0008","0x8FFF0000","0x8FFF000C","0xFFF0004",...
                 "0x8FFF0009","0xFFF0001","0xFFF000D","0x8FFF0005",...
                 "0x8FFF000A","0xFFF0002","0xFFF000E","0x8FFF0006",...
                 "0xFFF000B","0x8FFF0003","0x8FFF000F","0xFFF0007"];

N_STREAMS_SPLIT_FROM_PKT = TP_SSR/NPORTS_O;
% Stride-8 reordering: output packets in order 0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15
% Generate packet order array to match C++ pkt_idx calculation
packet_order = zeros(1, N_STREAMS_SPLIT_FROM_PKT);
for ii = 0:N_STREAMS_SPLIT_FROM_PKT-1
    packet_order(ii+1) = floor(ii / 2) + mod(ii, 2) * 8 + 1;  % +1 for MATLAB 1-based indexing
end

for nn = 1 : NPORTS_O
  fid_i = fopen(sprintf('data/sig_o_%d.txt',nn-1),'w');
  for kk=1:Niter
      data_i = sig_i((nn-1)*N_STREAMS_SPLIT_FROM_PKT+1:nn*N_STREAMS_SPLIT_FROM_PKT,:,kk);
      for ii=1:N_STREAMS_SPLIT_FROM_PKT
          % Get data from the packet that should appear at this output position
          pkt_idx = packet_order(ii);
          data_ii = data_i(pkt_idx,:);
          % First word: header + 3 samples (128-bit)
          fprintf(fid_i,'%d %d %d %d 65535 0\n',hex2dec(packet_header(pkt_idx)),...
                  typecast([real(data_ii.int(1)),imag(data_ii.int(1))],'uint32'),...
                  typecast([real(data_ii.int(2)),imag(data_ii.int(2))],'uint32'),...
                  typecast([real(data_ii.int(3)),imag(data_ii.int(3))],'uint32'));
          % Middle words: 4 samples per word (128-bit)
          for jj = 4 : 4 : numel(data_ii)-1
              fprintf(fid_i,'%d %d %d %d 65535 0\n',...
                      typecast([real(data_ii.int(jj)),imag(data_ii.int(jj))],'uint32'),...
                      typecast([real(data_ii.int(jj+1)),imag(data_ii.int(jj+1))],'uint32'),...
                      typecast([real(data_ii.int(jj+2)),imag(data_ii.int(jj+2))],'uint32'),...
                      typecast([real(data_ii.int(jj+3)),imag(data_ii.int(jj+3))],'uint32'));
          end
          % Last word: 1 sample + 3 padding (128-bit), TKEEP=15, TLAST=1
          fprintf(fid_i,'%d %d %d %d 15 1\n',...
                  typecast([real(data_ii.int(jj+4)),imag(data_ii.int(jj+4))],'uint32'),0,0,0);
      end
  end
  fclose(fid_i);
end
