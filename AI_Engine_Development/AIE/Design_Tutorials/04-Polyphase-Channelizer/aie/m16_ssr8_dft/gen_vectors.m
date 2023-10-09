%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Nearest','OverflowAction','Saturate');

% ------------------------------------------------------------
% Create input signal to IDFT
% ------------------------------------------------------------

Nsamp = 8*16384;  % 16K samples per stream
backoff_dB = -25;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nsamp),randn(1,Nsamp));

% Quantize:
sig_i = fi(sig_i,TT,FF);

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
twiddle = fi(twiddle,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');

% ------------------------------------------------------------
% Compute Output of First Tile
% ------------------------------------------------------------

% We will take transforms down the columns:
sig_i = reshape(sig_i,M,[]);

% ------------------------------------------------------------
% Compute Output of All Tiles
% ------------------------------------------------------------

% Compute the whole transform:
all_o = zeros(size(sig_i));
check_o = zeros(size(sig_i));
for ii = 1 : size(sig_i,2)
  all_o(:,ii) = double(transpose(sig_i(:,ii))) * double(twiddle);
  check_o(:,ii) = M * ifft(double(sig_i(:,ii)));
end

% Quantize:
all_o   = fi(  all_o,TT,FF);
check_o = fi(check_o,TT,FF);

% ------------------------------------------------------------
% Create Input Files
% ------------------------------------------------------------

[~,~,~] = mkdir('data');

% Tile A Stream 0
stream0 = reshape(sig_i(1:2,:),1,[]);
write_file('data/sig0_i.txt',stream0);

% Tile A Stream 1
stream1 = reshape(sig_i(3:4,:),1,[]);
write_file('data/sig1_i.txt',stream1);

% Tile B Stream 0
stream0 = reshape(sig_i(5:6,:),1,[]);
write_file('data/sig2_i.txt',stream0);

% Tile B Stream 1
stream1 = reshape(sig_i(7:8,:),1,[]);
write_file('data/sig3_i.txt',stream1);

% Tile C Stream 0
stream0 = reshape(sig_i(9:10,:),1,[]);
write_file('data/sig4_i.txt',stream0);

% Tile C Stream 1
stream1 = reshape(sig_i(11:12,:),1,[]);
write_file('data/sig5_i.txt',stream1);

% Tile D Stream 0
stream0 = reshape(sig_i(13:14,:),1,[]);
write_file('data/sig6_i.txt',stream0);

% Tile D Stream 1
stream1 = reshape(sig_i(15:16,:),1,[]);
write_file('data/sig7_i.txt',stream1);

% ------------------------------------------------------------
% Create Golden Outputs
% ------------------------------------------------------------

% Data comes out on even streams first, then odd streams second as we go across the columns:
data = reshape(all_o,4,[]);

stream0 = reshape(data(:,1:8:end),1,[]);
write_file('data/sig0_o.txt',stream0);

stream2 = reshape(data(:,2:8:end),1,[]);
write_file('data/sig2_o.txt',stream2);

stream4 = reshape(data(:,3:8:end),1,[]);
write_file('data/sig4_o.txt',stream4);

stream6 = reshape(data(:,4:8:end),1,[]);
write_file('data/sig6_o.txt',stream6);

stream1 = reshape(data(:,5:8:end),1,[]);
write_file('data/sig1_o.txt',stream1);

stream3 = reshape(data(:,6:8:end),1,[]);
write_file('data/sig3_o.txt',stream3);

stream5 = reshape(data(:,7:8:end),1,[]);
write_file('data/sig5_o.txt',stream5);

stream7 = reshape(data(:,8:8:end),1,[]);
write_file('data/sig7_o.txt',stream7);


% ------------------------------------------------------------
% Dump Out Hard-Coded Twiddles
% ------------------------------------------------------------

% We use 'transpose' here because AIE expects row-major order:
twidA0 = [ reshape(transpose(twiddle( 1: 2,1:4)),1,[]);...
           reshape(transpose(twiddle( 5: 6,1:4)),1,[]);...
           reshape(transpose(twiddle( 9:10,1:4)),1,[]);...
           reshape(transpose(twiddle(13:14,1:4)),1,[]) ];

% We use 'transpose' here because AIE expects row-major order:
twidA1 = [ reshape(transpose(twiddle( 3: 4,1:4)),1,[]);...
           reshape(transpose(twiddle( 7: 8,1:4)),1,[]);...
           reshape(transpose(twiddle(11:12,1:4)),1,[]);...
           reshape(transpose(twiddle(15:16,1:4)),1,[]) ];

twidB0 = [ reshape(transpose(twiddle( 1: 2,5:8)),1,[]);...
           reshape(transpose(twiddle( 5: 6,5:8)),1,[]);...
           reshape(transpose(twiddle( 9:10,5:8)),1,[]);...
           reshape(transpose(twiddle(13:14,5:8)),1,[]) ];

% We use 'transpose' here because AIE expects row-major order:
twidB1 = [ reshape(transpose(twiddle( 3: 4,5:8)),1,[]);...
           reshape(transpose(twiddle( 7: 8,5:8)),1,[]);...
           reshape(transpose(twiddle(11:12,5:8)),1,[]);...
           reshape(transpose(twiddle(15:16,5:8)),1,[]) ];

twidC0 = [ reshape(transpose(twiddle( 1: 2,9:12)),1,[]);...
           reshape(transpose(twiddle( 5: 6,9:12)),1,[]);...
           reshape(transpose(twiddle( 9:10,9:12)),1,[]);...
           reshape(transpose(twiddle(13:14,9:12)),1,[]) ];

% We use 'transpose' here because AIE expects row-major order:
twidC1 = [ reshape(transpose(twiddle( 3: 4,9:12)),1,[]);...
           reshape(transpose(twiddle( 7: 8,9:12)),1,[]);...
           reshape(transpose(twiddle(11:12,9:12)),1,[]);...
           reshape(transpose(twiddle(15:16,9:12)),1,[]) ];

twidD0 = [ reshape(transpose(twiddle( 1: 2,13:16)),1,[]);...
           reshape(transpose(twiddle( 5: 6,13:16)),1,[]);...
           reshape(transpose(twiddle( 9:10,13:16)),1,[]);...
           reshape(transpose(twiddle(13:14,13:16)),1,[]) ];

% We use 'transpose' here because AIE expects row-major order:
twidD1 = [ reshape(transpose(twiddle( 3: 4,13:16)),1,[]);...
           reshape(transpose(twiddle( 7: 8,13:16)),1,[]);...
           reshape(transpose(twiddle(11:12,13:16)),1,[]);...
           reshape(transpose(twiddle(15:16,13:16)),1,[]) ];


write_twiddle_M16_file('twiddle_m16_ssr8_dft.h',twidA0,twidA1,twidB0,twidB1,twidC0,twidC1,twidD0,twidD1);

% ============================================================

% Write two 'cint16' samples (4 values) per line since we assume plio_64_bits
function write_file( fname, vec )
   fid = fopen(fname,'w');
   for ii = 1 : 2 : numel(vec)
     fprintf(fid,'%g %g %g %g\n',...
             real(vec(ii+0)).int,imag(vec(ii+0)).int,...
             real(vec(ii+1)).int,imag(vec(ii+1)).int);
   end
   fclose(fid);
end


function write_twiddle_file( fname, twid0, twid1 )
   fid = fopen(fname,'w');
   fprintf(fid,'#define TWID0 {\\\n{ ');
   for rr = 1 : size(twid0,1)
     for cc = 1 : size(twid0,2)
       if ( cc < size(twid0,2) )                             fprintf(fid,'{%g,%g}, ',     real(twid0(rr,cc)).int,imag(twid0(rr,cc)).int);
       elseif ( cc == size(twid0,2) && rr <  size(twid0,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twid0(rr,cc)).int,imag(twid0(rr,cc)).int);
       elseif ( cc == size(twid0,2) && rr == size(twid0,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twid0(rr,cc)).int,imag(twid0(rr,cc)).int);
       end
     end
     if ( rr < size(twid0,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fprintf(fid,'#define TWID1 {\\\n{ ');
   for rr = 1 : size(twid1,1)
     for cc = 1 : size(twid1,2)
       if ( cc < size(twid1,2) )                             fprintf(fid,'{%g,%g}, ',     real(twid1(rr,cc)).int,imag(twid1(rr,cc)).int);
       elseif ( cc == size(twid1,2) && rr <  size(twid1,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twid1(rr,cc)).int,imag(twid1(rr,cc)).int);
       elseif ( cc == size(twid1,2) && rr == size(twid1,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twid1(rr,cc)).int,imag(twid1(rr,cc)).int);
       end
     end
     if ( rr < size(twid1,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fclose(fid);
end



function write_twiddle_M16_file(fname,twidA0,twidA1,twidB0,twidB1,twidC0,twidC1,twidD0,twidD1)
   fid = fopen(fname,'w');
   fprintf(fid,'//\n');
   fprintf(fid,'// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.\n');
   fprintf(fid,'// SPDX-License-Identifier: MIT\n');
   fprintf(fid,'//\n');
   fprintf(fid,'// Author: Mark Rollins\n\n');
   fprintf(fid,'#define A_TWID0 {\\\n{ ');
   for rr = 1 : size(twidA0,1)
     for cc = 1 : size(twidA0,2)
       if ( cc < size(twidA0,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidA0(rr,cc)).int,imag(twidA0(rr,cc)).int);
       elseif ( cc == size(twidA0,2) && rr <  size(twidA0,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidA0(rr,cc)).int,imag(twidA0(rr,cc)).int);
       elseif ( cc == size(twidA0,2) && rr == size(twidA0,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidA0(rr,cc)).int,imag(twidA0(rr,cc)).int);
       end
     end
     if ( rr < size(twidA0,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fprintf(fid,'#define A_TWID1 {\\\n{ ');
   for rr = 1 : size(twidA1,1)
     for cc = 1 : size(twidA1,2)
       if ( cc < size(twidA1,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidA1(rr,cc)).int,imag(twidA1(rr,cc)).int);
       elseif ( cc == size(twidA1,2) && rr <  size(twidA1,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidA1(rr,cc)).int,imag(twidA1(rr,cc)).int);
       elseif ( cc == size(twidA1,2) && rr == size(twidA1,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidA1(rr,cc)).int,imag(twidA1(rr,cc)).int);
       end
     end
     if ( rr < size(twidA1,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');

   fprintf(fid,'#define B_TWID0 {\\\n{ ');
   for rr = 1 : size(twidB0,1)
     for cc = 1 : size(twidB0,2)
       if ( cc < size(twidB0,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidB0(rr,cc)).int,imag(twidB0(rr,cc)).int);
       elseif ( cc == size(twidB0,2) && rr <  size(twidB0,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidB0(rr,cc)).int,imag(twidB0(rr,cc)).int);
       elseif ( cc == size(twidB0,2) && rr == size(twidB0,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidB0(rr,cc)).int,imag(twidB0(rr,cc)).int);
       end
     end
     if ( rr < size(twidB0,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fprintf(fid,'#define B_TWID1 {\\\n{ ');
   for rr = 1 : size(twidB1,1)
     for cc = 1 : size(twidB1,2)
       if ( cc < size(twidB1,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidB1(rr,cc)).int,imag(twidB1(rr,cc)).int);
       elseif ( cc == size(twidB1,2) && rr <  size(twidB1,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidB1(rr,cc)).int,imag(twidB1(rr,cc)).int);
       elseif ( cc == size(twidB1,2) && rr == size(twidB1,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidB1(rr,cc)).int,imag(twidB1(rr,cc)).int);
       end
     end
     if ( rr < size(twidB1,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');

   fprintf(fid,'#define C_TWID0 {\\\n{ ');
   for rr = 1 : size(twidC0,1)
     for cc = 1 : size(twidC0,2)
       if ( cc < size(twidC0,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidC0(rr,cc)).int,imag(twidC0(rr,cc)).int);
       elseif ( cc == size(twidC0,2) && rr <  size(twidC0,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidC0(rr,cc)).int,imag(twidC0(rr,cc)).int);
       elseif ( cc == size(twidC0,2) && rr == size(twidC0,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidC0(rr,cc)).int,imag(twidC0(rr,cc)).int);
       end
     end
     if ( rr < size(twidC0,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fprintf(fid,'#define C_TWID1 {\\\n{ ');
   for rr = 1 : size(twidC1,1)
     for cc = 1 : size(twidC1,2)
       if ( cc < size(twidC1,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidC1(rr,cc)).int,imag(twidC1(rr,cc)).int);
       elseif ( cc == size(twidC1,2) && rr <  size(twidC1,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidC1(rr,cc)).int,imag(twidC1(rr,cc)).int);
       elseif ( cc == size(twidC1,2) && rr == size(twidC1,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidC1(rr,cc)).int,imag(twidC1(rr,cc)).int);
       end
     end
     if ( rr < size(twidC1,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');

   fprintf(fid,'#define D_TWID0 {\\\n{ ');
   for rr = 1 : size(twidD0,1)
     for cc = 1 : size(twidD0,2)
       if ( cc < size(twidD0,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidD0(rr,cc)).int,imag(twidD0(rr,cc)).int);
       elseif ( cc == size(twidD0,2) && rr <  size(twidD0,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidD0(rr,cc)).int,imag(twidD0(rr,cc)).int);
       elseif ( cc == size(twidD0,2) && rr == size(twidD0,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidD0(rr,cc)).int,imag(twidD0(rr,cc)).int);
       end
     end
     if ( rr < size(twidD0,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   fprintf(fid,'#define D_TWID1 {\\\n{ ');
   for rr = 1 : size(twidD1,1)
     for cc = 1 : size(twidD1,2)
       if ( cc < size(twidD1,2) )                              fprintf(fid,'{%g,%g}, ',     real(twidD1(rr,cc)).int,imag(twidD1(rr,cc)).int);
       elseif ( cc == size(twidD1,2) && rr <  size(twidD1,1) ) fprintf(fid,'{%g,%g} },\\\n',real(twidD1(rr,cc)).int,imag(twidD1(rr,cc)).int);
       elseif ( cc == size(twidD1,2) && rr == size(twidD1,1) ) fprintf(fid,'{%g,%g} } \\\n',real(twidD1(rr,cc)).int,imag(twidD1(rr,cc)).int);
       end
     end
     if ( rr < size(twidD1,1) ) fprintf(fid,'{ '); end
   end
   fprintf(fid,'}\n');
   
   
   fclose(fid);
end




