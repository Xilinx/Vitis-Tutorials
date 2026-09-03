% 
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% Portions of this file consist of AI-generated content.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

%% M-PSK Communication System with Channel Estimation
% This script demonstrates channel estimation for an M-PSK communication
% system using least squares solution with Cholesky decomposition.
% All computations use single precision floating point.

clear all;
close all;
clc;

%% Configuration Parameters
% Adjustable parameters
M = 16;                % Modulation order (16-PSK by default)
Nsymbols = 4096;       % Number of transmitted symbols
SNR_dB = 30;          % Signal-to-noise ratio in dB
channel_taps = 32;    % Number of FIR channel taps
Niter = 8;            % Number of iterations for statistics
NO_OF_ITERATIONS = 4; % Number of times to repeat the same AIE stimulus dataset in output files

% Channel model parameters
passthru_tap_position = 12;  % Unity tap position in second quarter (9-16 for 32 taps)
num_dispersive_taps = 5;     % Number of random taps around pass-through (odd number recommended)
isi_power_fraction = 0.25;    % Fraction of total power in ISI taps (0 to 1, remaining in pass-through)

% Fixed parameters for reproducibility
rng(1);               % Fixed random seed (matches existing test data generation)

fprintf('M-PSK Channel Estimation Simulation\n');
fprintf('=====================================\n');
fprintf('Modulation Order (M): %d\n', M);
fprintf('Number of Symbols: %d\n', Nsymbols);
fprintf('SNR: %.1f dB\n', SNR_dB);
fprintf('Channel Taps: %d\n', channel_taps);
fprintf('Pass-through Tap Position: %d\n', passthru_tap_position);
fprintf('Dispersive Taps: %d\n', num_dispersive_taps);
fprintf('ISI Power Fraction: %.2f (Pass-through: %.2f)\n', isi_power_fraction, 1-isi_power_fraction);
fprintf('Number of Iterations: %d\n', Niter);
fprintf('AIE Stimulus Dataset Repetitions: %d\n', NO_OF_ITERATIONS);
fprintf('\n');

% Initialize arrays to store metrics across iterations
mse_all = zeros(Niter, 1, 'single');
normalized_mse_all = zeros(Niter, 1, 'single');
error_norm_all = zeros(Niter, 1, 'single');
signal_norm_all = zeros(Niter, 1, 'single');
normalized_error_all = zeros(Niter, 1, 'single');
max_abs_error_all = zeros(Niter, 1, 'single');
cond_number_all = zeros(Niter, 1, 'single');

%% Main Iteration Loop
for iter = 1:Niter
    if iter == 1
        fprintf('\n========== Iteration %d ==========\n', iter);
    else
        fprintf('\nIteration %d...', iter);
    end

    %% Step 1: Generate Random Data Bits
    bits_per_symbol = log2(M);
    num_bits = Nsymbols * bits_per_symbol;
    data_bits = randi([0, 1], num_bits, 1);
    
    if iter == 1
        fprintf('Generated %d random data bits\n', num_bits);
    end

    %% Step 2: M-PSK Modulation
    tx_symbols = mpsk_modulate(data_bits, M);
    
    if iter == 1
        fprintf('Modulated to %d M-PSK symbols (single precision)\n', length(tx_symbols));
    end

    %% Step 3: Generate Composite FIR Channel (Single Precision)
    % Initialize channel with zeros
    h_true = single(zeros(channel_taps, 1));
    
    % Compute tap indices centered around pass-through
    half_span = floor(num_dispersive_taps / 2);
    tap_start = max(1, passthru_tap_position - half_span);
    tap_end = min(channel_taps, passthru_tap_position + half_span);
    dispersive_indices = tap_start:tap_end;
    
    % Generate random complex ISI taps at dispersive positions (excluding pass-through)
    isi_indices = dispersive_indices(dispersive_indices ~= passthru_tap_position);
    for idx = isi_indices
        h_true(idx) = single((randn() + 1j*randn()) / sqrt(2));
    end
    
    % Normalize ISI taps to specified power fraction
    if ~isempty(isi_indices)
        current_isi_power = sum(abs(h_true(isi_indices)).^2);
        if current_isi_power > 0
            h_true(isi_indices) = h_true(isi_indices) * sqrt(single(isi_power_fraction) / current_isi_power);
        end
    end
    
    % Set pass-through tap to remaining power
    h_true(passthru_tap_position) = sqrt(single(1 - isi_power_fraction));
    
    if iter == 1
        fprintf('Generated composite %d-tap complex FIR channel (single precision)\n', channel_taps);
        fprintf('  Pass-through tap at position %d\n', passthru_tap_position);
        fprintf('  ISI taps from %d to %d (excluding position %d)\n', tap_start, tap_end, passthru_tap_position);
        fprintf('  Pass-through power: %.6f\n', abs(h_true(passthru_tap_position))^2);
        fprintf('  ISI power: %.6f\n', sum(abs(h_true(isi_indices)).^2));
        fprintf('  Total channel power: %.6f\n', sum(abs(h_true).^2));
    end

    %% Step 4: Apply FIR Channel
    % Convolve transmitted symbols with channel
    y_channel = filter(h_true, single(1), tx_symbols);
    
    if iter == 1
        fprintf('Applied FIR channel filtering\n');
    end

    %% Step 5: Add AWGN Noise (Single Precision)
    % Calculate signal power
    signal_power = single(mean(abs(y_channel).^2));
    
    % Compute noise variance from SNR
    noise_var = single(signal_power / (10^(SNR_dB/10)));
    
    % Generate complex Gaussian noise (single precision)
    noise = single(sqrt(noise_var/2)) * single(randn(size(y_channel)) + 1j*randn(size(y_channel)));
    
    % Add noise to channel output
    y_noisy = y_channel + noise;
    
    if iter == 1
        fprintf('Added AWGN noise (SNR = %.1f dB, noise variance = %.6f)\n', SNR_dB, noise_var);
    end

    %% Step 6: Construct Least Squares Problem
    % Remove initial transient samples (channel memory)
    transient_samples = channel_taps - 1;
    
    % Build convolution matrix H (Toeplitz structure)
    % Each row contains channel_taps consecutive transmitted symbols in REVERSE time order
    % For convolution: y[n] = h[1]*x[n] + h[2]*x[n-1] + ... + h[32]*x[n-31]
    % So H(i,:) should be [x[n], x[n-1], x[n-2], ..., x[n-31]]
    num_training = length(tx_symbols) - channel_taps + 1;
    H = single(zeros(num_training, channel_taps));
    
    for i = 1:num_training
        % Reverse order for proper convolution: x[n], x[n-1], ..., x[n-31]
        H(i, :) = tx_symbols(i+channel_taps-1:-1:i).';
    end
    
    % Corresponding observation vector (skip initial transient)
    y_obs = y_noisy(channel_taps:channel_taps+num_training-1);
    
    if iter == 1
        fprintf('Constructed convolution matrix H: %dx%d (single precision)\n', size(H, 1), size(H, 2));
        fprintf('Observation vector y: %dx1\n', length(y_obs));
    end

    %% Step 7: Form Normal Equations
    % H' * H matrix (channel_taps x channel_taps)
    HtH = H' * H;
    
    % H' * y vector (channel_taps x 1)
    Hty = H' * y_obs;
    
    if iter == 1
        fprintf('Formed normal equations: H''*H (%dx%d), H''*y (%dx1)\n', ...
            size(HtH, 1), size(HtH, 2), length(Hty));
    end
    
    % Check condition number
    cond_number = cond(HtH);
    cond_number_all(iter) = single(cond_number);
    if iter == 1
        fprintf('Condition number of H''*H: %.2e\n', cond_number);
    end

    %% Step 8: Solve Using Cholesky Decomposition
    % Compute Cholesky factorization: L * L' = H' * H
    try
        L = chol(HtH, 'lower');
        if iter == 1
            fprintf('Cholesky decomposition successful\n');
        end
    catch
        error('Cholesky decomposition failed - H''*H is not positive definite');
    end
    
    % Solve L * z = H' * y using forward substitution
    z = L \ Hty;
    
    % Solve L' * h_est = z using backward substitution
    h_est = L' \ z;
    
    if iter == 1
        fprintf('Solved least squares problem using Cholesky decomposition\n');
        
        % Verify solution using MATLAB backslash operator
        h_est_matlab = HtH \ Hty;
        backslash_diff = max(abs(h_est - h_est_matlab));
        fprintf('Verification: Max difference vs. backslash operator = %.6e\n', backslash_diff);
    end

    %% Step 9: Validation and Metrics
    % Calculate estimation error
    err = h_true - h_est;
    
    % Compute error metrics
    mse = single(mean(abs(err).^2));
    normalized_mse = mse / single(mean(abs(h_true).^2));
    
    % Norm-based error (more intuitive)
    error_norm = norm(err);
    signal_norm = norm(h_true);
    normalized_error = error_norm / signal_norm;
    
    % Maximum absolute error
    max_abs_error = max(abs(err));
    
    % Store metrics for this iteration
    mse_all(iter) = mse;
    normalized_mse_all(iter) = normalized_mse;
    error_norm_all(iter) = single(error_norm);
    signal_norm_all(iter) = single(signal_norm);
    normalized_error_all(iter) = single(normalized_error);
    max_abs_error_all(iter) = max_abs_error;
    
    if iter == 1
        fprintf('\n');
        fprintf('Channel Estimation Results (Iteration 1)\n');
        fprintf('===========================\n');
        fprintf('Mean Squared Error (MSE): %.6e\n', mse);
        fprintf('Normalized MSE (MSE/signal_power): %.6e\n', normalized_mse);
        fprintf('Error Norm: %.6e\n', error_norm);
        fprintf('Signal Norm: %.6e\n', signal_norm);
        fprintf('Normalized Error (||err||/||h_true||): %.6e (%.2f%%)\n', normalized_error, normalized_error*100);
        fprintf('Maximum Absolute Error: %.6e\n', max_abs_error);
        
        % Display data type confirmation
        fprintf('\nData Type Verification:\n');
        fprintf('h_true class: %s\n', class(h_true));
        fprintf('h_est class: %s\n', class(h_est));
        fprintf('HtH class: %s\n', class(HtH));
    else
        fprintf(' done\n');
    end
    
end  % End of iteration loop

%% Step 10: Statistics Across All Iterations
fprintf('\n');
fprintf('========================================\n');
fprintf('Statistics Across %d Iterations\n', Niter);
fprintf('========================================\n');
fprintf('Normalized Error (||err||/||h_true||):\n');
fprintf('  Mean:   %.6e (%.2f%%)\n', mean(normalized_error_all), mean(normalized_error_all)*100);
fprintf('  Std:    %.6e (%.2f%%)\n', std(normalized_error_all), std(normalized_error_all)*100);
fprintf('  Min:    %.6e (%.2f%%)\n', min(normalized_error_all), min(normalized_error_all)*100);
fprintf('  Max:    %.6e (%.2f%%)\n', max(normalized_error_all), max(normalized_error_all)*100);
fprintf('\n');
fprintf('Mean Squared Error (MSE):\n');
fprintf('  Mean:   %.6e\n', mean(mse_all));
fprintf('  Std:    %.6e\n', std(mse_all));
fprintf('  Min:    %.6e\n', min(mse_all));
fprintf('  Max:    %.6e\n', max(mse_all));
fprintf('\n');
fprintf('Normalized MSE:\n');
fprintf('  Mean:   %.6e\n', mean(normalized_mse_all));
fprintf('  Std:    %.6e\n', std(normalized_mse_all));
fprintf('\n');
fprintf('Maximum Absolute Error:\n');
fprintf('  Mean:   %.6e\n', mean(max_abs_error_all));
fprintf('  Std:    %.6e\n', std(max_abs_error_all));
fprintf('\n');
fprintf('Condition Number of H''*H:\n');
fprintf('  Mean:   %.2e\n', mean(cond_number_all));
fprintf('  Std:    %.2e\n', std(cond_number_all));

%% Step 11: Visualization (First Iteration Only)
figure('Position', [100, 100, 1200, 800]);

% Plot 1: Channel tap magnitudes
subplot(2, 2, 1);
stem(1:channel_taps, abs(h_true), 'b', 'LineWidth', 2, 'MarkerSize', 6);
hold on;
stem(passthru_tap_position, abs(h_true(passthru_tap_position)), 'r', 'LineWidth', 2, 'MarkerSize', 8);
stem(1:channel_taps, abs(h_est), 'k--', 'LineWidth', 2, 'MarkerSize', 6);
grid on;
xlabel('Tap Index');
ylabel('Magnitude');
title('Channel Tap Magnitudes');
legend('True Channel', 'Pass-through Tap', 'Estimated Channel', 'Location', 'best');

% Plot 2: Channel tap phases
subplot(2, 2, 2);
stem(1:channel_taps, angle(h_true), 'b', 'LineWidth', 2, 'MarkerSize', 6);
hold on;
stem(1:channel_taps, angle(h_est), 'r--', 'LineWidth', 2, 'MarkerSize', 6);
grid on;
xlabel('Tap Index');
ylabel('Phase (radians)');
title('Channel Tap Phases');
legend('True Channel', 'Estimated Channel', 'Location', 'best');

% Plot 3: Estimation error (magnitude)
subplot(2, 2, 3);
stem(1:channel_taps, abs(err), 'k', 'LineWidth', 1.5);
grid on;
xlabel('Tap Index');
ylabel('Error Magnitude');
title(sprintf('Estimation Error (MSE = %.6e)', mse));

% Plot 4: Constellation diagram (first 200 symbols)
subplot(2, 2, 4);
stem3(real(tx_symbols(1:min(200, length(tx_symbols)))), ...
     imag(tx_symbols(1:min(200, length(tx_symbols)))), ...
     ones(min(200, length(tx_symbols)), 1), 'filled', 'MarkerSize', 4);
view(2);
grid on;
axis equal;
xlabel('In-Phase');
ylabel('Quadrature');
title(sprintf('%d-PSK Constellation', M));

sgtitle(sprintf('M-PSK Channel Estimation - Iteration 1 (M=%d, Nsymbols=%d, SNR=%.1fdB)', ...
    M, Nsymbols, SNR_dB), 'FontSize', 14, 'FontWeight', 'bold');

if ~exist('../data', 'dir')
    mkdir('../data');
end

%% Step 12: Optional Data Export for AIE Stimulus
% Uncomment to save matrices for potential AIE design integration
save_aie_stimulus = true;
if save_aie_stimulus
    for write_iter = 1:NO_OF_ITERATIONS
        if write_iter == 1
            file_mode = 'w';
        else
            file_mode = 'a';
        end

        % Save H'*H matrix (column-major format, PLIO style)
        fid = fopen('../data/A_matrix.txt', file_mode);
        for col = 1:channel_taps
            for row = 1:2:channel_taps  % 2 complex samples per line (plio_64_bits)
                if row < channel_taps
                    val1 = HtH(row, col);
                    val2 = HtH(row+1, col);
                    fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
                    fprintf(fid, '%.8e %.8e\n', real(val2), imag(val2));
                    % fprintf(fid, '%.8e %.8e %.8e %.8e\n', ...
                    %     real(val1), imag(val1), real(val2), imag(val2));
                else
                    val1 = HtH(row, col);
                    fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
                end
            end
        end
        fclose(fid);

        % Save H'*y vector
        fid = fopen('../data/b_input.txt', file_mode);
        for row = 1:2:channel_taps
            if row < channel_taps
                val1 = Hty(row);
                val2 = Hty(row+1);
                fprintf(fid, '%.8e %.8e \n', ...
                    real(val1), imag(val1), real(val2), imag(val2));
            else
                val1 = Hty(row);
                fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
            end
        end
        fclose(fid);

        % Store the x_output vector - 32 taps - 2 taps in a line
        fid = fopen('../data/G_x_output.txt', file_mode);
        for row = 1:2:channel_taps
            if row < channel_taps
                val1 = h_est(row);
                val2 = h_est(row+1);
                fprintf(fid, '%.8e %.8e \n', ...
                    real(val1), imag(val1), real(val2), imag(val2));
            else
                val1 = h_est(row);
                fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
            end
        end
        fclose(fid);
    end

    fprintf('\nSaved H''*H matrix to A_matrix.txt (HtH_matrix) - %d iteration(s)\n', NO_OF_ITERATIONS);
    fprintf('Saved H''*y vector to b_input.txt (Hty_vector) - %d iteration(s)\n', NO_OF_ITERATIONS);
    fprintf('Saved x_output vector to G_x_output.txt (h_est_vector) - %d iteration(s)\n', NO_OF_ITERATIONS);
end

fprintf('\n');
fprintf('Simulation complete!\n');
