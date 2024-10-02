% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% MUSIC Algorithm
% Author: Bachir Berkane, Fidus Systems  
 

% Functions called :
%  -  
%  - 

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
  
%% -- SVD the upper square triangular matrix R of order nUla x nUla
%  -- Extract and sort the singular values vector
%  -- Determine the noise sub-space matrix using the noiseFloor threshold
[data] = computeNoiseSS (data, cfg);


%% -- Compute spatial spectrum over number of points in the interval defined in cfg
%  -- Return both the spatial spectrum data.Pm_peak (source signals 
%     identified by peaks in spectrum) and its corresponing inverse 
%     data.Pm_null (source signals identified by nulls in spectrum)
[data] = computeDoa(data, cfg);
doaAngles = ['\n          >> Estimated DOA Angles (Degrees) =  [', repmat('%g, ', 1, numel(data.doaEstDeg)-1), '%g]\n'];
fprintf(doaAngles,  data.doaEstDeg);

end
