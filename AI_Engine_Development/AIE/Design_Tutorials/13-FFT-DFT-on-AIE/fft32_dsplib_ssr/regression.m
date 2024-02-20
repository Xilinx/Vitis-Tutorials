%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86)
   addpath('../matlab');
   
   if (x86==0)
     act0_o = load_aiesim('aiesimulator_output/data/sig0_o.txt','int',1);
   else
     act0_o = load_aiesim('x86simulator_output/data/sig0_o.txt','int',1);
   end
   gld0_o = load_aiesim('data/sig0_o.txt','int',1);

   if (1)
     subplot(3,1,1); plot(real(gld0_o),'b.-'); hold on; plot(real(act0_o),'r.--'); hold off; ylabel('real');
     subplot(3,1,2); plot(imag(gld0_o),'b.-'); hold on; plot(imag(act0_o),'r.--'); hold off; ylabel('imag');
     subplot(3,1,3); plot(abs(act0_o-gld0_o(1:numel(act0_o))),'k.-'); ylabel('abs(err)');
% $$$      plot(abs(gld0_o),'b.-'); hold on; plot(abs(act0_o),'r.--'); hold off;
   end
   
   err = act0_o - gld0_o(1:numel(act0_o));
   max_err = max([abs(real(err)),abs(imag(err))]);
   fprintf(1,'Max Err: %g\n',max_err);
   if ( max_err <= 3 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
