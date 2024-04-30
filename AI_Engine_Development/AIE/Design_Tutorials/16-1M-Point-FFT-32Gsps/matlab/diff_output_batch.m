%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%---------------------------------
% diff output
%---------------------------------
function diff_output_batch(file_name_vec, listall)

 if ~exist('file_name_vec', 'var')
     file_name_vec = {'fft_x2'};
 end

if ~exist('listall', 'var')
     listall = 1;
end

num_file_names = numel(file_name_vec);

diffdb = zeros(num_file_names, 64);

for fidx = 1:num_file_names

	file_name = file_name_vec{fidx};

	for k=1:32

		ref_file = sprintf('../aie_src/data/%s_%02da.txt',     file_name, k-1);
		dut_file = sprintf('../aie_src/data/%s_%02da_dut.txt', file_name, k-1);
		
		diffdb(fidx, k*2-1) = diff_one_file(ref_file, dut_file);

		ref_file = sprintf('../aie_src/data/%s_%02db.txt',    file_name, k-1);
		dut_file = sprintf('../aie_src/data/%s_%02db_dut.txt',file_name, k-1);
		
		diffdb(fidx, k*2)  = diff_one_file(ref_file, dut_file);

	end
	
		
	if listall

		fprintf(1, '\n--------------------------------------\n');
		fprintf(1, '--         %s\n', file_name);
		fprintf(1, '--------------------------------------\n');
		for k=1:32
			fprintf("[%2d] %4.3f\t%4.3f\n", k-1, diffdb(fidx, k*2-1), diffdb(fidx, k*2));
		end
		fprintf(1, '--------------------------------------\n');
	
	end
	
	
	fprintf(1, '\n%s - Max Err = %.3f dBc\n\n', file_name, max(diffdb(fidx,:)));

end





function diff_db = diff_one_file(ref_file, dut_file)

    if ~exist(ref_file, 'file')
        error('Ref file %s does not exist.', ref_file);
    end
    
    if ~exist(dut_file, 'file')
        error('DUT file %s does not exist.', dut_file);
    end

    %------- load the files ---------------
    ref = load(ref_file); ref = ref.';
    dut = load(dut_file); dut = dut.';

    len_ref = numel(ref);
    len_dut = numel(dut);

    if len_ref~=len_dut
        fprintf(1, 'Length Mismatch\n', len_ref);
        fprintf(1, '\tRef file has %d samples.\n', len_ref);
        fprintf(1, '\tDUT file has %d samples.\n', len_dut);
        ref = ref(1:min(len_ref, len_dut));
        dut = dut(1:numel(ref));
    end

    dut = int2float(dut);

    diff_db = 10*log10( sum(abs(ref(:)-dut(:)).^2)./ sum(ref(:).^2) );
