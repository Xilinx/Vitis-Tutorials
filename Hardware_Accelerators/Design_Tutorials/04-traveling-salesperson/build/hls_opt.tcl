# Project setup
#
open_project -reset proj_opt
set_top tsp
add_files ../code/tsp_opt.cpp
add_files -tb ../code/tsp_TB.cpp

# Solution commands
#
open_solution "solution1" -flow_target vivado
set_part {xcvu9p-flga2104-2-i}
create_clock -period 3.0 -name default
set_clock_uncertainty 0.5
config_export -format ip_catalog -rtl verilog

# Main flow commands
#
csim_design
csynth_design
cosim_design
export_design -flow impl -rtl verilog -format syn_dcp
