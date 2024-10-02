%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

function data = computeNoiseSS (data, sysCfg, musiCfg)
 
%% Apply Jacobi (oneSided) SVD on the upper the square triangular
% matrix R of order nUla x nUla
if musiCfg.svdRotModel ==1
    [data] = svdOneSidedJacobi(data, @jacobiRotation_v1, sysCfg, musiCfg);
else
    [data] = svdOneSidedJacobi(data, @jacobiRotation_v2, sysCfg, musiCfg);
end


%% ---- SORT
[data] = sortSingularValues(data, sysCfg);

%% Determine the noise Noise sub-space
p = thresholding (data.s, sysCfg, musiCfg);
V = data.V;
noiseSS          = V(:, p:end);
noiseSS          = noiseSS(:, randperm(size(noiseSS, 2)));

data.noiseSS     = noiseSS;
data.sigSsDim    = p-1;

end



%% ------------------------------------------------------------
% Sorting the Singular values and corresponding V matrix
% ------------------------------------------------------------
function [dataOut] = sortSingularValues (dataIn, sysCfg)
[s, cIdx] = sort(dataIn.s, 'descend');
rIdx = 1:1:sysCfg.nUla; 

dataOut   = dataIn;
dataOut.s = s;
dataOut.V = dataIn.V(rIdx, cIdx);
end 


%% ------------------------------------------------------------
% Thresholding
% ------------------------------------------------------------
function p = thresholding (s, sysCfg, musiCfg)

switch musiCfg.noiseSsCutmeth
    
    case 1
         noiseFloor = musiCfg.noiseFloor;        % signal SS/noise SS cut .. in ]0 1[
         mask       = (s>noiseFloor);        % Vector operation
         p          = max(find((mask==1)))+1;
    case 2
         p = sysCfg.nSource+1;
    otherwise
        disp('Noise SS Cut Method Valid entrees are 1: Use noiseFloor or 2: Use Nbr of Sources')
end



end