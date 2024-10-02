% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%
% Author: Mark Rollins ... 
% Author:Bachir Berkane for 1 tile annotations, fragmented macros into 
% quasi-atomic vector/scalar aie-mappable operations


% Input:  Snapshots matrix A of size = cfg.mSamp x cfg.nULA    
% Return only the square Matrix R of size  cfg.nUla x cfg.nULa  
 

%% This version calculate R on a row-basis 

function data = gssrRowModel(A)

   [~, nUla] = size(A);
   Q = single(A);
   R = single(zeros(nUla,nUla));
   
   
   for kk = 1 : nUla    % outer loop

       % R(kk,kk) = norm(Q(:,kk));
       % Q(:,kk)  = Q(:,kk) * 1.0/R(kk,kk);

       % ---  "Atomic" vector and non-linear scalar operations
       q1ColHadamard    = conj(Q(:,kk)).* Q(:,kk);
       qNormS           = sum(q1ColHadamard);   
       R(kk,kk)         = sqrt(qNormS); % non-linear scalar operation
       Q(:,kk)          = Q(:,kk) * 1.0/ R(kk,kk); % scalar operation on a nSamp-vector

       % % ---  alternative vector and non-linear scalar operations
       % % ---  Should be more performant than above
       % q1ColHadamard    = conj(Q(:,kk)).* Q(:,kk);
       % qNormS           = sum(q1ColHadamard);   
       % qInvNorm         = 1/sqrt(qNormS);  % non-linear scalar operation
       % R(kk,kk)         = qNormS  * qInvNorm; 
       % Q(:,kk)          = Q(:,kk) * qInvNorm;   

       for ii = kk+1 : nUla  %% inner  loop
           % R(kk,ii) = Q(:,kk)' * Q(:,ii);
           % Q(:,ii)  = Q(:,ii) - R(kk,ii) * Q(:,kk);

       % ---   "Atomic" vector operations          
       q2ColHadamard    = conj(Q(:,kk)).* Q(:,ii);
       R(kk,ii)         = sum(q2ColHadamard);
       tQ               = R(kk,ii) .* Q(:,kk);
       Q(:,ii)          = Q(:,ii) - tQ;

       end % end of inner loop

   end % end of outer loop

   data.Q = Q;
   data.R = R;
end

