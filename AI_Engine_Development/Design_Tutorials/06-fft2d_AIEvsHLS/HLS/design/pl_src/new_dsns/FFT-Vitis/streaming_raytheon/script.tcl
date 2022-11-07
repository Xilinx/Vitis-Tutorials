open_project project 

add_files fft.cpp 
#add_files fft_rv1.cpp
add_files -tb fft_tb.cpp 

#set_top fft_top
set_top myfft

open_solution solution 

#set_part zynq
#set_part {xc7z020-clg484-2}
set_part {xqzu9eg-ffrc900-2-i}

create_clock -period 10
config_dataflow -start_fifo_depth 16 -scalar_fifo_depth 16 -task_level_fifo_depth 16
csim_design
csynth_design

#cosim_design -rtl vhdl
#cosim_design 
#cosim_design -wave_debug -trace_level all
#export_design -flow impl
