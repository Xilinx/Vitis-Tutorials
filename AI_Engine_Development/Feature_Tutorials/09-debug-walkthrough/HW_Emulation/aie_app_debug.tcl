#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

set aie_work_dir "${PROJECT_PATH}/cmd_src/Work"
set aie_mem_sock_port "10100"
set xrt_server_port "4352"
set app_name "aie_graph"

#Echo the environment setup
puts "Vitis install: ${XILINX_VITIS_PATH}/Xilinx/Vitis/2023.1/scripts/vitis/util/aie_debug_init.tcl"
puts "Application: $app_name, Work Directory: $aie_work_dir"
puts "XRT Server Port: $xrt_server_port, AIE Port: $aie_mem_sock_port"

#Set up AIE Debug environment
set source_tcl_cmd "source ${XILINX_VITIS_PATH}/Xilinx/Vitis/2023.1/scripts/vitis/util/aie_debug_init.tcl"
puts "$source_tcl_cmd"
eval $source_tcl_cmd

##run the command to connect and display debug targets
set aie_debug_cmd "init_aie_debug -work-dir $aie_work_dir -url tcp::$xrt_server_port \
-memsock-url localhost:$aie_mem_sock_port -sim-type memserver -name $app_name -full-program"
puts "$aie_debug_cmd"
eval $aie_debug_cmd
