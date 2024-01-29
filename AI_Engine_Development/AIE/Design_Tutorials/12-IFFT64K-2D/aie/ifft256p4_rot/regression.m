%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [1,256+4,60];
   elseif (nargin == 1)          settings = [1,256+4,60];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   addpath('../../matlab');
   Nport      = settings(1);
   Nfft       = settings(2);
   Ntransform = settings(3);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   % Samples in polyphase order:
   act_o = zeros(Nport,Nfft/Nport,Ntransform);
   gld_o = zeros(Nport,Nfft/Nport,Ntransform);
   for pp = 1 : Nport
     tmp = load_aiesim(sprintf('%s/data/sig_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     act_o(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/sig_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     gld_o(pp,:,:) = tmp;
   end
   % Outputs in polyphase order:
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
   level = 2^17;
   if ( max_err <= level )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
