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

