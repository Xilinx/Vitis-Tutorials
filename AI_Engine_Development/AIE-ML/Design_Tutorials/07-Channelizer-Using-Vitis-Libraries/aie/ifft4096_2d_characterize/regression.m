%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [1,4096,8];
   elseif (nargin == 1)          settings = [1,4096,8];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   addpath('../../scripts');
   close all;
   Nports       = settings(1);
   Nfft         = settings(2);
   Ntransform   = settings(3);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   % Samples in polyphase order:
   front_act_o = zeros(Nports,Nfft/Nports,Ntransform);
   front_gld_o = zeros(Nports,Nfft/Nports,Ntransform);
   for pp = 1 : Nports
     tmp = load_aiesim(sprintf('%s/data/front_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_act_o(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/front_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_gld_o(pp,:,:) = tmp;
   end
   back_act_o = zeros(Nports,Nfft/Nports,Ntransform);
   back_gld_o = zeros(Nports,Nfft/Nports,Ntransform);
   for pp = 1 : Nports
     tmp = load_aiesim(sprintf('%s/data/back_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     back_act_o(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/back_o_%d.txt',pp-1),'int32',1);
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

   if (0)
     figure;
     subplot(3,1,1); plot(real(front_gld_o),'b.-'); hold on; plot(real(front_act_o),'r.--'); hold off; ylabel('real');
     subplot(3,1,2); plot(imag(front_gld_o),'b.-'); hold on; plot(imag(front_act_o),'r.--'); hold off; ylabel('imag');
     subplot(3,1,3); plot(real(err_front),'k.-'); hold on; plot(imag(err_front),'m.-'); hold off; ylabel('abs(err_front)');
     figure;
     subplot(3,1,1); plot(real(back_gld_o),'b.-'); hold on; plot(real(back_act_o),'r.--'); hold off; ylabel('real');
     subplot(3,1,2); plot(imag(back_gld_o),'b.-'); hold on; plot(imag(back_act_o),'r.--'); hold off; ylabel('imag');
     subplot(3,1,3); plot(real(err_back),'k.-'); hold on; plot(imag(err_back),'m.-'); hold off; ylabel('abs(err_back)');
   end
   max_err_front = max([abs(real(err_front)),abs(imag(err_front))]);
   fprintf(1,'Max err_front: %g\n',max_err_front);
   max_err_back = max([abs(real(err_back)),abs(imag(err_back))]);
   fprintf(1,'Max err_back: %g\n',max_err_back);
   level = 2^16;
   if ( and(max_err_front <= level,max_err_back <= level) )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
