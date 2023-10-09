#=====================================================================
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
======================================================================#

using Printf
using PyPlot
using DelimitedFiles

# specify workspace directory
ws_dir = pwd() * "/";


ref_file = ws_dir * "data/impresponse_a.dat";				# impulse response from Julia
dut_file = ws_dir * "Emulation-SW/x86simulator_output/output.dat";	# impulse response from AI engine simulation

# read the files into variables
ref = readdlm(ref_file);
dut = readdlm(dut_file);

err = ref - dut;	# calculate the difference

# plot the difference
plot(err);
grid("on");
title("Impulse Response Error");
xlabel("Sampling Index");
ylabel("Error");

@printf("eps(Float32) = %e\n", eps(Float32));
@printf("maximum(abs.(err) = %e\n", maximum(abs.(err)));
