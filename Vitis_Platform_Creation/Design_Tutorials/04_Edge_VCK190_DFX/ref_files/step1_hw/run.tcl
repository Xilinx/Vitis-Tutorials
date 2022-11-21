#*****************************************************************************************
# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#*****************************************************************************************
if { $argc != 1 } {
    puts "Please provide the board type with -tclargs: VCK190 or Other"
    puts $argc
    exit
} 
if { [string equal -nocase VCK190 [lindex $argv 0]] } {
    # xhub::install [xhub::get_xitems *vck190_es:*]
    puts "Building example project for VCK190"
    set BOARD "vck190"
} else {
    # xhub::install [xhub::get_xitems *vck190:*]
    puts "Valide argument is VCK190 or VCK190_ES1"
    exit
}

if {[file exists local_user_repo]} {
               file delete -force local_user_repo
               }

#unset ::env(HTTP_PROXY) 
file mkdir local_user_repo
set ::env(XILINX_XHUB_USERAREA_ROOT_DIR) local_user_repo

xhub::get_xstores

# ------ Get VCK190 or VCK190 ES1 board files ------
xhub::refresh_catalog [xhub::get_xstores xilinx_board_store]
get_property LOCAL_ROOT_DIR [xhub::get_xstores xilinx_board_store]
set_param board.repoPaths [get_property LOCAL_ROOT_DIR [xhub::get_xstores xilinx_board_store]]


#xhub::install [xhub::get_xitems *vck190:*]
xhub::install [xhub::get_xitems *${BOARD}:*]


xhub::refresh_catalog [xhub::get_xstores Vivado_example_project]
get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]
set_param ced.repoPaths [get_property LOCAL_ROOT_DIR [xhub::get_xstores Vivado_example_project]]
xhub::install [xhub::get_xitems *versal_dfx_platform*]

# ------ Set Target Board ------
set my_board [get_board_parts *${BOARD}:part0* -latest_file_version]

create_project project_1 ./project_1 -part xcvc1902-vsva2197-2MP-e-S -force 
set_property board_part $my_board [current_project]
create_bd_design "versal_dfx_platform" -mode batch
instantiate_example_design -template xilinx.com:design:versal_dfx_platform:1.0 -design versal_dfx_platform -options { Clock_Options.VALUE {clk_out1 156.25 0 true clk_out2 104.167 1 false} Include_AIE.VALUE true Include_DDR.VALUE true}
update_compile_order -fileset sources_1


#run implementtation
launch_runs impl_1 -to_step write_device_image -jobs 10
wait_on_run impl_1
open_run impl_1


write_hw_platform -force -fixed -static -file vck190_custom_dfx_static.xsa
write_hw_platform  -force -rp versal_dfx_platform_i/VitisRegion vck190_custom_dfx_rp.xsa
#set_property platform.platform_state "pre_synth" [current_project]
# Default Value
#write_hw_platform -hw_emu -force -file vck190_custom_dfx_hw_emu.xsa
set_property platform.platform_state "pre_synth" [current_project] 
# Default Value
write_hw_platform -hw_emu -force -file vck190_custom_dfx_hw_emu.xsa
