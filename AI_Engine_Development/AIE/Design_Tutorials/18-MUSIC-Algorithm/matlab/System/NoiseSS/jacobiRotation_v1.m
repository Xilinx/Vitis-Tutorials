% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: AMD, comments by Bachir Berkane, Fidus Systems

% ------------------------------------------------------------
function Rot = jacobiRotation_v1( Xv, Yv )
   % Compute cos and sin of angle P, given vector Xv and Yv
   % vectors Xv and Yv are columns of the U from previous iteration
   % initially U is set to A
   d      = Xv'*Xv - Yv'*Yv;  % real number (distance)
   z      = Xv'*Yv;           % complex number; rotation of Yv by arg(conj(Xv))
   
   % relation of angle 2P w.r.t. Xv and Yv ??
   %

   % The rest is High school geometry 
   %  \
   % | \
   % |  \ 
   % |   \
   % |2|z|\  ==> cos2P = d/sqrt((2|Z|}^2+d^2) 
   % |     \ 
   % |    2P\
   % ---d----\
   
   % Derive Cos(2*P)
   zHadamard   = conj(z).* z;
   zNorm2      = sum(zHadamard);   
   t           = 1/sqrt(d^2+4*zNorm2);    
   cos2P       = d * t;                  

   % Derive cos(P) and sin(P) from  cos(2*P).. straightforward
   cosP   =  sqrt(0.5 * (1+cos2P)); % cos(2*P) = cos2(P)-sin2(P) 
                                    %          = cos2(P)-(1-cos2(P))
                                    %          = 2cos2(P)-1)
                                    % ==> cos(P) = sqrt (0.5* (cos(2*P) +1))
                                     
   sinP   = -sqrt(0.5 * (1-cos2P)); % cos(2*P) = cos2(P)-sin2(P) 
                                    %          = (1-sin2(P) -sin2(P)
                                    %          = 1- 2sin2(P) 
                                    % ==> sin(P) = sqrt (0.5* (1-cos(2*P))         

   % Form rotation transformation by angle P clockwise
   % RotbyP  = [cosP,     sinP;
   %            -sinP,    cosP];

   % Form unit vector in the direction of of conj(z)
   zInvNorm  = 1/sqrt(zNorm2);  % inverse square root
   unitVec   = conj(z) * zInvNorm;

   % form linear transformation to rotate the 2nd vector by arg(unitVec)
   % angle B = arg(unnitVec); that is arg(e^j(-B)); cos(-B)= real(z)/zNorm
   %                                                sin(-B)= imag(z)/zNorm
   % RotbyB = [1, 0;         
   %           0, unitVec]; 
   
   % Combine RotbyB and RotByP
   % Rot = RotbyB * RotbyP;

   Rot  = [cosP,                sinP;
           (unitVec)*-sinP,     (unitVec)*cosP];
    
end

