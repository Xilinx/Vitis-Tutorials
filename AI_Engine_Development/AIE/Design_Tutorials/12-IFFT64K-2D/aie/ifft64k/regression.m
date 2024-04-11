%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [5,(256+4)^2,1];
   elseif (nargin == 1)          settings = [5,(256+4)^2,1];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   addpath('../../matlab');
   Nport      = settings(1);
   Nfft       = settings(2);
   Ntransform = settings(3);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   
   % Front End:
   front_a = zeros(Nport,Nfft/Nport,Ntransform);
   front_g = zeros(Nport,Nfft/Nport,Ntransform);
   for pp = 1 : Nport
     tmp = load_aiesim(sprintf('%s/data/front_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_a(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/front_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     front_g(pp,:,:) = tmp;
   end
   front_a = reshape(front_a,1,[]);   
   front_g = reshape(front_g,1,[]);
   front_e = front_a - front_g;

   % Back End:
   back_a = zeros(Nport,Nfft/Nport,Ntransform);
   back_g = zeros(Nport,Nfft/Nport,Ntransform);
   for pp = 1 : Nport
     tmp = load_aiesim(sprintf('%s/data/back_o_%d.txt',base,pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     back_a(pp,:,:) = tmp;

     tmp = load_aiesim(sprintf('data/back_o_%d.txt',pp-1),'int32',1);
     tmp = reshape(tmp,[],Ntransform);
     back_g(pp,:,:) = tmp;
   end
   back_a = reshape(back_a,1,[]);   
   back_g = reshape(back_g,1,[]);
   back_e = back_a - back_g;

   if (1)
     subplot(3,2,1); plot(real(front_g),'b.-'); hold on; plot(real(front_a),'r.--'); hold off; ylabel('real'); title('Front');
     subplot(3,2,3); plot(imag(front_g),'b.-'); hold on; plot(imag(front_a),'r.--'); hold off; ylabel('imag'); title('Front');
     subplot(3,2,5); plot(real(front_e),'k.-'); hold on; plot(imag(front_e),'m.-'); hold off; ylabel('err'); title('Front');
     subplot(3,2,2); plot(real(back_g),'b.-'); hold on; plot(real(back_a),'r.--'); hold off; ylabel('real'); title('Back');
     subplot(3,2,4); plot(imag(back_g),'b.-'); hold on; plot(imag(back_a),'r.--'); hold off; ylabel('imag'); title('Back');
     subplot(3,2,6); plot(real(back_e),'k.-'); hold on; plot(imag(back_e),'m.-'); hold off; ylabel('err'); title('Back');
   end
   front_m = max([abs(real(front_e)),abs(imag(front_e))]);
   back_m  = max([abs(real(back_e)),abs(imag(back_e))]);
   max_err = max(front_m,back_m);
   fprintf(1,'Max Err: %g\n',max_err);
   level = 2^16;
   if ( max_err <= level )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
