# M-PSK Communication System MATLAB Models

## Overview

This directory contains MATLAB models demonstrating two complementary applications of least squares solution via Cholesky decomposition in M-PSK communication systems. Both models use an **identical structured channel model** and generate test data and golden references for potential AI Engine (AIE) design integration.

**Applications**:
1. **Channel Estimation**: Estimate unknown channel taps from known transmitted symbols
2. **Linear Equalization**: Design an equalizer to invert channel effects and recover transmitted symbols

**Key Features**:
- Parameterized M-PSK modulation (configurable modulation order)
- **Unified 32-tap composite channel model** with controlled ISI (pass-through + dispersive taps)
- AWGN noise addition with configurable SNR
- Least squares solution using Cholesky decomposition with forward/backward substitution
- **Single precision floating point** throughout (compatible with AIE `float` and `cfloat` datatypes)
- Comprehensive visualization and performance metrics

---

## Files

| File | Description |
|------|-------------|
| `mpsk_channel_estimation.m` | Channel estimation simulation (estimate h from known x) |
| `mpsk_linear_equalization.m` | Linear equalization simulation (design w to recover x from y) |
| `mpsk_modulate.m` | M-PSK modulation helper function |
| `README.md` | This documentation file |

---

## Application 1: Channel Estimation

### Purpose
Estimate the unknown channel impulse response from transmitted training symbols and received observations.

### Problem Formulation
- **Given**: Transmitted symbols **x** (known), received signal **y** = **H**·**h** + **n**
- **Find**: Channel taps **h** (unknown)
- **Method**: Solve least squares problem via Cholesky decomposition

### Usage

Run the channel estimation script:

```matlab
mpsk_channel_estimation
```

### Key Parameters

```matlab
M = 8;                          % Modulation order (8-PSK)
Nsymbols = 512;                 % Number of transmitted symbols
SNR_dB = 10;                    % Signal-to-noise ratio in dB
channel_taps = 32;              % Number of FIR channel taps
passthru_tap_position = 12;    % Main channel tap position
num_dispersive_taps = 5;       % Number of ISI taps around main tap
isi_power_fraction = 0.1;      % Fraction of power in ISI (10%)
```

### Channel Model
Uses the same **composite channel model** as linear equalization:
- **Pass-through tap**: Dominant path at configurable position (default: position 12)
- **ISI taps**: Random dispersive taps around pass-through with controlled power
- **Power normalization**: Total channel power = 1.0
  - Pass-through power = 1 - isi_power_fraction
  - ISI power = isi_power_fraction

### Outputs
- Channel estimation error metrics (MSE, normalized error, max error)
- Visualization: True vs. estimated channel taps (with pass-through highlighted), constellation diagram
- Condition number of **H**'·**H** matrix

---

## Application 2: Linear Equalization

### Purpose
Design an equalizer to recover transmitted symbols from received signal corrupted by channel and noise.

### Problem Formulation
- **Given**: Received symbols **y**, training symbols **x** (for training phase)
- **Find**: Equalizer weights **w** such that **w**·**y** ≈ **x**
- **Method**: Solve least squares problem via Cholesky decomposition with proper delay alignment

### Usage

Run the linear equalization script:

```matlab
mpsk_linear_equalization
```

### Key Parameters

```matlab
M = 8;                          % Modulation order (8-PSK)
Nsymbols = 2048;                % Number of transmitted symbols
SNR_dB = 15;                    % Signal-to-noise ratio in dB
channel_taps = 32;              % Number of FIR channel taps
equalizer_taps = 32;            % Number of FIR equalizer taps
training_ratio = 0.5;           % Fraction of symbols for training (50%)
passthru_tap_position = 12;    % Main channel tap position
num_dispersive_taps = 5;       % Number of ISI taps around main tap
isi_power_fraction = 0.05;     % Fraction of power in ISI (5%)
```

### Channel Model
The equalizer uses a **composite channel model**:
- **Pass-through tap**: Dominant path at configurable position (default: position 12)
- **ISI taps**: Random dispersive taps around pass-through with controlled power
- **Power normalization**: Total channel power = 1.0
  - Pass-through power = 1 - isi_power_fraction
  - ISI power = isi_power_fraction

This models realistic wireless channels with a strong direct path and multipath distortion.

### Training/Testing Split
- **Training phase**: Use known symbols to compute equalizer weights **w**
- **Testing phase**: Apply equalizer to independent data and measure performance
- Data partitioned according to `training_ratio`

### Outputs
- **Symbol Error Rate (SER)**: Before and after equalization
- **Bit Error Rate (BER)**: Communication system performance
- **Visualization**:
  - Channel impulse response
  - Equalizer tap weights (magnitude and phase)
  - Constellation diagrams (input vs. output)
  - Frequency response (channel, equalizer, combined)

---

## Algorithm Description

Both applications use the same core algorithm and **identical structured channel model**.

### Composite Channel Model (Both Applications)

The channel model consists of:
- **Pass-through tap**: Dominant path at position `passthru_tap_position` (default: 12)
- **ISI taps**: Random complex Gaussian taps at positions around the pass-through
- **Power control**: 
  - ISI taps normalized to `isi_power_fraction` of total power (e.g., 0.1 = 10%)
  - Pass-through tap set to `sqrt(1 - isi_power_fraction)` of total power
  - Total channel power = 1.0

This models realistic wireless channels with:
- Strong direct/line-of-sight path (pass-through)
- Controlled multipath distortion (ISI taps)
- Adjustable severity via `isi_power_fraction`

### Communication System Model (Both Applications)

1. **Data Generation**: Random bits generated based on modulation order M
2. **M-PSK Modulation**: Bits mapped to constellation points: $e^{j2\pi k/M}$ for $k = 0, 1, \ldots, M-1$
3. **Composite FIR Channel**: 32-tap structured channel with controlled ISI
4. **AWGN Noise**: Complex Gaussian noise added at specified SNR

### Channel Estimation: Least Squares Problem

**Problem Formulation**:
- Received signal: $y = H \cdot h + n$
  - $H$: Convolution matrix (Toeplitz structure) from transmitted symbols **x**
  - $h$: Unknown channel taps (32×1 vector)
  - $n$: AWGN noise
- Least squares solution: Minimize $\|y - H\cdot h\|^2$

**Normal Equations**:
$$H^H H \cdot \hat{h} = H^H y$$

**Cholesky Solution**:
1. Factor: $H^H H = L \cdot L^H$ (Cholesky decomposition)
2. Forward substitution: Solve $L \cdot z = H^H y$ for $z$
3. Backward substitution: Solve $L^H \cdot \hat{h} = z$ for $\hat{h}$

**Validation**:
- Compare $\hat{h}$ (estimated) vs. $h_{true}$ (ground truth)
- Metrics: MSE, normalized MSE, maximum absolute error

### Linear Equalization: Least Squares Problem

**Problem Formulation**:
- Equalization output: $\hat{x} = Y \cdot w$
  - $Y$: Convolution matrix (Toeplitz structure) from received symbols **y**
  - $w$: Unknown equalizer weights (32×1 vector)
  - $\hat{x}$: Estimated transmitted symbols
- Least squares solution: Minimize $\|x_{desired} - Y\cdot w\|^2$

**Normal Equations**:
$$Y^H Y \cdot \hat{w} = Y^H x_{desired}$$

**Cholesky Solution**:
1. Factor: $Y^H Y = L \cdot L^H$ (Cholesky decomposition)
2. Forward substitution: Solve $L \cdot z = Y^H x_{desired}$ for $z$
3. Backward substitution: Solve $L^H \cdot \hat{w} = z$ for $\hat{w}$

**Critical Alignment**:
- **Equalizer delay**: Set to pass-through tap position (e.g., 12)
- Ensures proper alignment between received window and desired symbol
- Training symbols: $x_{desired} = x_{train}[delay : delay + N - 1]$

**Performance Metrics**:
- Symbol Error Rate (SER): Before and after equalization
- Bit Error Rate (BER): Communication system performance
- MSE: Mean squared error between reference and equalized symbols

---

## Data Format

### Single Precision

All computations use MATLAB's `single` datatype:
- **Real signals**: `single` (32-bit float)
- **Complex signals**: Complex `single` (64-bit cfloat: 32-bit real + 32-bit imag)
- **Compatible with AIE**: Matches AIE `float` and `cfloat` datatypes

### Column-Major Order

MATLAB's native storage format (column-major) is preserved for potential AIE integration.

### Optional PLIO Export

Uncomment the data export section in either script to save matrices:

**Channel Estimation** (`mpsk_channel_estimation.m`):
- `HtH_matrix.txt`: **H**'·**H** matrix (32×32 complex)
- `Hty_vector.txt`: **H**'·**y** vector (32×1 complex)

**Linear Equalization** (`mpsk_linear_equalization.m`):
- `YtY_matrix.txt`: **Y**'·**Y** matrix (32×32 complex)
- `Ytx_vector.txt`: **Y**'·**x** vector (32×1 complex)

**Format**: PLIO-style with 2 complex samples per line (plio_64_bits):
```
real1 imag1 real2 imag2
...
```

---

## Example Outputs

### Channel Estimation Output

```
M-PSK Channel Estimation Simulation
=====================================
Modulation Order (M): 8
Number of Symbols: 512
SNR: 10.0 dB
Channel Taps: 32
Pass-through Tap Position: 12
Dispersive Taps: 5
ISI Power Fraction: 0.10 (Pass-through: 0.90)

========== Iteration 1 ==========
Generated 1536 random data bits
Modulated to 512 M-PSK symbols (single precision)
Generated composite 32-tap complex FIR channel (single precision)
  Pass-through tap at position 12
  ISI taps from 10 to 14 (excluding position 12)
  Pass-through power: 0.900000
  ISI power: 0.100000
  Total channel power: 1.000000
Applied FIR channel filtering
Added AWGN noise (SNR = 10.0 dB, noise variance = 0.001234)
Constructed convolution matrix H: 480x32 (single precision)
Observation vector y: 480x1
Formed normal equations: H'*H (32x32), H'*y (32x1)
Condition number of H'*H: 2.34e+02
Cholesky decomposition successful
Solved least squares problem using Cholesky decomposition

Channel Estimation Results (Iteration 1)
===========================
Mean Squared Error (MSE): 1.234567e-04
Error Norm: 3.456789e-02
Normalized Error (||err||/||h_true||): 1.234567e-02 (1.23%)
Maximum Absolute Error: 2.345678e-02

Statistics Across 8 Iterations
========================================
Normalized Error (||err||/||h_true||):
  Mean:   1.345678e-02 (1.35%)
  Std:    2.345678e-03 (0.23%)
```

### Linear Equalization Output

```
M-PSK Linear Equalization Simulation
======================================
Modulation Order (M): 8
Number of Symbols: 2048
SNR: 15.0 dB
Channel Taps: 32
Pass-through Tap Position: 12
Dispersive Taps: 5
ISI Power Fraction: 0.05 (Pass-through: 0.95)
Equalizer Taps: 32
Training Ratio: 50.0%

========== Iteration 1 ==========
Generated composite 32-tap complex FIR channel (single precision)
  Pass-through tap at position 12
  ISI taps from 10 to 14 (excluding position 12)
  Pass-through power: 0.950000
  ISI power: 0.050000
  Total channel power: 1.000000
Applied FIR channel filtering
Added AWGN noise (SNR = 15.0 dB, noise variance = 0.001234)
Training samples: 1016, Testing samples: 1016
Constructed convolution matrix Y: 985x32 (single precision)
Equalizer delay (alignment): 12 samples
Cholesky decomposition successful
Applied equalizer to 985 test samples

Equalization Results (Iteration 1)
===========================
Mean Squared Error (MSE): 1.234567e-03
Symbol Error Rate (SER): 1.234567e-03 (0.1235%)
Bit Error Rate (BER): 4.567890e-04 (0.0457%)
Number of Test Symbols: 985
Symbol Errors: 1
Bit Errors: 3

Statistics Across 8 Iterations
========================================
Symbol Error Rate (SER):
  Mean:   1.345678e-03 (0.1346%)
  Std:    2.345678e-04
Bit Error Rate (BER):
  Mean:   4.567890e-04 (0.0457%)
  Std:    8.901234e-05
```

---

## Visualization

### Channel Estimation Plots (2×2 layout)
1. **Channel Tap Magnitudes**: True channel (blue), pass-through tap highlighted (red), estimated channel (black dashed)
2. **Channel Tap Phases**: True vs. estimated phase progression
3. **Estimation Error**: Stem plot of error magnitude per tap
4. **Constellation Diagram**: First 200 transmitted M-PSK symbols

### Linear Equalization Plots (2×3 layout)
1. **Channel Impulse Response**: Shows pass-through tap (red) and ISI taps (black)
2. **Equalizer Tap Magnitudes**: Learned equalizer weights
3. **Equalizer Tap Phases**: Phase response of equalizer
4. **Equalizer Input Constellation**: Received symbols with ISI and noise (includes SER before equalization)
5. **Equalizer Output Constellation**: Reference vs. equalized symbols (includes SER after equalization)
6. **Frequency Response**: Channel, equalizer, and combined response showing equalization effectiveness

---

## Performance Considerations

### Numerical Stability (Both Applications)

- **Condition Number**: Reported for **H**'·**H** (estimation) or **Y**'·**Y** (equalization). High values (>1e6) indicate ill-conditioning
- **Single Precision**: May have reduced accuracy vs. double precision (~7 significant digits)
- **Cholesky Feasibility**: Requires positive definite matrix (guaranteed for full-rank convolution matrices)

### Channel Estimation: Parameter Sensitivity

- **Nsymbols ≥ 4 × channel_taps**: Recommended for reliable estimation (overdetermined system)
- **SNR Impact**: Lower SNR → higher MSE. At SNR < 5 dB, estimation may be poor
- **ISI Power**: Higher ISI power (>0.2) creates more challenging channel with multiple significant paths
- **Channel Model**: Different random ISI tap realizations produce different condition numbers

### Linear Equalization: Parameter Sensitivity

- **Training Ratio**: More training data (50-80%) improves equalizer quality but reduces test data
- **Equalizer Delay**: Must match channel pass-through tap position for proper alignment
- **ISI Power**: Higher ISI power (>0.2) creates more challenging equalization problem
- **SNR Impact**: 
  - At SNR < 10 dB: Noise dominates, equalization benefit limited
  - At SNR = 15-20 dB: Good equalization performance with 5-10% ISI
  - At SNR > 30 dB: Excellent performance, SER approaches zero

### Modulation Order Impact (Both Applications)

- **M**: Must be a power of 2 (2, 4, 8, 16, 32, 64)
  - M=2: BPSK (Binary PSK) - most robust
  - M=4: QPSK (Quadrature PSK) - good balance
  - M=8: 8-PSK - moderate spectral efficiency
  - M=16: 16-PSK - high spectral efficiency, requires higher SNR
- **Higher M**: Requires higher SNR for same error rate (constellation points closer together)

---

## Integration with AIE Designs

### Current Status

These are **standalone MATLAB models** independent of existing Cholesky/QRD AIE designs in:
- `cholesky/single_tile/`
- `cholesky/multi_tile/`
- `qrd/single_tile/`
- `qrd/multi_tile/`

### Future Integration Options

**Channel Estimation**:
1. **Test Data Generator**: Export **H**'·**H** and **H**'·**y** for AIE Cholesky verification
2. **Golden Reference**: Compare AIE channel estimation results vs. MATLAB ground truth
3. **Training Symbol Design**: Optimize transmitted sequences for better conditioning

**Linear Equalization**:
1. **Test Data Generator**: Export **Y**'·**Y** and **Y**'·**x** for AIE Cholesky verification
2. **Golden Reference**: Compare AIE equalizer weights vs. MATLAB ground truth
3. **End-to-End Pipeline**: AIE implementation of full communication system
   - M-PSK modulation → FIR channel → AWGN → equalizer training → symbol recovery
4. **Adaptive Equalization**: Use AIE for real-time equalizer update (LMS/RLS algorithms)

### Export Format (Both Applications)

Uncomment the data export section in either script to save matrices in PLIO format:
- `HtH_matrix.txt` / `YtY_matrix.txt`: 32×32 complex matrix
- `Hty_vector.txt` / `Ytx_vector.txt`: 32×1 complex vector

**Format**: PLIO-style with 2 complex samples per line (plio_64_bits):
```
real1 imag1 real2 imag2
...
```

---

## References

- **Vitis Solver Library Cholesky**: https://docs.amd.com/r/en-US/Vitis_Libraries/solver/user_guide/L2/func-cholesky.html
- **Least Squares Channel Estimation**: Standard adaptive filtering technique (Wiener filtering, RLS)
- **Linear Equalization**: Zero-forcing equalizer design via least squares
- **M-PSK Modulation**: Digital communication fundamentals (Proakis & Salehi)
- **Adaptive Filtering**: Haykin, "Adaptive Filter Theory"

---

## License

```
Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
Portions of this file consist of AI-generated content.
SPDX-License-Identifier: MIT

Author: Mark Rollins
```

---

## Revision History

| Date | Version | Description |
|------|---------|-------------|
| 2026-02-21 | 1.2 | Unified channel model across both applications (structured channel with controllable ISI) |
| 2026-02-21 | 1.1 | Added linear equalization model with structured channel and performance metrics |
| 2026-02-20 | 1.0 | Initial release with M-PSK channel estimation model |
