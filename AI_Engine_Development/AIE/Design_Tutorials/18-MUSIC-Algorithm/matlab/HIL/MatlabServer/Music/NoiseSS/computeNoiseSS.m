%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

function data = computeNoiseSS (data, cfg)
 
%% Apply Jacobi (oneSided) SVD on the upper the square triangular
% matrix R of order nUla x nUla
%cfg.svdRotModel =2;
if cfg.svdRotModel ==2
    [data] = svdOneSidedJacobi(data, @jacobiRotation_v1, cfg);
else
    [data] = svdOneSidedJacobi(data, @jacobiRotation_v2, cfg);
end


%% ---- SORT
[data] = sortSingularValues(data, cfg);

%% Determine the noise Noise sub-space
p = thresholding (data.s, cfg);
V = data.V;
noiseSS          = V(:, p:end);
noiseSS          = noiseSS(:, randperm(size(noiseSS, 2)));

data.noiseSS     = noiseSS;
data.sigSsDim    = p-1;

end



%% ------------------------------------------------------------
% Sorting the Singular values and corresponding V matrix
% ------------------------------------------------------------
function [dataOut] = sortSingularValues (dataIn, cfg)
[s, cIdx] = sort(dataIn.s, 'descend');
rIdx = 1:1:cfg.nUla; 

dataOut   = dataIn;
dataOut.s = s;
dataOut.V = dataIn.V(rIdx, cIdx);
end 


%% ------------------------------------------------------------
% Thresholding
% ------------------------------------------------------------
function p = thresholding (s, cfg)

switch cfg.noiseSsCutmeth
    
    case 1
         noiseFloor = cfg.noiseFloor;        % signal SS/noise SS cut .. in ]0 1[
         mask       = (s>noiseFloor);        % Vector operation
         p          = max(find((mask==1)))+1;
    case 2
         p = cfg.nSource+1;
    otherwise
        disp('Noise SS Cut Method Valid entrees are 1: Use noiseFloor or 2: Use Nbr of Sources')
end



end