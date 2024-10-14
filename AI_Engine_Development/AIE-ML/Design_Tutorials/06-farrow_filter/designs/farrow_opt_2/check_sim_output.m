%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function check_sim_output(TARGET)
    %% Read golden output
    fileID = fopen('../../data/sig_o.txt','r');
    formatSpec = '%f %f %f %f';
    [sig_g] = fscanf(fileID,formatSpec).';
    fclose(fileID);
    sig_g = [complex(sig_g(1:4:end),sig_g(2:4:end));complex(sig_g(3:4:end),sig_g(4:4:end))];
    sig_g = sig_g(:).';
    
    if (TARGET=="hw")
        fileID = fopen('aiesimulator_output/data/sig_o.txt','r');
        txt = textscan(fileID,'%s','delimiter','\n');
        fclose(fileID);
        txt{1}(strcmp(txt{1},'TLAST'))=[];
        tmp = zeros(1,size(txt{1},1));
        for i=2:2:size(txt{1},1)
            x = strsplit(txt{1}{i},' ');
            tmp(i-1) = complex(str2double(x{1}),str2double(x{2}));
            tmp(i) = complex(str2double(x{3}),str2double(x{4}));
        end
        sig_o_sim = tmp;

        y1 = strsplit(txt{1}{1},' ');
        if strcmp(y1(end),'ns') factor = 1; elseif strcmp(y1(end),'us') factor = 1000; elseif strcmp(y1(end),'ps') factor = 1/1000; end
        y1 = str2num(y1{2})*factor;
        y2 = strsplit(txt{1}{end-1},' ');
        if strcmp(y2(end),'ns') factor = 1; elseif strcmp(y2(end),'us') factor = 1000; elseif strcmp(y2(end),'ps') factor = 1/1000; end
        y2 = str2num(y2{2})*factor;
        throughput = round(size(sig_o_sim,2)/(y2-y1)*1000,1);
        fprintf('Raw Throughput = %.1f MSPS\n',throughput);
    elseif (TARGET=="x86sim")
        fileID = fopen('x86simulator_output/data/sig_o.txt','r');
        formatSpec = '%f %f %f %f';
        [sig_o_sim] = fscanf(fileID,formatSpec).';
        fclose(fileID);
        sig_o_sim = [complex(sig_o_sim(1:4:end),sig_o_sim(2:4:end));complex(sig_o_sim(3:4:end),sig_o_sim(4:4:end))];
        sig_o_sim = sig_o_sim(:).';
    end
    fprintf('Max error LSB = %d\n',max(max(real(sig_g-sig_o_sim)),max(imag(sig_g-sig_o_sim))));
end