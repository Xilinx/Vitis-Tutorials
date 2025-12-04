%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function check_sim_output()
    %% Read golden output
    fileID = fopen('data/sig_o.txt','r');
    formatSpec = '%f %f %f %f';
    [sig_g] = fscanf(fileID,formatSpec).';
    fclose(fileID);
    sig_g = [sig_g(1:4:end);sig_g(2:4:end);sig_g(3:4:end);sig_g(4:4:end)];
    sig_g = sig_g(:).';

    fileID = fopen('x86simulator_output/data/sig_o.txt','r');
    formatSpec = '%f %f %f %f';
    [sig_o_sim] = fscanf(fileID,formatSpec).';
    fclose(fileID);
    sig_o_sim = [sig_o_sim(1:4:end);sig_o_sim(2:4:end);sig_o_sim(3:4:end);sig_o_sim(4:4:end)];
    sig_o_sim = sig_o_sim(:).';
    fprintf('Max error = %f\n',max(abs(sig_g-sig_o_sim)));
end