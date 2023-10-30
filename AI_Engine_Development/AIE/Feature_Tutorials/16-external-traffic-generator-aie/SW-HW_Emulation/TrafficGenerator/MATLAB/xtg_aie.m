% Users need to import plio classes from the released aie_stub.
vivado = getenv("XILINX_VIVADO");
libPath = fullfile(vivado,"data/emulation/matlab/xtlm_ipc");
addpath(libPath)

% data_gen function
% user logic to generate data for the AIE kernel interpolator 
% data_gen() is user logic to read the values from a file and return list of values
% user logic varies based on required dataset generated on the fly or from a file
% (optional) generate random list as well for the choice of dataset
% reading input data from the existing input file
data_in = data_gen("./data/interpolator_in.txt",512);

% main function
% Instantiate the AIE PLIO ports 
% to send/receive data from external traffic generator
classifier_data = aie_func(data_in);

% Dump the received data to the output file
dump_data(classifier_data);

function classifier_data = aie_func(data_in)
    % instantiate input PLIO of the interpolator
    in_interpolator = aie_input_plio("in_interpolator", 'int16');

    % instantiate output PLIO of the classifier
    out_classifier = aie_output_plio("out_classifier", 'int32');

    classifier_data = zeros(1,1024,'int32');
    index = 0;
    dataCount = 0;
		b = 0;
    % prepare logic to send the data 
    for i = 1:4
        % sending data values to the interpolator 
        % using send_data() API call
				for j = b+1:128*i
	   % user logic to generate data for the AIE kernel interpolator 
           % data values are read from file and 
           % stored in a list called in_interpolator_data 
           % data_gen() is user logic to read the values from a file and return list of values
	   % user logic can varry based on required dataset generated on the fly or from a file
	   % reading input data from the existing input file
            data = int16(data_in{j});
				% sending data values to the interpolator 
        			% send_data(data_val, tlast) API call
        			% 512 bytes are sent using send_data API call 
				% send_data() call 4 times
				% total 2048 bytes sent
		    		in_interpolator.send_data(data, true);
				end
				b = 128*i;
        
        % receive the data at the output port 
	% use receive_data() API call
	    % receive data call 4 times
	    % each iteration 1024 bytes are received
	    % expected total 4096 bytes to be received
        classifier_buf = out_classifier.receive_data_with_size(1024);
        numSamples = numel(classifier_buf);
        classifier_data(dataCount+1 : dataCount + numSamples) = classifier_buf;
        dataCount = numSamples*i;
    end
end
        

% user logic to fetch required data values from any file into the list
function data_in = data_gen(filename,lines)
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

% dump the received data to the output file 
% the output file can be compared with golden file.
function dump_data(data)
    j = 1;
    fid = fopen('classifier_out.txt', 'w'); % Open the file for writing    
    while j <= length(data)
        fprintf(fid, '%d\n', data(j)); % Write data to the file
        j = j + 1;
    end
    fclose(fid); % Close the file
end

