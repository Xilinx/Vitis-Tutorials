% 
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% Portions of this file consist of AI-generated content.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

%% M-PSK Communication System with Linear Equalization
% This script demonstrates linear equalization for an M-PSK communication
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
equalizer_taps = 32;  % Number of FIR equalizer taps
training_ratio = 0.5; % Fraction of symbols used for training (0 < ratio <= 1)
Niter = 8;            % Number of iterations for statistics
NO_OF_ITERATIONS = 4; % Number of times to repeat the same test vectors in output files

% Channel model parameters
passthru_tap_position = 12;  % Unity tap position in second quarter (9-16 for 32 taps)
num_dispersive_taps = 5;     % Number of random taps around pass-through (odd number recommended)
isi_power_fraction = 0.25;    % Fraction of total power in ISI taps (0 to 1, remaining in pass-through)

% Fixed parameters for reproducibility
rng(1);               % Fixed random seed (matches existing test data generation)

fprintf('M-PSK Linear Equalization Simulation\n');
fprintf('======================================\n');
fprintf('Modulation Order (M): %d\n', M);
fprintf('Number of Symbols: %d\n', Nsymbols);
fprintf('SNR: %.1f dB\n', SNR_dB);
fprintf('Channel Taps: %d\n', channel_taps);
fprintf('Pass-through Tap Position: %d\n', passthru_tap_position);
fprintf('Dispersive Taps: %d\n', num_dispersive_taps);
fprintf('ISI Power Fraction: %.2f (Pass-through: %.2f)\n', isi_power_fraction, 1-isi_power_fraction);
fprintf('Equalizer Taps: %d\n', equalizer_taps);
fprintf('Training Ratio: %.1f%%\n', training_ratio*100);
fprintf('Number of Iterations: %d\n', Niter);
fprintf('\n');

% Initialize arrays to store metrics across iterations
mse_all = zeros(Niter, 1, 'single');
ber_all = zeros(Niter, 1, 'single');
ser_all = zeros(Niter, 1, 'single');
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
    h_channel = single(zeros(channel_taps, 1));
    
    % Compute tap indices centered around pass-through
    half_span = floor(num_dispersive_taps / 2);
    tap_start = max(1, passthru_tap_position - half_span);
    tap_end = min(channel_taps, passthru_tap_position + half_span);
    dispersive_indices = tap_start:tap_end;
    
    % Generate random complex ISI taps at dispersive positions (excluding pass-through)
    isi_indices = dispersive_indices(dispersive_indices ~= passthru_tap_position);
    for idx = isi_indices
        h_channel(idx) = single((randn() + 1j*randn()) / sqrt(2));
    end
    
    % Normalize ISI taps to specified power fraction
    if ~isempty(isi_indices)
        current_isi_power = sum(abs(h_channel(isi_indices)).^2);
        if current_isi_power > 0
            h_channel(isi_indices) = h_channel(isi_indices) * sqrt(single(isi_power_fraction) / current_isi_power);
        end
    end
    
    % Set pass-through tap to remaining power
    h_channel(passthru_tap_position) = sqrt(single(1 - isi_power_fraction));
    
    if iter == 1
        fprintf('Generated composite %d-tap complex FIR channel (single precision)\n', channel_taps);
        fprintf('  Pass-through tap at position %d\n', passthru_tap_position);
        fprintf('  ISI taps from %d to %d (excluding position %d)\n', tap_start, tap_end, passthru_tap_position);
        fprintf('  Pass-through power: %.6f\n', abs(h_channel(passthru_tap_position))^2);
        fprintf('  ISI power: %.6f\n', sum(abs(h_channel(isi_indices)).^2));
        fprintf('  Total channel power: %.6f\n', sum(abs(h_channel).^2));
    end

    %% Step 4: Apply FIR Channel
    % Convolve transmitted symbols with channel
    y_channel = filter(h_channel, single(1), tx_symbols);
    
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

    %% Step 6: Partition into Training and Testing Sets
    % Remove initial transient samples (channel memory)
    transient_samples = channel_taps - 1;
    y_valid = y_noisy(channel_taps:end);
    x_valid = tx_symbols(channel_taps:end);
    
    % Determine training length
    num_valid = length(y_valid);
    num_training = floor(num_valid * training_ratio);
    num_testing = num_valid - num_training;
    
    % Split into training and testing
    y_train = y_valid(1:num_training);
    x_train = x_valid(1:num_training);
    y_test = y_valid(num_training+1:end);
    x_test = x_valid(num_training+1:end);
    
    if iter == 1
        fprintf('Training samples: %d, Testing samples: %d\n', num_training, num_testing);
    end

    %% Step 7: Construct Least Squares Problem for Equalizer
    % Build convolution matrix Y from received symbols (Toeplitz structure)
    % Each row contains equalizer_taps consecutive received symbols in REVERSE time order
    % For equolution: x_est[n] = w[1]*y[n] + w[2]*y[n-1] + ... + w[L]*y[n-L+1]
    % So Y(i,:) should be [y[n], y[n-1], y[n-2], ..., y[n-L+1]]
    
    % Set equalizer delay to align with channel (typically at pass-through position)
    % This determines which transmitted symbol we're trying to recover
    equalizer_delay = passthru_tap_position;
    
    num_train_samples = length(y_train) - equalizer_taps + 1;
    Y = single(zeros(num_train_samples, equalizer_taps));
    
    for i = 1:num_train_samples
        % Reverse order for proper convolution: y[n], y[n-1], ..., y[n-L+1]
        Y(i, :) = y_train(i+equalizer_taps-1:-1:i).';
    end
    
    % Align transmitted symbols with equalizer delay
    % x_desired(i) should correspond to the symbol that Y(i,:) is trying to recover
    % accounting for the channel delay and equalizer window position
    x_desired = x_train(equalizer_delay:equalizer_delay+num_train_samples-1);
    
    if iter == 1
        fprintf('Constructed convolution matrix Y: %dx%d (single precision)\n', size(Y, 1), size(Y, 2));
        fprintf('Equalizer delay (alignment): %d samples\n', equalizer_delay);
        fprintf('Desired training symbols x: %dx1\n', length(x_desired));
    end

    %% Step 8: Form Normal Equations
    % Y' * Y matrix (equalizer_taps x equalizer_taps)
    YtY = Y' * Y;
    
    % Y' * x vector (equalizer_taps x 1)
    Ytx = Y' * x_desired;
    
    if iter == 1
        fprintf('Formed normal equations: Y''*Y (%dx%d), Y''*x (%dx1)\n', ...
            size(YtY, 1), size(YtY, 2), length(Ytx));
    end
    
    % Check condition number
    cond_number = cond(YtY);
    cond_number_all(iter) = single(cond_number);
    if iter == 1
        fprintf('Condition number of Y''*Y: %.2e\n', cond_number);
    end

    %% Step 9: Solve Using Cholesky Decomposition
    % Compute Cholesky factorization: L * L' = Y' * Y
    try
        L = chol(YtY, 'lower');
        if iter == 1
            fprintf('Cholesky decomposition successful\n');
        end
    catch
        error('Cholesky decomposition failed - Y''*Y is not positive definite');
    end
    
    % Solve L * z = Y' * x using forward substitution
    z = L \ Ytx;
    
    % Solve L' * w = z using backward substitution
    w_equalizer = L' \ z;
    
    if iter == 1
        fprintf('Solved least squares problem using Cholesky decomposition\n');
        
        % Verify solution using MATLAB backslash operator
        w_matlab = YtY \ Ytx;
        backslash_diff = max(abs(w_equalizer - w_matlab));
        fprintf('Verification: Max difference vs. backslash operator = %.6e\n', backslash_diff);
    end

    %% Step 10: Apply Equalizer to Test Data
    % Build convolution matrix for test data
    num_test_samples = length(y_test) - equalizer_taps + 1;
    Y_test = single(zeros(num_test_samples, equalizer_taps));
    
    for i = 1:num_test_samples
        Y_test(i, :) = y_test(i+equalizer_taps-1:-1:i).';
    end
    
    % Apply equalizer: x_est = Y_test * w
    x_est = Y_test * w_equalizer;
    
    % Align reference symbols with same equalizer delay used in training
    x_reference = x_test(equalizer_delay:equalizer_delay+num_test_samples-1);
    
    if iter == 1
        fprintf('Applied equalizer to %d test samples\n', num_test_samples);
    end

    %% Step 11: Validation and Metrics
    % Calculate equalization error
    err = x_reference - x_est;
    
    % Compute error metrics
    mse = single(mean(abs(err).^2));
    
    % Norm-based error (more intuitive)
    error_norm = norm(err);
    signal_norm = norm(x_reference);
    normalized_error = error_norm / signal_norm;
    
    % Maximum absolute error
    max_abs_error = max(abs(err));
    
    % Symbol Error Rate (SER) - decision-based
    % Make hard decisions by finding nearest constellation point
    x_est_decided = zeros(size(x_est), 'single');
    constellation = single(exp(1j * 2 * pi * (0:M-1) / M));
    for i = 1:length(x_est)
        [~, idx] = min(abs(x_est(i) - constellation));
        x_est_decided(i) = constellation(idx);
    end
    num_symbol_errors = sum(x_est_decided ~= x_reference);
    ser = single(num_symbol_errors / num_test_samples);
    
    % Bit Error Rate (BER) - by demodulating
    % Convert to bits and compare
    x_ref_bits = zeros(num_test_samples * bits_per_symbol, 1);
    x_est_bits = zeros(num_test_samples * bits_per_symbol, 1);
    for i = 1:num_test_samples
        % Reference bits
        [~, ref_idx] = min(abs(x_reference(i) - constellation));
        ref_symbol_idx = ref_idx - 1; % 0-indexed
        ref_bits = de2bi(ref_symbol_idx, bits_per_symbol, 'left-msb');
        x_ref_bits((i-1)*bits_per_symbol+1:i*bits_per_symbol) = ref_bits;
        
        % Estimated bits
        [~, est_idx] = min(abs(x_est_decided(i) - constellation));
        est_symbol_idx = est_idx - 1; % 0-indexed
        est_bits = de2bi(est_symbol_idx, bits_per_symbol, 'left-msb');
        x_est_bits((i-1)*bits_per_symbol+1:i*bits_per_symbol) = est_bits;
    end
    num_bit_errors = sum(x_ref_bits ~= x_est_bits);
    ber = single(num_bit_errors / (num_test_samples * bits_per_symbol));
    
    % Store metrics for this iteration
    mse_all(iter) = mse;
    ber_all(iter) = ber;
    ser_all(iter) = ser;
    error_norm_all(iter) = single(error_norm);
    signal_norm_all(iter) = single(signal_norm);
    normalized_error_all(iter) = single(normalized_error);
    max_abs_error_all(iter) = max_abs_error;
    
    if iter == 1
        fprintf('\n');
        fprintf('Equalization Results (Iteration 1)\n');
        fprintf('===========================\n');
        fprintf('Mean Squared Error (MSE): %.6e\n', mse);
        fprintf('Error Norm: %.6e\n', error_norm);
        fprintf('Signal Norm: %.6e\n', signal_norm);
        fprintf('Normalized Error (||err||/||x||): %.6e (%.2f%%)\n', normalized_error, normalized_error*100);
        fprintf('Maximum Absolute Error: %.6e\n', max_abs_error);
        fprintf('Symbol Error Rate (SER): %.6e (%.4f%%)\n', ser, ser*100);
        fprintf('Bit Error Rate (BER): %.6e (%.4f%%)\n', ber, ber*100);
        fprintf('Number of Test Symbols: %d\n', num_test_samples);
        fprintf('Symbol Errors: %d\n', num_symbol_errors);
        fprintf('Bit Errors: %d\n', num_bit_errors);
        
        % Display data type confirmation
        fprintf('\nData Type Verification:\n');
        fprintf('w_equalizer class: %s\n', class(w_equalizer));
        fprintf('x_est class: %s\n', class(x_est));
        fprintf('YtY class: %s\n', class(YtY));
    else
        fprintf(' done\n');
    end
    
end  % End of iteration loop

%% Step 12: Statistics Across All Iterations
fprintf('\n');
fprintf('========================================\n');
fprintf('Statistics Across %d Iterations\n', Niter);
fprintf('========================================\n');
fprintf('Normalized Error (||err||/||x||):\n');
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
fprintf('Symbol Error Rate (SER):\n');
fprintf('  Mean:   %.6e (%.4f%%)\n', mean(ser_all), mean(ser_all)*100);
fprintf('  Std:    %.6e\n', std(ser_all));
fprintf('  Min:    %.6e (%.4f%%)\n', min(ser_all), min(ser_all)*100);
fprintf('  Max:    %.6e (%.4f%%)\n', max(ser_all), max(ser_all)*100);
fprintf('\n');
fprintf('Bit Error Rate (BER):\n');
fprintf('  Mean:   %.6e (%.4f%%)\n', mean(ber_all), mean(ber_all)*100);
fprintf('  Std:    %.6e\n', std(ber_all));
fprintf('  Min:    %.6e (%.4f%%)\n', min(ber_all), min(ber_all)*100);
fprintf('  Max:    %.6e (%.4f%%)\n', max(ber_all), max(ber_all)*100);
fprintf('\n');
fprintf('Maximum Absolute Error:\n');
fprintf('  Mean:   %.6e\n', mean(max_abs_error_all));
fprintf('  Std:    %.6e\n', std(max_abs_error_all));
fprintf('\n');
fprintf('Condition Number of Y''*Y:\n');
fprintf('  Mean:   %.2e\n', mean(cond_number_all));
fprintf('  Std:    %.2e\n', std(cond_number_all));

%% Step 13: Visualization (First Iteration Only)
% Calculate SER at equalizer input (before equalization)
% Make hard decisions on received test symbols
y_test_decided = zeros(size(y_test), 'single');
for i = 1:length(y_test)
    [~, idx] = min(abs(y_test(i) - constellation));
    y_test_decided(i) = constellation(idx);
end
% Compare with transmitted symbols at appropriate delay
x_test_aligned = x_test(equalizer_delay:min(equalizer_delay+length(y_test)-1, length(x_test)));
y_test_subset = y_test_decided(1:length(x_test_aligned));
num_symbol_errors_input = sum(y_test_subset ~= x_test_aligned);
ser_input = single(num_symbol_errors_input / length(x_test_aligned));

figure('Position', [100, 100, 1400, 900]);

% Plot 1: Channel impulse response magnitudes
subplot(2, 3, 1);
stem(1:channel_taps, abs(h_channel), 'k', 'LineWidth', 2, 'MarkerSize', 6);
hold on;
stem(passthru_tap_position, abs(h_channel(passthru_tap_position)), 'r', 'LineWidth', 2, 'MarkerSize', 8);
grid on;
xlabel('Tap Index');
ylabel('Magnitude');
title('Channel Impulse Response');
legend('Channel Taps', 'Pass-through Tap', 'Location', 'best');

% Plot 2: Equalizer tap magnitudes
subplot(2, 3, 2);
stem(1:equalizer_taps, abs(w_equalizer), 'b', 'LineWidth', 2, 'MarkerSize', 6);
grid on;
xlabel('Tap Index');
ylabel('Magnitude');
title('Equalizer Tap Magnitudes');

% Plot 3: Equalizer tap phases
subplot(2, 3, 3);
stem(1:equalizer_taps, angle(w_equalizer), 'b', 'LineWidth', 2, 'MarkerSize', 6);
grid on;
xlabel('Tap Index');
ylabel('Phase (radians)');
title('Equalizer Tap Phases');

% Plot 4: Constellation at equalizer input (received signal)
subplot(2, 3, 4);
% Use subset of received test symbols for visualization
num_display = min(200, length(y_test));
plot(real(y_test(1:num_display)), imag(y_test(1:num_display)), 'kx', 'MarkerSize', 8);
grid on;
axis equal;
xlabel('In-Phase');
ylabel('Quadrature');
title(sprintf('Equalizer Input\nSNR=%.1fdB, ISI Power=%.2f, SER=%.2f%%', SNR_dB, isi_power_fraction, ser_input*100));

% Plot 5: Constellation diagram - Transmitted vs Equalized
subplot(2, 3, 5);
plot(real(x_est), imag(x_est), 'kx', 'MarkerSize', 6, 'LineWidth', 1.5);
hold on;
plot(real(x_reference), imag(x_reference), 'mo', 'MarkerSize', 8, 'LineWidth', 1.5, 'MarkerFaceColor', 'm');
grid on;
axis equal;
xlabel('In-Phase');
ylabel('Quadrature');
title(sprintf('Equalizer Output\nSER = %.4f%%', ser*100));
legend('Equalized Symbols', 'Reference Symbols', 'Location', 'best');

% Plot 6: Combined channel and equalizer frequency response
subplot(2, 3, 6);
[H_channel, f_channel] = freqz(h_channel, 1, 512);
[H_equalizer, f_equalizer] = freqz(w_equalizer, 1, 512);
H_combined = H_channel .* H_equalizer;
plot(f_channel/pi, 20*log10(abs(H_channel)), 'k-', 'LineWidth', 1.5);
hold on;
plot(f_equalizer/pi, 20*log10(abs(H_equalizer)), 'b-', 'LineWidth', 1.5);
plot(f_channel/pi, 20*log10(abs(H_combined)), 'r--', 'LineWidth', 2);
grid on;
xlabel('Normalized Frequency (\times\pi rad/sample)');
ylabel('Magnitude (dB)');
title('Frequency Response');
legend('Channel', 'Equalizer', 'Combined', 'Location', 'best');

sgtitle(sprintf('M-PSK Linear Equalization - Iteration 1 (M=%d, SNR=%.1fdB, Training=%.0f%%)', ...
    M, SNR_dB, training_ratio*100), 'FontSize', 14, 'FontWeight', 'bold');

if ~exist('../data', 'dir')
    mkdir('../data');
end

%% Step 14: Optional Data Export for AIE Stimulus
% Uncomment to save matrices for potential AIE design integration
save_aie_stimulus = true;
for write_iter = 1:NO_OF_ITERATIONS
    if write_iter == 1
        file_mode = 'w';
    else
        file_mode = 'a';
    end

    if save_aie_stimulus
        % Save Y'*Y matrix (column-major format, PLIO style)
        fid = fopen('../data/A_matrix.txt', file_mode);
        for col = 1:equalizer_taps
            for row = 1:2:equalizer_taps  % 2 complex samples per line (plio_64_bits)
                if row < equalizer_taps
                    val1 = YtY(row, col);
                    val2 = YtY(row+1, col);
                    fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
                    fprintf(fid, '%.8e %.8e\n', real(val2), imag(val2));
                else
                    val1 = YtY(row, col);
                    fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
                end
            end
        end
        fclose(fid);

        % Save Y'*x vector
        fid = fopen('../data/b_input.txt', file_mode);
        for row = 1:2:equalizer_taps
            if row < equalizer_taps
                val1 = Ytx(row);
                val2 = Ytx(row+1);
                fprintf(fid, '%.8e %.8e \n', ...
                    real(val1), imag(val1), real(val2), imag(val2));
            else
                val1 = Ytx(row);
                fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
            end
        end
        fclose(fid);
    end

    % Store the x_output vector - 32 taps - 2 taps in a line
    fid = fopen('../data/G_x_output.txt', file_mode);
    for row = 1:2:channel_taps
        if row < channel_taps
            val1 = w_equalizer(row);
            val2 = w_equalizer(row+1);
            fprintf(fid, '%.8e %.8e \n', ...
                real(val1), imag(val1), real(val2), imag(val2));
        else
            val1 = h_est(row);
            fprintf(fid, '%.8e %.8e\n', real(val1), imag(val1));
        end
    end
    fclose(fid);
end

if save_aie_stimulus
    fprintf('\nSaved Y''*Y matrix to A_matrix.txt (YtY matrix), %d iteration(s)\n', NO_OF_ITERATIONS);
    fprintf('Saved Y''*x vector to b_input.txt (Ytx_vector), %d iteration(s)\n', NO_OF_ITERATIONS);
end
fprintf('Saved x_output vector to G_x_output.txt (w_equalizer), %d iteration(s)\n', NO_OF_ITERATIONS);


fprintf('\n');
fprintf('Simulation complete!\n');
