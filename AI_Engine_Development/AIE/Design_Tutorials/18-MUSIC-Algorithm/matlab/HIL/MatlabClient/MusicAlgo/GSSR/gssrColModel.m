%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins & Bachir Berkane


% Input the snapshots matrix = mSamp x nULA    
% Return only square Matrix R = nUla x nULa  
% nSnapshots should set as parameters ... set to 128 x 16 for now
 

%% This version calculate R on a column-basis 

function data = gssrColModel(A)

   [~, nUla] = size(A);
   Q = single(A);
   R = single(zeros(nUla,nUla));

    for kk = 1 : nUla   % outer loop
       
        
      for ii = 1 : kk-1 % inner Loop  
       % R(ii,kk) = Q(:,ii)' * Q(:,kk);
       % Q(:,kk)  = Q(:,kk) - R(ii,kk) * Q(:,ii);

       % ---   Atomic vector operations          
       q2ColHadamard    = conj(Q(:,ii)).* Q(:,kk);
       R(ii,kk)         = sum(q2ColHadamard);
       tQ               = R(ii,kk) * Q(:,ii);
       Q(:,kk)          = Q(:,kk) - tQ;

      end

      % R(kk,kk) = norm(Q(:,kk));
      % Q(:,kk) = Q(:,kk) * 1.0/R(kk,kk);

      % ---   vector and non-linear scalar operations
      q1ColHadamard    = conj(Q(:,kk)).* Q(:,kk);
      qNormS           = sum(q1ColHadamard);   
      R(kk,kk)         = sqrt(qNormS); % non-linear scalar operations
      Q(:,kk)          = Q(:,kk) * 1.0/R(kk,kk) ; % scalar operation !

      % ---  alternative vector and non-linear scalar operations
      % q1ColHadamard    = conj(Q(:,kk)).* Q(:,kk);
      % qNormS           = sum(q1ColHadamard);   
      % qInvNorm         = 1/sqrt(qNormS);    % non-linear scalar operation
      % R(kk,kk)         = qNormS * qInvNorm; % 
      % Q(:,kk)          = Q(:,kk)* qInvNorm; % vector opertaion 
      

      % logging
      data.Q = Q;
      data.R = R;
    end
end

