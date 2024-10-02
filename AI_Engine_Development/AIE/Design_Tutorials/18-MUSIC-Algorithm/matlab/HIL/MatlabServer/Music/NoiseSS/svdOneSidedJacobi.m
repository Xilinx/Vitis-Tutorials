% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark   Rollins (AMD):   Original model, 
%         Bachir Berkane (Fidus): Restructured code and added 
%                                 - Logging of final and intermediate data
%                                 - Scaling of the singular values vector
% ========================================================================         

function [data] = svdOneSidedJacobi(data, jacobiRotation, cfg)  
   % Perform one-sided Jacobi for Matrix data.R
   nUla         = cfg.nUla;
   V            = single(eye(nUla)); % identity matrix
   W            = single(data.R);
   nIter        = cfg.svdNumIter;


   for iter = 1 : nIter % iteration-Loop  x nIter 
       

       for p = 1 : nUla-1       % Outer-loop   x (nUla-1)
           %fprintf('p = %d \n', p);
       

           for q = p+1 : nUla   % Inner-loop   x (nUla-p)
              % fprintf('q = %d \n', q);
               Rot = jacobiRotation(W(:,p), W(:,q) );
               V(:,[p,q]) = V(:,[p,q]) * Rot;
               W(:,[p,q]) = W(:,[p,q]) * Rot;

           end
       end
   end

   % Compute sv vector s and Matrix S, and the left orthogonal matrix U:
   % s vector is the normalized diag(S) w.r.t. the max value.
   % s is sorted in descending order

   [U, S, s]  = computeSingularValues (W, cfg); 
   % Logging
   data.S = S; data.V = V; data.U = U; data.s =s;
end


% -------------------------
% Compute Singular values
% -------------------------
function [U, S, s]  = computeSingularValues (W, cfg)
 
  % Compute scaled singular values and the left orthogonal matrix U 
   nUla         = cfg.nUla;
   U = single(zeros(nUla,nUla)); 
   S = single(zeros(nUla,nUla));
   s = single(zeros(nUla,1));

   S(1,1)     = W(:,1)'*W(:,1);
   s1Norm     = 1.0/sqrt(S(1, 1));
   U(:,1)     = W(:,1) .* s1Norm;
   svScale    = 1.0/S(1,1); 
   
   for ii = 2 : nUla 
       S(ii, ii)    = W(:,ii)'*W(:,ii);      % vector operation 
       sNorm        = 1.0/sqrt(S(ii, ii));
       s(ii)        = S(ii, ii)*svScale;     % Scalar Operation 
 
       % Operations not required for MUSIC 
       U(:,ii)  = W(:,ii) .* sNorm;          % Normalize columns of U
   end
   s(1)     = 1;
end 

