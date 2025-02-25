%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [16,8,4,2];
   elseif (nargin == 1)          settings = [16,8,4,2];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   addpath('../../matlab');
   R          = settings(1);
   C          = settings(2);
   Ntrials    = settings(3);
   Nports     = settings(4);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   act_o = zeros(Nports,R*C*Ntrials/Nports);
   gld_o = zeros(Nports,R*C*Ntrials/Nports);
   for pp = 1 : Nports
     act_o(pp,:) = reshape(load_aiesim(sprintf('%s/data/sig_%d_o.txt',base,pp-1),'int',0),1,[]);
     gld_o(pp,:) = reshape(load_aiesim(sprintf('data/sig_%d_o.txt',pp-1),'int',0),1,[]);
   end
   act_o = reshape(act_o,1,[]);
   gld_o = reshape(gld_o,1,[]);
   err_o = act_o - gld_o;
   figure;
   subplot(2,1,1); plot(gld_o,'b.-'); hold on; plot(act_o,'r.--'); hold off;
   subplot(2,1,2); plot(err_o,'k.-');
   
   fprintf(1,'Max err: %d\n',max(abs(err_o)));
   if (max(abs(err_o)) < 4)
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end

   