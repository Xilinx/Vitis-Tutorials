%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

function check_sim_output(TARGET)
    NCHAN=4096;
    Niter=4;
    %% filterbank
    NPORT_O=4;
    for ii=1:NPORT_O
        % Read golden output
        file_name = sprintf('data/filterbank_o_%d.txt',ii-1);
        fileID = fopen(file_name,'r');
        formatSpec = '%f %f';
        [sig_g] = fscanf(fileID,formatSpec).';
        fclose(fileID);
        sig_g = [complex(sig_g(1:2:end),sig_g(2:2:end))];
        sig_g = sig_g(:).';
        
        if (TARGET=="hw")
            file_name = sprintf('aiesimulator_output/data/filterbank_o_%d.txt',ii-1);
        elseif (TARGET=="x86sim")
            file_name = sprintf('x86simulator_output/data/filterbank_o_%d.txt',ii-1);
        end
        fileID = fopen(file_name,'r');
        txt = textscan(fileID,'%s','delimiter','\n');
        fclose(fileID);

        tlast_count = size(find(strcmp(txt{1},'TLAST')),1); % Identify how many frames
        txt{1}(strcmp(txt{1},'TLAST'))=[];
        txt{1}(contains(txt{1},'T '))=[];
        tmp = [];
        for i=1:size(txt{1},1)
            x = strsplit(txt{1}{i},' ');
            if size(x,2)==3
                tmp   = [tmp,str2double(x{1}),str2double(x{2})];
            else
                tmp   = [tmp,str2double(x{1})]; % Last sample of every frame uses only half the PLIO width
            end
        end
        tmp  = reshape(tmp,[],tlast_count);
        tmp2 = zeros((size(tmp,1)-1),tlast_count/Niter,Niter); % Remove first sample of every frame since that is packet header
        packet_id_tmp = [];
        for iii=1:Niter
            for i=1:tlast_count/Niter
                packet_id = bitand(tmp(1,(iii-1)*tlast_count/Niter+i),hex2dec('0x00001F'));
                packet_id_tmp = [packet_id_tmp,packet_id];
                tmp2(:,packet_id+1,iii) = tmp(2:end,(iii-1)*tlast_count/Niter+i);
            end
        end
        tmp2 = reshape(tmp2,[],1);
        tmp2 = double(typecast(uint32(tmp2),'int32')); % Samples in the output are in uint32 format
        sig_o_sim = complex(tmp2(1:2:end),tmp2(2:2:end)).';
        fprintf('Filterbank output port %d max error LSB = %d\n',ii-1,max(max(real(sig_g-sig_o_sim)),max(imag(sig_g-sig_o_sim))));
    end

    %% FFT
    NPORT_O=8;
    for ii=1:NPORT_O
        % Read golden output
        file_name = sprintf('data/fft_back_o_%d.txt',ii-1);
        fileID = fopen(file_name,'r');
        formatSpec = '%f %f';
        [sig_g] = fscanf(fileID,formatSpec).';
        fclose(fileID);
        sig_g = [complex(sig_g(1:2:end),sig_g(2:2:end))];
        sig_g = sig_g(:).';
        
        if (TARGET=="hw")
            file_name = sprintf('aiesimulator_output/data/fft_back_o_%d.txt',ii-1);
        elseif (TARGET=="x86sim")
            file_name = sprintf('x86simulator_output/data/fft_back_o_%d.txt',ii-1);
        end
        fileID = fopen(file_name,'r');
        txt = textscan(fileID,'%s','delimiter','\n');
        fclose(fileID);

        txt{1}(strcmp(txt{1},'TLAST'))=[];
        txt{1}(contains(txt{1},'T '))=[];
        tmp = [];
        for i=1:size(txt{1},1)
            x = strsplit(txt{1}{i},' ');
            tmp   = [tmp,complex(str2double(x{1}),str2double(x{2}))];
        end
        sig_o_sim  = tmp;
        fprintf('FFT output port %d max error LSB = %d\n',ii-1,max(max(real(sig_g-sig_o_sim)),max(imag(sig_g-sig_o_sim))));
    end
end