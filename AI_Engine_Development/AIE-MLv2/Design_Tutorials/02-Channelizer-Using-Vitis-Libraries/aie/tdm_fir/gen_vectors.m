%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

clear all;
close all;
rng(1);
% ------------------------------------------------------------
% System Parameters
% ------------------------------------------------------------

Fs_Gsps = 2;
NCHAN = 4096;
TP_FIR_LEN = 36;

TP_SSR = 32;
Niter = 4;

NPORTS_I=2;
NPORTS_O=4;

MARGIN = TP_FIR_LEN * NCHAN / TP_SSR;
Nsamp = Niter * 4096;
Ntaps = NCHAN * TP_FIR_LEN;

fprintf(1,'NCHAN:      %g\n',NCHAN);
fprintf(1,'MARGIN:     %g\n',MARGIN);
fprintf(1,'Ntaps:      %g\n',Ntaps);
fprintf(1,'Nsamp:      %g\n',Nsamp);

% ------------------------------------------------------------
% Create Channelizer Taps
% ------------------------------------------------------------

TT_16 = numerictype(1,16,15);
TT_32 = numerictype(1,32,31);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

sep = 0.15;
sfact = 32;  % Design filter using frequency-domain upsampling with sfact factor
NtapsL = Ntaps / sfact;
tapsL = firgr(NtapsL-1,[0,(0.5+sep)/(NCHAN/sfact),(1-sep)/(NCHAN/sfact),1],[1,1,0,0]);
tapsLf = fft(tapsL);
tapsHf = [tapsLf(1:NtapsL/2),zeros(1,(sfact-1)*NtapsL),tapsLf(1+NtapsL/2:end)];
taps = real(ifft(tapsHf));
taps_fxp = fi(taps,TT_32,FF);
[Hxx,Fxx] = freqz(double(taps_fxp),1,64*Ntaps,Fs_Gsps);
figure;
idx = find(Fxx < Fs_Gsps);
subplot(2,2,1); plot(Fxx(idx),20*log10(abs(Hxx(idx))),'k-');
xlabel('Frequency (GHz)'); ylabel('Magnitude (dB)'); grid on;
v = axis; axis([v(1) v(2),-100,0]);
title(sprintf('Fs: %g Gsps  # Channels: %g',Fs_Gsps,NCHAN));
idx = find(Fxx < Fs_Gsps/NCHAN);
subplot(2,2,3); plot(1e3*Fxx(idx),20*log10(abs(Hxx(idx))),'k-');
xlabel('Frequency (MHz)'); ylabel('Magnitude (dB)'); grid on;
title(sprintf('Ntaps: %g',Ntaps));
v = axis; axis([v(1) v(2),-100,0]);
subplot(2,2,[2,4]); plot(taps_fxp,'b-');
title(sprintf('# Taps/Phase: %g',TP_FIR_LEN));
grid on;

% ------------------------------------------------------------
% Store taps for design
% ------------------------------------------------------------

taps_hw = reshape(taps_fxp,NCHAN,TP_FIR_LEN);

% Assign 16 channels to each instance:
fid = fopen('firbank_init_taps.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Faisal El-Shabani\n');
fprintf(fid,'#pragma once\n');
fprintf(fid,'#define TAPS_INIT_0 { ');
range = [1:NCHAN];
taps_u = reshape(taps_hw(range,:),1,[]);
fprintf(fid,'%d, ',  taps_u.int(1:end-1));
fprintf(fid,'%d }\n',taps_u.int(end));
fclose(fid);

% ------------------------------------------------------------
% I/O Samples
% ------------------------------------------------------------

backoff_dB = -15;
sig_i = sqrt(0.5)*10^(backoff_dB/20)*complex(randn(1,Nsamp),randn(1,Nsamp));
sig_i = fi(sig_i,TT_16,FF);
sig_i = reshape(sig_i,NCHAN,[]);

% Filter the channels along the rows:
taps_use = double(reshape(taps_hw,NCHAN,TP_FIR_LEN));
sig_o = zeros(size(sig_i));
for cc = 1 : NCHAN
  sig_o(cc,:) = filter(taps_use(cc,:),1,double(sig_i(cc,:)));
end
sig_o = fi(sig_o,TT_32,FF);

sig_i = reshape(sig_i,TP_SSR,[],Niter);
sig_o = reshape(sig_o,TP_SSR,[],Niter);

% ------------------------------------------------------------
% Store I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Packet headers should be alined with Work/reports/packet_switching_report.json
packet_header_port0 = ["0xFFF0008","0x8FFF0009","0x8FFF000A","0xFFF000B",...
                 "0x8FFF000C","0xFFF000D","0xFFF000E","0x8FFF000F",...
                 "0xFFF0004","0x8FFF0005","0x8FFF0006","0xFFF0007",...
                 "0x8FFF0000","0xFFF0001","0xFFF0002","0x8FFF0003"];
packet_header_port1 = ["0x8FFF000C","0xFFF000D","0xFFF000E","0x8FFF000F",...
                 "0xFFF0008","0x8FFF0009","0x8FFF000A","0xFFF000B",...
                 "0x8FFF0000","0xFFF0001","0xFFF0002","0x8FFF0003",...
                 "0xFFF0004","0x8FFF0005","0x8FFF0006","0xFFF0007"];
% packet_header_port0 = ["0x8FFF0000","0xFFF0001","0xFFF0002","0x8FFF0003",...
%                  "0xFFF0004","0x8FFF0005","0x8FFF0006","0xFFF0007",...
%                  "0xFFF0008","0x8FFF0009","0x8FFF000A","0xFFF000B",...
%                  "0x8FFF000C","0xFFF000D","0xFFF000E","0x8FFF000F"];
% packet_header_port1 = ["0x8FFF0000","0xFFF0001","0xFFF0002","0x8FFF0003",...
%                  "0xFFF0004","0x8FFF0005","0x8FFF0006","0xFFF0007",...
%                  "0xFFF0008","0x8FFF0009","0x8FFF000A","0xFFF000B",...
%                  "0x8FFF000C","0xFFF000D","0xFFF000E","0x8FFF000F"];


N_STREAMS_SPLIT_FROM_PKT = TP_SSR/NPORTS_I;

% Packet Size = 128, interleaved packet ordering
for nn = 1 : NPORTS_I
  if nn==1
      packet_header = packet_header_port0;
  else
      packet_header = packet_header_port1;
  end
  fid_i = fopen(sprintf('data/filterbank_i_%d.csv',nn-1),'w');
  fprintf(fid_i,'CMD, D, D, TKEEP, TLAST\n');
  for kk=1:Niter
      data_i = sig_i((nn-1)*N_STREAMS_SPLIT_FROM_PKT+1:nn*N_STREAMS_SPLIT_FROM_PKT,:,kk);
      offset=0;
      for jj=1:N_STREAMS_SPLIT_FROM_PKT
          if (jj~=1 && mod(jj-1,2)==0 ) % 2 output ports per input port
              offset=offset+1;
          end
          data_ii = data_i(mod((jj-1)*8 + 1,N_STREAMS_SPLIT_FROM_PKT)+offset,:);
          fprintf(fid_i,'DATA, %d, %d, 0x0, 0\n',hex2dec(packet_header(mod((jj-1)*8 + 1,N_STREAMS_SPLIT_FROM_PKT)+offset)),typecast([real(data_ii.int(1)),imag(data_ii.int(1))],'uint32'));
          for ii = 2 : 2 : numel(data_ii)-1
              fprintf(fid_i,'DATA, %d, %d, 0x0, 0\n',typecast([real(data_ii.int(ii)),imag(data_ii.int(ii))],'uint32'),typecast([real(data_ii.int(ii+1)),imag(data_ii.int(ii+1))],'uint32'));
          end
          fprintf(fid_i,'DATA, %d, %d, 0x0F, 1\n',typecast([real(data_ii.int(ii+2)),imag(data_ii.int(ii+2))],'uint32'),0);
      end
  end
  fclose(fid_i);
end

% Packet Size = 128, linear packet ordering
% for nn = 1 : NPORTS_I
%   if nn==1
%       packet_header = packet_header_port0;
%   else
%       packet_header = packet_header_port1;
%   end
%   fid_i = fopen(sprintf('data/filterbank_i_%d.csv',nn-1),'w');
%   fprintf(fid_i,'CMD, D, D, TKEEP, TLAST\n');
%   for kk=1:Niter
%       data_i = sig_i((nn-1)*N_STREAMS_SPLIT_FROM_PKT+1:nn*N_STREAMS_SPLIT_FROM_PKT,:,kk);
%       for jj=1:N_STREAMS_SPLIT_FROM_PKT
%           data_ii = data_i(jj,:);
%           fprintf(fid_i,'DATA, %d, %d, 0x0, 0\n',hex2dec(packet_header(mod(jj-1,N_STREAMS_SPLIT_FROM_PKT)+1)),typecast([real(data_ii.int(1)),imag(data_ii.int(1))],'uint32'));
%           for ii = 2 : 2 : numel(data_ii)-1
%               fprintf(fid_i,'DATA, %d, %d, 0x0, 0\n',typecast([real(data_ii.int(ii)),imag(data_ii.int(ii))],'uint32'),typecast([real(data_ii.int(ii+1)),imag(data_ii.int(ii+1))],'uint32'));
%           end
%           fprintf(fid_i,'DATA, %d, %d, 0x0F, 1\n',typecast([real(data_ii.int(ii+2)),imag(data_ii.int(ii+2))],'uint32'),0);
%       end
%   end
%   fclose(fid_i);
% end

% Packet Size = 1, linear packet ordering
% for nn = 1 : NPORTS_I
%   if nn==1
%       packet_header = packet_header_port0;
%   else
%       packet_header = packet_header_port1;
%   end
%   fid_i = fopen(sprintf('data/filterbank_i_%d.csv',nn-1),'w');
%   fprintf(fid_i,'CMD, D, D, TKEEP, TLAST\n');
%   for kk=1:Niter
%       data_i = reshape(sig_i((nn-1)*N_STREAMS_SPLIT_FROM_PKT+1:nn*N_STREAMS_SPLIT_FROM_PKT,:,kk),[],1);
%       for ii = 1 : numel(data_i)
%           fprintf(fid_i,'DATA, %d, %d, -1, 1\n',hex2dec(packet_header(mod(ii-1,N_STREAMS_SPLIT_FROM_PKT)+1)),typecast([real(data_i.int(ii)),imag(data_i.int(ii))],'uint32'));
%       end
%   end
%   fclose(fid_i);
% end

for nn = 1 : NPORTS_O
  fid_o = fopen(sprintf('data/filterbank_o_%d.txt',nn-1),'w');
  for kk=1:Niter
      data_o = sig_o((nn-1)*TP_SSR/NPORTS_O+1:nn*TP_SSR/NPORTS_O,:,kk);
      data_o = reshape(transpose(data_o),1,[]);
      for ii = 1 : numel(data_o)
          fprintf(fid_o,'%d %d\n',...
              real(data_o(ii  )).int,imag(data_o(ii  )).int);
      end
  end
  fclose(fid_o);
end
