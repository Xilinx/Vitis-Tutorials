% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
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
intermDatlog = false;

%  Set to value out of range .. legacy parameters
cfg.svdIter2Log = 100;   % range = 1 .. cfg.nIter
cfg.svdPass2Log = 100;   % range = 1 .. to cfg.nUla  (Jacobi svd outer Loop)
%%

for iter = 1 : nIter % iteration-Loop  x nIter

    iter2Log = false;

    if iter == cfg.svdIter2Log
        iter2Log = true;
        intermDatlog = true; 
    end

    for p = 1 : nUla-1       % Outer-loop   x (nUla-1)
        %fprintf('p = %d \n', p);
        pass2Log = false;
        if p == cfg.svdPass2Log
            pass2Log = true; pass = strcat('pass', num2str(p)); end

        for q = p+1 : nUla   % Inner-loop   x (nUla-p)
            % fprintf('q = %d \n', q);
            Rot = jacobiRotation(W(:,p), W(:,q) );
            V(:,[p,q]) = V(:,[p,q]) * Rot;
            W(:,[p,q]) = W(:,[p,q]) * Rot;

            if (pass2Log && iter2Log == true)
                % log data for the configured iteration and pass
                s = strcat(pass,'_',num2str(q));
                intermData.(s).V    = V(:,[p,q]);
                intermData.(s).W    = W(:,[p,q]);
                intermData.(s).Rot  = Rot;
            end
        end
    end
end

% Compute sv vector s and Matrix S, and the left orthogonal matrix U:
% s vector is the normalized diag(S) w.r.t. the max value.
% s is sorted in descending order

[U, S, Sdiag, scaledSdiag]  = computeSingularValues (W, cfg);
 

% Logging
data.S = S;  data.Sdiag = Sdiag; data.scaledSdiag = scaledSdiag;
data.V = V; data.U = U;

if (intermDatlog == true)
    data.intermData = intermData;
end

end


% -------------------------
% Compute Singular values
% -------------------------
function [U, S, Sdiag, scaledSdiag]  = computeSingularValues (W, cfg)

% Compute scaled singular values and the left orthogonal matrix U
% added non scaled one as requested by Pfeing
nUla         = cfg.nUla;
U = single(zeros(nUla,nUla));
S = single(zeros(nUla,nUla));
scaledSdiag = single(zeros(nUla,1));
Sdiag = single(zeros(nUla,1));

S(1,1)     = W(:,1)'*W(:,1);
s1Norm     = 1.0/sqrt(S(1, 1));
U(:,1)     = W(:,1) .* s1Norm;
svScale    = 1.0/S(1,1);
Sdiag(1)   = S(1,1);


for ii = 2 : nUla
    S(ii, ii)    = W(:,ii)'*W(:,ii);      % vector operation
    sNorm        = 1.0/sqrt(S(ii, ii));
    scaledSdiag(ii)        = S(ii, ii)*svScale;     % Scalar Operation
    Sdiag(ii)  = S(ii,ii);
    % Operations not required for MUSIC
    U(:,ii)  = W(:,ii) .* sNorm;          % Normalize columns of U
end
scaledSdiag(1)     = 1;
end