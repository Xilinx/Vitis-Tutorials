%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   addpath('../../tools/matlab');
   if     (nargin == 0) x86 = 1; settings = [16,24];
   elseif (nargin == 1)          settings = [16,24];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   close all;
   Nsamp      = settings(1);
   Ntrials    = settings(2);

   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   act_o = load_aiesim(sprintf('%s/data/sig_o.txt',base),'float',0);
   gld_o = load_aiesim('data/sig_o.txt','float',0);
   err_o = act_o - gld_o;
   plot(gld_o,'b.-'); hold on; plot(act_o,'r.--'); hold off;
   if (max(abs(err_o)) < eps(single(1)))
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
 end
 