%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [32,4096*4];
   elseif (nargin == 1)          settings = [32,4096*4];
   elseif (nargin ~= 2) error('Bad voodoo'); end

   addpath('../../scripts');
   close all;
   Nport      = settings(1);
   Nsamp      = settings(2);
   if (x86==0)  base = 'aiesimulator_output';
   else         base = 'x86simulator_output';
   end
   % Even transforms on ss0, odd transforms on ss1
   act_o = zeros(Nport,Nsamp/Nport);
   gld_o = zeros(Nport,Nsamp/Nport);
   for pp = 1 : Nport
     tmp = load_aiesim(sprintf('%s/data/filterbank_o_%d.txt',base,pp-1),'int32',1);
     act_o(pp,:) = tmp;
     tmp = load_aiesim(sprintf('data/filterbank_o_%d.txt',pp-1),'int32',1);
     gld_o(pp,:) = tmp;
   end
   act_o = reshape(act_o,1,[]);
   gld_o = reshape(gld_o,1,[]);
   err_o = act_o - gld_o;

   max_err = max([abs(real(err_o)),abs(imag(err_o))]);
   fprintf(1,'Max err: %g\n',max_err);

   if ( max(abs(err_o)) < 2 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end
