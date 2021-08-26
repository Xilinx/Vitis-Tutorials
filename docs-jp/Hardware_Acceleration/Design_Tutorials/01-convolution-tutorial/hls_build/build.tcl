open_project -reset conv_filter_prj
set_top Filter2DKernel
add_files -tb ../src/hls_testbench.cpp  -cflags "-I../src"
add_files -tb ../src/cmdlineparser.cpp  -cflags "-I../src"
add_files -tb ../src/filter2d_sw.cpp    -cflags "-I../src"
add_files     ../src/filter2d_hw.cpp    -cflags "-I../src"
open_solution "solution"
set_part {xcu200-fsgd2104-2-e}
create_clock -period 3.33333333 -name default
config_flow -target vitis
csim_design
csynth_design
cosim_design -trace_level all -enable_dataflow_profiling
exit
