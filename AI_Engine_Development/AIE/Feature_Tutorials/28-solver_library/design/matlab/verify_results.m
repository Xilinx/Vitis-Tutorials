%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved. Portions of this file consist of AI-generated content.
% SPDX-License-Identifier: MIT
%
% Author: Chandra Sekar Balakrishnan

function verify_results(sim_type)
% verify_results - Verifies simulation results against MATLAB golden reference
%
% Usage:
%   verify_results('x86')  - Verify x86simulator output
%   verify_results('aie')  - Verify aiesimulator output
%
% Arguments:
%   sim_type - String specifying simulator: 'x86' or 'aie'

if nargin < 1
    error('Usage: verify_results(''x86'') or verify_results(''aie'')');
end

if ~ismember(sim_type, {'x86', 'aie'})
    error('sim_type must be either ''x86'' or ''aie''');
end

N = 128;

% --- Read golden reference (one cfloat sample per line: real imag) ---
golden_file = '../data/G_x_output.txt';
if ~exist(golden_file, 'file')
    error('Golden reference file not found: %s\nRun "make data" first.', golden_file);
end

x_golden = read_cfloat_vector(golden_file, false);

if length(x_golden) ~= N
    error('Golden reference sample count mismatch: expected %d, got %d', N, length(x_golden));
end

% --- Read simulator output ---
if strcmp(sim_type, 'x86')
    output_file = '../x86simulator_output/data/x_output.txt';
    sim_name = 'x86simulator';
else
    output_file = '../aiesimulator_output/data/x_output.txt';
    sim_name = 'aiesimulator';
end

fprintf('Reading %s output...\n', sim_name);
if ~exist(output_file, 'file')
    error('Output file not found: %s\nRun "make %ssim" first.', output_file, sim_type);
end

strip_timestamps = strcmp(sim_type, 'aie');
x_sim = read_cfloat_vector(output_file, strip_timestamps);

if length(x_sim) ~= N
    error('Simulation output sample count mismatch: expected %d, got %d', N, length(x_sim));
end

% --- Compute error metrics (adapted from mpsk_channel_estimation.m) ---
err = x_golden - x_sim;

mse = mean(abs(err).^2);
normalized_mse = mse / mean(abs(x_golden).^2);

error_norm = norm(err);
signal_norm = norm(x_golden);
normalized_error = error_norm / signal_norm;

max_abs_error = max(abs(err));

% --- Display results ---
fprintf('\n========================================\n');
fprintf('Linear Equation Solver Verification\n');
fprintf('========================================\n');
fprintf('Simulator:    %s\n', sim_name);
fprintf('System size:  %dx%d (A) x %dx1 (x) = %dx1 (b)\n', N, N, N, N);
fprintf('Data type:    cfloat (complex single precision)\n');
fprintf('PLIO:         plio_64_bits (1 cfloat sample per line)\n');
fprintf('TP_DIM:       %d\n', N);
fprintf('TP_GRID_DIM:  1\n\n');

fprintf('=== Accuracy Verification ===\n');
fprintf('Error Metrics (x_golden vs x_sim):\n');
fprintf('  Mean Squared Error (MSE):                %.6e\n', mse);
fprintf('  Normalized MSE (MSE/signal_power):       %.6e\n', normalized_mse);
fprintf('  Error Norm ||err||:                      %.6e\n', error_norm);
fprintf('  Signal Norm ||x_golden||:                %.6e\n', signal_norm);
fprintf('  Normalized Error (||err||/||x_golden||): %.6e (%.4f%%)\n', normalized_error, normalized_error*100);
fprintf('  Maximum Absolute Error:                  %.6e\n', max_abs_error);

% Sample-by-sample comparison
fprintf('\n=== Sample-by-Sample Comparison ===\n');
fprintf('%-6s | %-28s | %-28s | %-14s\n', ...
    'Index', 'Golden (real, imag)', 'Sim (real, imag)', '|Error|');
fprintf('%s\n', repmat('-', 1, 88));

for k = 1:N
    fprintf('%4d   | %+13.6e %+13.6e | %+13.6e %+13.6e | %+13.6e\n', ...
        k, real(x_golden(k)), imag(x_golden(k)), ...
        real(x_sim(k)), imag(x_sim(k)), ...
        abs(err(k)));
end

% --- PASS/FAIL determination ---
THRESHOLD = 1e-4;
test_passed = max_abs_error < THRESHOLD;

fprintf('\nValidation (threshold: %e):\n', THRESHOLD);
if test_passed
    fprintf('  Solution accuracy check PASSED\n');
else
    fprintf('  Solution accuracy check FAILED\n');
end

% --- Performance, resource, and latency metrics (AIE only) ---
if strcmp(sim_type, 'aie')
    fprintf('\n=== Performance Metrics ===\n');

    throughput_csv = '../aiesimulator_output/throughput_summary.csv';
    if exist(throughput_csv, 'file')
        warning('off', 'MATLAB:table:ModifiedAndSavedVarnames');
        throughput_table = readtable(throughput_csv, 'VariableNamingRule', 'preserve');
        warning('on', 'MATLAB:table:ModifiedAndSavedVarnames');

        if height(throughput_table) > 0
            update_rate_khz = throughput_table{1, 'Update Rate (KHz)'};
            throughput_msps = throughput_table{1, 'Throughput (Msps)'};
            samples_per_iter = throughput_table{1, 'Samples per Iteration'};

            fprintf('Update Rate:     %.2f KHz\n', update_rate_khz);
            fprintf('Throughput:      %.2f Msps\n', throughput_msps);
            fprintf('Samples/Iter:    %d\n', samples_per_iter);
        else
            fprintf('No throughput metrics found in CSV\n');
        end
    else
        fprintf('Throughput metrics not available. Run: make extract-throughput\n');
    end

    fprintf('\n=== AIE Resource Utilization ===\n');
    resources_csv = '../Work/aie_resources.csv';
    if exist(resources_csv, 'file')
        resources = readtable(resources_csv);
        num_compute = resources.Value(strcmp(resources.Metric, 'Compute Tiles'));
        num_total = resources.Value(strcmp(resources.Metric, 'Total AIE Tiles'));
        num_input_plios = resources.Value(strcmp(resources.Metric, 'Input PLIOs'));
        num_output_plios = resources.Value(strcmp(resources.Metric, 'Output PLIOs'));

        fprintf('Compute tiles:   %d\n', num_compute);
        fprintf('Total AIE tiles: %d\n', num_total);
        fprintf('Input PLIOs:     %d\n', num_input_plios);
        fprintf('Output PLIOs:    %d\n', num_output_plios);
    else
        fprintf('Resource metrics not available. Run: make extract-resources\n');
    end

    fprintf('\n=== AIE Latency Metrics ===\n');
    latency_csv = '../aiesimulator_output/latency_summary.csv';
    if exist(latency_csv, 'file')
        warning('off', 'MATLAB:table:ModifiedAndSavedVarnames');
        latency_table = readtable(latency_csv, 'VariableNamingRule', 'preserve');
        warning('on', 'MATLAB:table:ModifiedAndSavedVarnames');

        if height(latency_table) > 0
            fprintf('\n%-50s | %-50s | %-12s\n', 'Output Port', 'Input Port', 'Last Latency (us)');
            fprintf('%s\n', repmat('-', 1, 120));
            for i = 1:height(latency_table)
                fprintf('%-50s | %-50s | %12.2f\n', ...
                    char(latency_table{i, 'Output Port'}), ...
                    char(latency_table{i, 'Input Port'}), ...
                    latency_table{i, 'Last Latency (us)'});
            end
        else
            fprintf('No latency metrics found in CSV\n');
        end
    else
        fprintf('Latency metrics not available. Run: make extract-latency\n');
    end
end

% --- Final PASS/FAIL ---
fprintf('\n========================================\n');
if test_passed
    fprintf('VERIFICATION PASSED\n');
else
    fprintf('VERIFICATION FAILED\n');
end
fprintf('========================================\n');

end


function x = read_cfloat_vector(filepath, strip_timestamps)
% read_cfloat_vector - Read a vector of cfloat samples from a text file
%   Each data line contains: real_part imag_part
%   For AIE simulator output, lines starting with 'T' are timestamps and are skipped.

    fid = fopen(filepath, 'r');
    if fid == -1
        error('Could not open file: %s', filepath);
    end

    vals_real = [];
    vals_imag = [];

    while ~feof(fid)
        line = fgetl(fid);
        if ~ischar(line) || isempty(strtrim(line))
            continue;
        end
        if strip_timestamps && line(1) == 'T'
            continue;
        end
        parts = sscanf(line, '%f');
        if length(parts) == 2
            vals_real(end+1) = parts(1); %#ok<AGROW>
            vals_imag(end+1) = parts(2); %#ok<AGROW>
        end
    end
    fclose(fid);

    x = complex(single(vals_real(:)), single(vals_imag(:)));
end
