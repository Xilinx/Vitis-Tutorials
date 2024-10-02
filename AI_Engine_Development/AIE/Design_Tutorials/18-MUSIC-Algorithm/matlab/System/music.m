% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems


% Runs Music algorithm and generate test vectors for AIE sims. 
%  Algorithm accepts snapshot matric, and consists of:
    % - QRD (A) according to GSSR: Return the upper traingular Matrix R
    % - Identify NoiseSS: 
    %    - runs Jacobi SVD (1-sided) on R from QRD, and returns singular 
    %      values vector, and the right orthogonal matrix V  
    %    - Runs thresholding on s vectors to identify the noise Sub-space
    % - Compute DOAs: Sweep over the configured sweep-span @ the defined 
    %   granularity:
    %    - Construct signal beam steered into 1 direction,
    %    - compute the pseudo spectrum as the inverse of the product 
    %      of the Noise sub-space and signal.
    %    - Returns both the pseudo spectrum (Pm_peak) and its inverse (Pm_null)


function data = music (A, cfg)
 
%% Music Algorithm 

%-- QRD the Snaphot matrix A
dataRow = gssrRowModel(A);
dataCol = gssrColModel(A);

% Check if Row = Rcol 
if (~isequal(dataRow.R, dataCol.R) )  
fprintf('Rrow does NOT match Rcol!');
end
  
% Select the configured Model output
switch cfg.gssrModel
    case   'row' 
        data = dataRow;
    case   'col'
        data = dataCol;
    otherwise
        disp ('gssr Model not supported \n');
end

% Compare reconstructed matrix A error, vs reconstructed matrix using 
% Mathwork Rm with [Qm Rm] = qr(A)  
data.A   = A;
[Qm, Rm] = qr(A);
Qm = Qm(:, 1:cfg.nUla); 
Rm = Rm (1:cfg.nUla, :);
qrErrMtx   = abs(Qm*Rm - A);
gssrErrMtx = abs(data.Q*data.R - A);
 
fprintf('\n<strong>- Note:</strong> Matlab QR(A)  [Q*R vs A]: max(abs(Q*R - A)(:)) = %e\t \n', max(qrErrMtx(:)));
fprintf('\n<strong>- Note:</strong> GSSR QR(A)    [Q*R vs A]: max(abs(Q*R - A)(:)) = %e \n', max(gssrErrMtx(:)));

   
%% -- SVD the upper square triangular matrix R of order nUla x nUla
%  -- Extract and sort the singular values vector
%  -- Determine the noise sub-space matrix using the noiseFloor threshold
[data] = computeNoiseSS (data, cfg);
fprintf('\n<strong>- Note:</strong> Noise Subspace Size = %d \n', cfg.nUla-data.sigSsDim);

% Check consistency of results 
reconstructedR    = data.U*sqrt(data.S)*data.V';  
[~, Sm, ~]        = svd (data.R);
oneSidedJacobiErr = abs(reconstructedR - data.R);
vsMatSvd          = abs(Sm.^2 - data.S);

fprintf('\n<strong>- Note:</strong> One-Sided Jacobi: Reconstructed Matrix Maximum Error = %e \n', max(oneSidedJacobiErr(:)));
fprintf('\n<strong>- Note:</strong> Singular Values: Matlab-svd vs. OneSidedJacobi Maximum Deviation = %e \n', max(vsMatSvd(:)));

%% -- Compute spatial spectrum over number of points in the interval defined in cfg
%  -- Return both the spatial spectrum data.Pm_peak (source signals 
%     identified by peaks in spectrum) and its corresponing inverse 
%     data.Pm_null (source signals identified by nulls in spectrum)
[data] = computeDoa(data, cfg);

end
