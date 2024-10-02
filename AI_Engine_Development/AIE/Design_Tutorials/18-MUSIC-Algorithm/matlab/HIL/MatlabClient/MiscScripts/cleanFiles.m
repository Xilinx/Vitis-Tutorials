% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Author B. Berkane, Fidus Systems 
% Snapshot generation 

function cleanFiles (Dir, fileExt, msg)

ext = strcat('*.', fileExt);
if ~isfolder(Dir), mkdir(Dir); end
filePattern = fullfile(Dir, '*.bin');
sFiles = dir(filePattern);

for sFile = 1 : length(sFiles)
    baseFileName = sFiles(sFile).name;
    fullFileName = fullfile(Dir, baseFileName);
    fprintf(1, strcat(msg, ' %s\n'), fullFileName);
    delete(fullFileName);
end

end