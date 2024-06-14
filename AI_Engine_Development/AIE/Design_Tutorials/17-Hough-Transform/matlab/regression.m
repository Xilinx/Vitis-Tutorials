%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86,settings)
   if     (nargin == 0) x86 = 1; settings = [128,322,32]; 
   elseif (nargin == 1)          settings = [128,322,32];
   elseif (nargin ~= 2) error('regression(x86);'); end
   theta_res = settings(1);
   rho_max   = settings(2);
   NT        = settings(3);
   
   tmp = linspace(-90,90,1+theta_res); 
   theta = tmp(1:end-1);
   rho = [-rho_max:1:rho_max];
   
   if ( x86 == 1 ) folder = 'x86simulator_output';
   else            folder = 'aiesimulator_output';
   end

   act = [];
   gld = [];
   for tt = 1 : NT
     tmpA = load_aiesim(sprintf('../aie/%s/data/sig_o_%d.txt',folder,tt-1),'int',0);
     tmpG = load_aiesim(sprintf('../aie/data/sig_o_%d.txt',          tt-1),'int',0);
     act = cat(1,act,tmpA);
     gld = cat(1,gld,tmpG);
     disp(sprintf('Done file %d of %d',tt,NT));
   end
   extra = size(act,2)-size(gld,2);
   act = act(:,1:end-extra);
   err = act-gld;
   maxerr = max(abs(reshape(err,1,[])));

   figure;
   subplot(1,2,1);
   imshow(imadjust(rescale(transpose(gld))),'XData',theta,'YData',rho,'InitialMagnification','fit');
   title('Matlab'); xlabel('\theta'); ylabel('\rho');
   axis on, axis normal; colormap(gca,jet);

   subplot(1,2,2);
   imshow(imadjust(rescale(transpose(act))),'XData',theta,'YData',rho,'InitialMagnification','fit');
   title(sprintf('AIE (%d tiles)',NT)); xlabel('\theta'); ylabel('\rho');
   axis on, axis normal; colormap(gca,jet);
   
   fprintf(1,'Max err: %g\n',maxerr);

   if ( maxerr <= 1 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end

