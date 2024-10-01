%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

% Compares output file generated during x86 simulation with testvector
% files and reports discrepancies.

slack = 1;    % floating point values might not match exactly
flush_subnorm = true; % sets subnormal values to zero

% get number of testvectors
config_txt = fileread('../aie/src/config.h');
txt_cells = regexp(config_txt, '\n', 'split');
line_id = ~cellfun(@isempty, regexp(txt_cells, 'NRUN'));
nvec_line = split(txt_cells(line_id));
num_vec = str2double(string(nvec_line(3)));

% get test vector data
tv_file = sprintf('../aie/data/smax_output_ref.txt');
fid = fopen(tv_file,'r');
tv_data = fscanf(fid,'%d');
fclose(fid);
tv_data = reshape(tv_data,length(tv_data)/num_vec,num_vec);
if flush_subnorm
    I = abs(tv_data) < typecast(realmin('single'), 'int32');
    tv_data(I) = 0.0;
end

% get simulator output data
sim_file = sprintf('../aie/x86simulator_output/data/smax_output_aie.txt');
fid = fopen(sim_file,'r');
sim_data = fscanf(fid,'%d');
fclose(fid);
sim_data = reshape(sim_data,length(sim_data)/num_vec,num_vec);
if flush_subnorm
    I = abs(sim_data) < typecast(realmin('single'), 'int32');
    sim_data(I) = 0.0;
end

% compare the test vectors
for idx = 1:num_vec
    [max_diff, I] = max(abs(sim_data(:,idx)-tv_data(:,idx)));
    if max_diff <= slack
        fprintf('MATCH: Test vector %d\n',idx);
    else
        fprintf('MISMATCH: Test vector %d, index = %d, sim = %s, tv = %s, diff = %d\n', ...
            idx, I, dec2hex(int32(sim_data(I,idx)),8), dec2hex(int32(tv_data(I,idx)),8), max_diff);
    end
end

% compare to gold reference
gld_file = sprintf('../aie/data/smax_gold_ref.txt');
fid = fopen(gld_file,'r');
gld_data = fscanf(fid,'%f');
fclose(fid);
gld_data = reshape(gld_data,length(gld_data)/num_vec,num_vec);

fprintf('\nComparing to gold reference (double precision)\n')
for idx = 1:num_vec
    sim_data_fp = double(typecast(int32(sim_data(:,idx)),'single'));
    [max_diff, I] = max(abs(gld_data(:,idx) - sim_data_fp));
    fprintf('Vec %d: softmax sum = %0.9e, max_diff = %0.9e, smax(fp32) = %0.9e, smax(gold) = %0.9e\n', ...
        idx, sum(sim_data_fp), max_diff, sim_data_fp(I), gld_data(I,idx));
end
