
% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: AMD  

% ------------------------------------------------------------
% Jacobi Rotation : 
% James Demmel et al. Jacobi's Method is More Accurate than QR. SIAM J. 
% Matrix Anal. Appl., 1989}, volume 13,  pages: 1204-1245, 
% https://api.semanticscholar.org/CorpusID:16486687

% Version 2 of jacobi rotation ... Derivation by AMD
% ------------------------------------------------------------
function [Rot] = jacobiRotation_v2( Xv, Yv )

   % Vector operations .. Compute Hpp, Hqq and tmp
   HppHadamard = real (Xv.*conj(Xv)); % Casted to real .. to fix a weird matlab failure
   Hpp         = (sum(HppHadamard));  
 
   HqqHadamard = real (Yv.*conj(Yv));
   Hqq         = (sum(HqqHadamard)); % Real
   
   tmpHadamard = (Xv.*conj(Yv));
   tmp         = sum(tmpHadamard); % complex 

   Hrr = real(tmp);  Hjj = imag(tmp);

   ei_2t1 = calc_ei_2t(Hjj,Hrr);
   ei_t1  = calc_ei_t(imag(ei_2t1),real(ei_2t1));
   
   tx = 0.5*(Hqq-Hpp);
   ty = Hrr * imag(ei_2t1) + Hjj * real(ei_2t1);
   
   ei_2t2 = calc_ei_2t(tx,ty);
   ei_t2  = calc_ei_t(imag(ei_2t2), real(ei_2t2));
   
   Rot = calc_rot( ei_t1, real(ei_t2), imag(ei_t2) );
end

function [res] = calc_ei_2t(x,y)
   R = sqrt(x^2+y^2);
   res = complex(x/R,y/R);
end

function [res] = calc_ei_t(sin_2t,cos_2t)
   R = sqrt((1+cos_2t)^2 + sin_2t^2);
   res = complex((1+cos_2t)/R, sin_2t/R);
end

function [Rot] = calc_rot( eit, c, s )
   Rot = [      eit *complex(0,s),       eit*complex(c,0);
           conj(eit)*complex(-c,0), conj(eit)*complex(0,-s) ];
end