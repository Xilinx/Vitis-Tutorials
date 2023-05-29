# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

#Set the required variables
set vitis_install "${XILINX_VITIS}"
set aie_work_dir "${PROJECT_DIR}/Work"
set aie_mem_sock_port "10100"
set xrt_server_port "4352"
set app_name "aie_debug"
 
#Printing the information
puts "Install: $vitis_install"
puts "Application: $app_name, Work Directory: $aie_work_dir"
puts "XRT Server Port: $xrt_server_port, AIE Port: $aie_mem_sock_port"
  
   
set source_tcl_cmd "source ${vitis_install}/scripts/vitis/util/aie_debug_init.tcl"
puts "$source_tcl_cmd"
eval $source_tcl_cmd
  
##run the command to connect and display debug targets
set aie_debug_cmd "init_aie_debug -work-dir $aie_work_dir -url tcp::$xrt_server_port -memsock-url localhost:$aie_mem_sock_port -sim-type memserver -name $app_name -full-program"
puts "$aie_debug_cmd"
eval $aie_debug_cmd

