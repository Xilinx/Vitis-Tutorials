% 
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% Portions of this file consist of AI-generated content.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function symbols = mpsk_modulate(data_bits, M)
    % MPSK_MODULATE  M-PSK modulation mapper
    %
    % Inputs:
    %   data_bits - Vector of data bits (0 or 1)
    %   M         - Modulation order (2, 4, 8, 16, etc.)
    %
    % Outputs:
    %   symbols   - Complex symbol vector (single precision)
    %
    % Description:
    %   Maps input bits to M-PSK constellation points using Gray coding.
    %   Constellation: exp(1j * 2*pi * k/M) for k = 0, 1, ..., M-1
    %   Output is single precision complex to match AIE cfloat datatype.
    
    % Validate inputs
    if ~ismember(M, [2, 4, 8, 16, 32, 64])
        error('M must be a power of 2: 2, 4, 8, 16, 32, or 64');
    end
    
    % Number of bits per symbol
    bits_per_symbol = log2(M);
    
    % Reshape bits into symbols (each row is one symbol)
    num_symbols = floor(length(data_bits) / bits_per_symbol);
    data_bits = data_bits(1:num_symbols*bits_per_symbol); % Trim to multiple of bits_per_symbol
    bit_matrix = reshape(data_bits, bits_per_symbol, num_symbols)';
    
    % Convert bit groups to decimal symbol indices
    symbol_indices = zeros(num_symbols, 1);
    for i = 1:bits_per_symbol
        symbol_indices = symbol_indices + bit_matrix(:,i) * 2^(bits_per_symbol - i);
    end
    
    % Generate M-PSK constellation (single precision)
    constellation = single(exp(1j * 2*pi * (0:M-1)' / M));
    
    % Map indices to constellation points
    symbols = constellation(symbol_indices + 1); % MATLAB indexing starts at 1
    
    % Ensure output is column vector in single precision
    symbols = single(symbols(:));
end
