% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

function save2File (dat, fileName, format)

switch format

    case 'mtx' % complex matrix
        writematrix(dat, fileName, 'Delimiter','space');

    case  'col'  % column-wise I<space>Q
        datColWise = reshape(dat, 1, []).';
        if ~isreal(dat)
            IspaceQ    = [real(datColWise(:,1))  imag(datColWise(:,1))];
        else
            IspaceQ =datColWise;
        end
        writematrix(IspaceQ, fileName, 'Delimiter','space');

    case 'row'  % row-wise I<space>Q
        datRowWise = reshape(dat.', 1, []).';
        if ~isreal(dat)
            IspaceQ    = [real(datRowWise(:,1))  imag(datRowWise(:,1))];
        else
            [~, n] = size(dat);
            if n ~= 1
            IspaceQ =datRowWise;
            else
                IspaceQ =datRowWise.';
            end
        end

        writematrix(IspaceQ, fileName, 'Delimiter','space');

    otherwise
        disp('Logging Format not defined')

end