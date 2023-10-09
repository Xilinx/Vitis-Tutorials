%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

% Compares output file generated during AIE simulation with testvector
% files and reports discrepancies.

% Retrieve the number of cores used
config_txt = fileread('../aie/src/config.h');
txt_cells = regexp(config_txt, '\n', 'split');
line_id = ~cellfun(@isempty, regexp(txt_cells, 'NCORE'));
ncore_line = split(txt_cells(line_id));
num_core = str2double(string(ncore_line(3)));

% get gold reference
load golden_ref.mat;
[ysz, xsz] = size(Imgld);
aie_sim_img = zeros(ysz*xsz, 1);
tv_img = zeros(ysz*xsz, 1);
max_idx = ysz*xsz - 1;
slack = 2;    % floating point values won't match exactly

% compare the files for each core
for idx = 1:num_core
    % get the test vector data
    tv_file = sprintf('../aie/data/output_%d_ref.txt',idx);
    fid = fopen(tv_file,'r');
    tv_data = int32(fscanf(fid,'%d'));
    fclose(fid);

    % find entries with index values out of range and delete
    pxl_idx = int32(tv_data(1:2:end));
    pxl_val = int32(tv_data(2:2:end)); 
    I = find(pxl_idx > max_idx);
    pxl_idx(I) = [];
    pxl_val(I) = [];
    tv_data = [pxl_idx pxl_val]';
    tv_data = tv_data(:);

    % place test_vector data into image
    pxl_idx = tv_data(1:2:end);
    interp_pxl = typecast(tv_data(2:2:end), 'single');
    tv_img(pxl_idx+1) = interp_pxl;

    % get simulator output data and remove time stamps
    sim_file = sprintf('../aie/aiesimulator_output/data/output_%d_aie.txt',idx);
    sim_file_txt = fileread(sim_file);
    sim_file_cells = regexp(sim_file_txt, '\n', 'split');
    pruned_cells = sim_file_cells(~contains(sim_file_cells,'T'));
    sim_data = str2double(string(strsplit(cell2mat(pruned_cells))))';

    % find entries with index values out of range and delete
    pxl_idx = int32(sim_data(1:2:end));
    pxl_val = int32(sim_data(2:2:end)); 
    if length(pxl_idx) > length(pxl_val)
        pxl_idx(end) = [];
    end
    I = find(pxl_idx > max_idx);
    pxl_idx(I) = [];
    pxl_val(I) = [];
    sim_data = [pxl_idx pxl_val]';
    sim_data = sim_data(:);

    % place sim data into image
    pxl_idx = sim_data(1:2:end);
    interp_pxl = typecast(sim_data(2:2:end), 'single');
    aie_sim_img(pxl_idx+1) = interp_pxl;

    % compare the test vectors
    max_diff_even = max(abs(sim_data(1:2:end)-tv_data(1:2:end)));
    max_diff_odd = max(abs(sim_data(2:2:end)-tv_data(2:2:end)));
    if max_diff_even == 0 && max_diff_odd <= slack
        fprintf('MATCH: Test vector %d\n', idx);
    else
        fprintf('MISMATCH: Test vector %d, index diff = %d, pixel diff = %d\n', idx, max_diff_even, max_diff_odd);
    end
end

aie_sim_img = reshape(aie_sim_img, ysz, xsz);
tv_img = reshape(tv_img, ysz, xsz);

% plot results
subplot(1,2,1);
imshow(aie_sim_img/255)
title('AIE Simulation')
subplot(1,2,2);
imshow(Imgld/255)
title('Golden Reference')

% calculate error compared to single precision test vectors
err = abs(aie_sim_img - tv_img);
max_err = max(err(:));
ms_err = mean(err(:).^2);
fprintf('\nComparison of simulation with single precision test vectors:\n');
fprintf('Maximum pixel error is %.9f\n', max_err)
fprintf('Mean square pixel error is %.9f\n', ms_err)

% calculate error compared to double precision gold reference
err = abs(aie_sim_img - Imgld);
max_err = max(err(:));
ms_err = mean(err(:).^2);
fprintf('\nComparison of simulation with double precision golden reference:\n');
fprintf('Maximum pixel error is %.9f\n', max_err)
fprintf('Mean square pixel error is %.9f\n', ms_err)