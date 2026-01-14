%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [32,8,4096,4];
   elseif (nargin == 1)          settings = [32,8,4096,4];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   addpath('../../scripts');
   close all;
   Nport_tdm   = settings(1);
   Nports_fft  = settings(2);
   Nsamp       = settings(3);
   Ntransform  = settings(4);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   
   % TDM check
   act_o = zeros(Nport_tdm,Nsamp*Ntransform/Nport_tdm);
   gld_o = zeros(Nport_tdm,Nsamp*Ntransform/Nport_tdm);
   for pp = 1 : Nport_tdm
     tmp = load_aiesim(sprintf('%s/data/filterbank_o_%d.txt',base,pp-1),'int32',1);
     act_o(pp,:) = tmp;
     tmp = load_aiesim(sprintf('data/filterbank_o_%d.txt',pp-1),'int32',1);
     gld_o(pp,:) = tmp;
   end
   act_o = reshape(act_o,1,[]);
   gld_o = reshape(gld_o,1,[]);
   err_o = act_o - gld_o;

   max_tdm_err = max([abs(real(err_o)),abs(imag(err_o))]);
   fprintf(1,'Max TDM err: %g\n',max_tdm_err);

   if ( max(abs(err_o)) < 2 )
     fprintf(1,'--- TDM PASSED ---\n');
   else
     fprintf(1,'*** TDM FAILED ***\n');
   end

   % 2D FFT check
   front_act_o = zeros(Nports_fft,Nsamp/Nports_fft,Ntransform);
   front_gld_o = zeros(Nports_fft,Nsamp/Nports_fft,Ntransform);
   for pp = 1 : Nports_fft
     tmp = load_aiesim(sprintf('%s/data/fft_front_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_act_o(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/fft_front_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_gld_o(pp,:,:) = tmp;
   end
   back_act_o = zeros(Nports_fft,Nsamp/Nports_fft,Ntransform);
   back_gld_o = zeros(Nports_fft,Nsamp/Nports_fft,Ntransform);
   for pp = 1 : Nports_fft
     tmp = load_aiesim(sprintf('%s/data/fft_back_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     back_act_o(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/fft_back_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     back_gld_o(pp,:,:) = tmp;
   end
   % Outputs in polyphase order:
   front_act_o = reshape(front_act_o,1,[]);
   front_gld_o = reshape(front_gld_o,1,[]);
   err_front = front_act_o - front_gld_o;
   back_act_o = reshape(back_act_o,1,[]);
   back_gld_o = reshape(back_gld_o,1,[]);
   err_back = back_act_o - back_gld_o;

   max_err_front = max([abs(real(err_front)),abs(imag(err_front))]);
   fprintf(1,'Max err_front: %g\n',max_err_front);
   max_err_back = max([abs(real(err_back)),abs(imag(err_back))]);
   fprintf(1,'Max err_back: %g\n',max_err_back);
   level = 2^16;
   if ( and(max_err_front <= level,max_err_back <= level) )
     fprintf(1,'--- 2D FFT PASSED ---\n');
   else
     fprintf(1,'*** 2D FFT FAILED ***\n');
   end
end
