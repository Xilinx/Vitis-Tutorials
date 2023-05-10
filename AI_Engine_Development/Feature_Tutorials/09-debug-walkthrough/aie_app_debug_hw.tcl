# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

#Set the required variables
set vitis_install "/proj/xbuilds/2022.1_released/installs/lin64/Vitis/2022.1"
set aie_work_dir "${PROJECT_PATH}/Work"
set hw_server_host "${HOST NAME/IP}"
set app_name "aie"
 
#Printing the information
puts "Install: $vitis_install"
puts "Application: $app_name, Work Directory: $aie_work_dir"
puts "Hardware Server: $hw_server_host"
 
 
set source_tcl_cmd "source ${vitis_install}/scripts/vitis/util/aie_debug_init.tcl"
puts "$source_tcl_cmd"
eval $source_tcl_cmd
 
##run the command to connect and display debug targets
set aie_debug_cmd "init_aie_debug -work-dir $aie_work_dir -url tcp:$hw_server_host:3121 -jtag -name $app_name"
puts "$aie_debug_cmd"
eval $aie_debug_cmd

