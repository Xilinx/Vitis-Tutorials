%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [2,8,1008];
   elseif (nargin == 1)          settings = [2,8,1008];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   addpath('../../matlab');
   Nport      = settings(1);
   Ntransform = settings(2);
   Nfft       = settings(3);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   % Even transforms on ss0, odd transforms on ss1
   act7_o = zeros(Nfft,Ntransform);
   gld7_o = zeros(Nfft,Ntransform);
   act9_o = zeros(Nfft,Ntransform);
   gld9_o = zeros(Nfft,Ntransform);
   act16_o = zeros(Nport,4,Ntransform*Nfft/Nport/4);
   gld16_o = zeros(Nport,4,Ntransform*Nfft/Nport/4);
   for pp = 1 : Nport
     % ---------- DFT7 ----------
     tmp = load_aiesim(sprintf('%s/data/dft7_o_%d.txt',base,pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     act7_o(:,pp:2:end) = tmp;
     tmp = load_aiesim(sprintf('data/dft7_o_%d.txt',pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     gld7_o(:,pp:2:end) = tmp;
     % ---------- DFT9 ----------
     tmp = load_aiesim(sprintf('%s/data/dft9_o_%d.txt',base,pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     act9_o(:,pp:2:end) = tmp;
     tmp = load_aiesim(sprintf('data/dft9_o_%d.txt',pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     gld9_o(:,pp:2:end) = tmp;
     % ---------- DFT16 ----------
     tmp = load_aiesim(sprintf('%s/data/dft16_o_%d.txt',base,pp-1),'int',1);
     tmp = reshape(tmp,4,[]);
     act16_o(pp,:,:) = tmp;
     tmp = load_aiesim(sprintf('data/dft16_o_%d.txt',pp-1),'int',1);
     tmp = reshape(tmp,4,[]);
     gld16_o(pp,:,:) = tmp;
   end
   % Outputs dealt in linear order:
   act7_o = reshape(act7_o,1,[]);
   gld7_o = reshape(gld7_o,1,[]);
   act9_o = reshape(act9_o,1,[]);
   gld9_o = reshape(gld9_o,1,[]);
   act16_o = reshape(act16_o,1,[]);
   gld16_o = reshape(gld16_o,1,[]);
   err7  = act7_o  - gld7_o;
   err9  = act9_o  - gld9_o;
   err16 = act16_o - gld16_o;

   if (1)
     subplot(3,3,1); plot(real(gld7_o),'b.-'); hold on; plot(real(act7_o),'r.--'); hold off; ylabel('real');
     subplot(3,3,2); plot(imag(gld7_o),'b.-'); hold on; plot(imag(act7_o),'r.--'); hold off; ylabel('imag');
     subplot(3,3,3); plot(real(err7),'k.-'); hold on; plot(imag(err7),'m.--'); hold off; ylabel('imag');
     subplot(3,3,4); plot(real(gld9_o),'b.-'); hold on; plot(real(act9_o),'r.--'); hold off; ylabel('real');
     subplot(3,3,5); plot(imag(gld9_o),'b.-'); hold on; plot(imag(act9_o),'r.--'); hold off; ylabel('imag');
     subplot(3,3,6); plot(real(err9),'k.-'); hold on; plot(imag(err9),'m.--'); hold off; ylabel('imag');
     subplot(3,3,7); plot(real(gld16_o),'b.-'); hold on; plot(real(act16_o),'r.--'); hold off; ylabel('real');
     subplot(3,3,8); plot(imag(gld16_o),'b.-'); hold on; plot(imag(act16_o),'r.--'); hold off; ylabel('imag');
     subplot(3,3,9); plot(real(err16),'k.-'); hold on; plot(imag(err16),'m.--'); hold off; ylabel('imag');
   end
   max_err = max([abs(real(err7)),abs(imag(err7)),abs(real(err9)),abs(imag(err9)),abs(real(err16)),abs(imag(err16))]);
   fprintf(1,'Max Err: %g\n',max_err);
   if ( max_err <= 33 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
