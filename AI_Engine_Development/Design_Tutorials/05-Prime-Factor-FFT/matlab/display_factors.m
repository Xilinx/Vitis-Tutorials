%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function display_factors( range )
    for ii = 1 : numel(range)
      s = sprintf('%4d:  ',range(ii));
      f = sprintf('%4d ',factor(range(ii)));
      disp(sprintf('%s%s',s,f));
    end
end
