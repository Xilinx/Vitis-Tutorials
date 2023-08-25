%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [2,8*7*16,9];
   elseif (nargin == 1)          settings = [2,8*7*16,9];
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
   act_o = zeros(Nfft,Ntransform);
   gld_o = zeros(Nfft,Ntransform);
   for pp = 1 : Nport
     tmp = load_aiesim(sprintf('%s/data/sig_o_%d.txt',base,pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     act_o(:,pp:2:end) = tmp;

     tmp = load_aiesim(sprintf('data/sig_o_%d.txt',pp-1),'int',1);
     tmp = reshape(tmp,[],Ntransform/2);
     gld_o(:,pp:2:end) = tmp;
   end
   act_o = reshape(act_o,1,[]);
   gld_o = reshape(gld_o,1,[]);
   err = act_o - gld_o;

   if (1)
     subplot(3,1,1); plot(real(gld_o),'b.-'); hold on; plot(real(act_o),'r.--'); hold off; ylabel('real');
     subplot(3,1,2); plot(imag(gld_o),'b.-'); hold on; plot(imag(act_o),'r.--'); hold off; ylabel('imag');
     subplot(3,1,3); plot(real(err),'k.-'); hold on; plot(imag(err),'m.-'); hold off; ylabel('abs(err)');
   end
   max_err = max([abs(real(err)),abs(imag(err))]);
   fprintf(1,'Max Err: %g\n',max_err);
   if ( max_err <= 33 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
