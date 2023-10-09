#=====================================================================
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
======================================================================#

using PyPlot
using DelimitedFiles

# specify workspace directory
ws_dir = pwd() * "/";

out_file = ws_dir * "Emulation-SW/x86simulator_output/output.dat";	# impulse response from AI engine simulation

# read the file
aie_out = readdlm(out_file);

# plot the difference
plot(aie_out);
grid("on");
title("AI Engine Output");
xlabel("Sampling Index");
ylabel("Amplitude");
