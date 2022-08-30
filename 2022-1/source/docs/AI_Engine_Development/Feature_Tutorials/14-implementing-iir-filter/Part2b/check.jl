#=
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Copyright 2022 Xilinx, Inc.</sup></p>
=#

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
ref = vec(readdlm(ref_file));
dut = vec(readdlm(dut_file));

ref_size = size(ref, 1);
dut_size = size(dut, 1);

@printf("ref_size = %d\n", ref_size);
@printf("dut_size = %d\n", dut_size);
@printf("\n");

resize!(ref, dut_size);  # match dimensions

err = ref - dut;	# calculate the difference

@printf("eps(Float32) = %e\n", eps(Float32));
@printf("maximum(abs.(err)) = %e\n", maximum(abs.(err)));
@printf("\n");
