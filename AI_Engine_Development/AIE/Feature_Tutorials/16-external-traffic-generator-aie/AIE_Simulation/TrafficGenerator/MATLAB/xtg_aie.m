% Users need to import plio classes from the released aie_stub.
vivado =  getenv("XILINX_VIVADO");
libPath = fullfile(vivado, "/data/emulation/matlab/xtlm_ipc");
addpath(libPath)

in_classifier_data = gen_data("./data/classifier_in.txt", 1024);
in_interpolator_data = gen_data("./data/interpolator_in.txt", 512);

% Call main function
[out_classifier_data, out_interpolator_data] = aie_func(in_classifier_data, in_interpolator_data);

% Consume generated data
dump_data(out_classifier_data, "out_classifier_data.txt");
dump_data(out_interpolator_data, "out_interpolator_data.txt");


function [out_classifier_data, out_interpolator_data] = aie_func(in_classifier_data, in_interpolator_data)
    % create in/out port objects for interpolator
    in_interpolator = aie_input_plio("in_interpolator", 'int16');
    out_interpolator = aie_output_plio("out_interpolator", 'int16');

    % create in/out port objects for the classifier
    in_classifier = aie_input_plio("in_classifier",'int16');
    out_classifier = aie_output_plio("out_classifier", 'int32');
		
	a = 0;
	b = 0;
	out_classifier_data = [];
	out_interpolator_data = [];
    % prepare logic to send the data 
    for i = 1:4
        % sending data values to the interpolator 
        % send_data() API call
        for j = a+1:256*i
            data = int16(in_classifier_data{j});
		    in_classifier.send_data(data,true);
        end

        % receive the data at the output port 
        out_classifier_data = [out_classifier_data ,out_classifier.receive_data_with_size(1024)];

        % sending data values to the interpolator 
        % send_data() API call
		for j = b+1:128*i
            data = int16(in_interpolator_data{j});
		    in_interpolator.send_data(data, true);
		end
		a = 256*i;
		b = 128*i;
        % receive the data at the output port 
        out_interpolator_data = [out_interpolator_data, out_interpolator.receive_data_with_size(1024)];
    end

end


function data_in = gen_data(filename,lines)
    data_in = cell(1,lines);
    fid = fopen(filename, 'r'); % Open the file for reading
    if fid == -1
        disp("ERROR cannot open file")
    end
    i = 1;
    while ~feof(fid)
        line = fgetl(fid); % Read a line from the file
        x = sscanf(line, '%d %d'); % Read integers from the line
        data_in{i} = x.';
        i = i+1;
    end
    fclose(fid); % Close the file
end


% Dumping the received data to the output file 
% which can be compared with golden file.
function dump_data(data, recv_file)
    j = 1;
    fid = fopen(recv_file, 'w'); % Open the file for writing    
    while j <= length(data)
        fprintf(fid, '%hd\n', data(j)); % Write data to the file
        j = j + 1;
    end
    fclose(fid); % Close the file
end

