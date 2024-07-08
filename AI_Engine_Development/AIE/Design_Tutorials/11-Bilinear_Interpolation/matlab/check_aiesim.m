%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
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
pxl_rem = max_idx + 1;
core_pxl_cnt = 0;
slack = 0;    % floating point values won't match exactly

% compare the files for each core
for idx = 1:num_core
    % get the test vector data
    tv_file = sprintf('../aie/data/output_%d_ref.txt',idx);
    fid = fopen(tv_file,'r');
    tv_data = int32(fscanf(fid,'%d'));
    fclose(fid);

    % get simulator output data and remove time stamps
    sim_file = sprintf('../aie/aiesimulator_output/data/output_%d_aie.txt',idx);
    sim_file_txt = fileread(sim_file);
    sim_file_cells = regexp(sim_file_txt, '\n', 'split');
    pruned_cells = sim_file_cells(~contains(sim_file_cells,'T'));
    sim_data = int32(str2double(string(strsplit(cell2mat(pruned_cells)))))';

    % check and removed extra pixels
    tv_data = tv_data(1:min(length(tv_data), pxl_rem));
    sim_data = sim_data(1:min(length(tv_data), pxl_rem));
    pxl_rem = pxl_rem - length(tv_data);

    % place data into image
    tv_img((core_pxl_cnt+1):(core_pxl_cnt+length(tv_data))) = typecast(tv_data, 'single');
    aie_sim_img((core_pxl_cnt+1):(core_pxl_cnt+length(sim_data))) = typecast(sim_data, 'single');
    core_pxl_cnt = core_pxl_cnt+ length(tv_data);

    % compare the test vectors
    max_diff = max(abs(sim_data-tv_data));
    if max_diff <= slack
        fprintf('MATCH: Test vector %d\n', idx);
    else
        fprintf('MISMATCH: Test vector %d, pixel diff = %d\n', idx, max_diff);
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