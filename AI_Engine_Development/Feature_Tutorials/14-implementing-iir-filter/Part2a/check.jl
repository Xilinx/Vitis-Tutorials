#=====================================================================
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
======================================================================#

using Printf
using DelimitedFiles

function show_usage()
    @printf("\nUsage:\n");
    @printf("  \$ julia check.jl {mode}\n\n");
    @printf("where {mode} may be:\n\n");
    @printf("  sw: check Emulation-SW results\n");
    @printf("  aie: check Emulation-AIE results\n\n");
end # show_usage()

@printf("\nComparing reference and calculated impulse responses...\n\n");

num_args = size(ARGS, 1);   # number of arguments
if (num_args != 1)
    show_usage();
    exit();
end

mode = ARGS[1];
if (!(mode == "sw" || mode == "aie"))
    @printf("\nUnrecognized mode \"%s\".\n", mode);
    show_usage();
    exit();
end

# workspace directory is current directory
ws_dir = pwd() * "/";

ref_file = ws_dir * "data/impresponse.dat";	# impulse response from Julia

if (mode == "aie")

    cp_cmd = `cp Emulation-AIE/aiesimulator_output/output.dat Emulation-AIE/aiesimulator_output/output.dat.mod`;
    sed_cmd = `sed -i '/T/d' Emulation-AIE/aiesimulator_output/output.dat.mod`; # remove lines which begin with "T"

    run(cp_cmd);
    run(sed_cmd);

    dut_file = ws_dir * "Emulation-AIE/aiesimulator_output/output.dat.mod";	# impulse response from AI engine simulation

else

    dut_file = ws_dir * "Emulation-Sw/x86simulator_output/output.dat";  # impulse response from SW simulation

end

# read the files into variables
ref = readdlm(ref_file);
dut = readdlm(dut_file);

err = ref - dut;	# calculate the difference

@printf("eps(Float32) = %e\n", eps(Float32));
@printf("maximum(abs.(err)) = %e\n", maximum(abs.(err)));
@printf("\n");
