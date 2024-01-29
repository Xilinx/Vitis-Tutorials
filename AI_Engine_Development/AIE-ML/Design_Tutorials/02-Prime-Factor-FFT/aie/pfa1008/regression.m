%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [8,4*1008];
   elseif (nargin == 1)          settings = [8,4*1008];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   addpath('../../matlab');
   Ntransform = settings(1);
   Nfft       = settings(2);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   tmp = load_aiesim(sprintf('%s/data/sig_o_aie.txt',base),'int',1);
   act_o = reshape(tmp,1,[]);

   tmp = load_aiesim('data/sig_o_aie.txt','int',1);
   gld_o = reshape(tmp,1,[]);

   % Outputs dealt in linear order:
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
