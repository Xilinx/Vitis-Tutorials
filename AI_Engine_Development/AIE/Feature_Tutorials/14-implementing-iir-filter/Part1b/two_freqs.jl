#=====================================================================
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
======================================================================#

# Ref: https://docs.juliadsp.org/latest/filters/#Filters-filter-design-and-filtering

# coefficient matrix written out as 1d array

using DSP
using PyPlot
using DoubleFloats
using Printf

using DSP
using FFTW
using Noise

# --- begin user parameters

f1 = 2.0e6              # frequency of s1
f2 = 18.0e6             # frequency of s2
fs = 100.0e6            # sampling frequency
N = 256                 # no. of samples for impulse response
a1 = 0.5                # amplitude of s1
a2 = 0.5                # amplitude of s2

# --- end user parameters

n = 0 : (N - 1)         # sampling index
Ts = 1/fs               # sampling period

s1 = a1 * sin.(2 * pi * f1 * n * Ts)
s2 = a2 * sin.(2 * pi * f2 * n * Ts)
s = add_gauss(s1 + s2, 0.2)

xfactor = 16
s_pad = zeros(N * xfactor)
s_pad[1:N] = s          # zero padding for FFT

win = kaiser(N * xfactor, 3, padding=0, zerophase=false)

S = (fft(win .* s_pad)) |> fftshift
freqs = fftfreq(length(s_pad), fs) |> fftshift
PS = 20*log10.(abs.(S)) # power spectrum

#close("all")

fig1, ax1 = subplots()
ax1.plot(n, s)
ax1.grid("on")
ax1.set_title("Noisy Input Signal")
ax1.set_xlabel("Sampling Index")
ax1.set_ylabel("Amplitude")
ax1.set_xlim([0, N])
ax1.set_ylim([-1.5, 1.5])

fig2, ax2 = subplots()
ax2.plot(freqs[Int32(N * xfactor/2) : end], PS[Int32(N * xfactor/2) : end])
ax2.grid("on")
ax2.set_title("Power Spectrum")
ax2.set_xlabel("Frequency (Hz)")
ax2.set_ylabel("Amplitude (dB)")
ax2.set_xlim([0, fs/2])
ax2.set_ylim([-60, 0])

fid = open("two_freqs.dat", "w")
for i = 1 : N
    @printf(fid, "%f\n", s[i])
end # for i = 1 : N
close(fid)
