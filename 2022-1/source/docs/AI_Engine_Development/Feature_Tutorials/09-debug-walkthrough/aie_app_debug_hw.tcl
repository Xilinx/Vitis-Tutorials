#
# Copyright 2022 Xilinx, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://www.apache.org/licenses/LICENSE-2.0
#     Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
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

