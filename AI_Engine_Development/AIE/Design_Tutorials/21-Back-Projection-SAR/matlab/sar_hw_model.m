%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [im_o] = sar_hw_model( Npulse_u, range_i, ifft_i, coord_i, R0_range_i )
   Npulse        = 586;
   Nifft         = 2048;
   Nsize         = 512;
   c_light       = single(299792458);
   deltaF        = single(1.471301625e+06);
   Fmin          = single(9.288080384e+09);
   maxWr         = single(c_light/(2*deltaF));
   scale_theta   = single(2*Fmin/c_light);
   scale_domain  = single(1.0/maxWr);            % Scale 'dR' to produce 'domain'
   sqrt_compress = single(1.025640994e-08);      % 1.0/(97.5e6)
   sqrt_expand   = single(9.874208984e+03);      % sqrt(97.5e6)
   
   coord_i = reshape(coord_i,Npulse,3);
   tmp     = reshape(range_i,8,[]);
   x_mat   = reshape(tmp(:,1:3:end),1,[]);
   y_mat   = reshape(tmp(:,2:3:end),1,[]);
   z_mat   = reshape(tmp(:,3:3:end),1,[]);
   im_o    = complex(single(zeros(1,Nsize*Nsize)));
   r_vec   = 0.5+single(linspace(-Nifft/2,Nifft/2-1,Nifft))/Nifft;
   for pp = 1 : Npulse_u
     rc       = fftshift(ifft(ifft_i(:,pp)));
     dist_sq  = (coord_i(pp,1)-x_mat).^2 + ...
                (coord_i(pp,2)-y_mat).^2 + ...
                (coord_i(pp,3)-z_mat).^2;
     sqrt_o   = sqrt_expand*sqrt(sqrt_compress*dist_sq);
     dR       = sqrt_o - R0_range_i(pp);
     fmod_i   = scale_theta*dR;
     fmod_o   = mod(fmod_i,1);
     phCorr   = complex(cos(2*pi*fmod_o),sin(2*pi*fmod_o));
     dRn      = 0.5 + scale_domain*dR;
     distpart = interp1(r_vec,rc,dRn,'linear',0);
     im_o     = im_o + distpart .* phCorr;
   end
end
