# Copyright 2020 Xilinx Inc.
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

if { $argc != 1 } {
    puts "Please provide the board type with -tclargs: VCK190 or VCK190_ES1"
    puts $argc
    exit
} 
if { [string equal -nocase VCK190_ES1 [lindex $argv 0]] } {
    # xhub::install [xhub::get_xitems *vck190_es:*]
    puts "Building example project for VCK190_ES1"
    set BOARD "vck190_es"
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

# The environment variable XILINX_XHUB_USERAREA_ROOT_DIR is responsible for redirecting downloaded board files to local_user_repo
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
xhub::install [xhub::get_xitems *ext_platform*]

# ------ Set Target Board ------
set my_board [get_board_parts *${BOARD}:part0* -latest_file_version]

# VCK190 Production Board
# set my_board [get_board_parts *vck190:part0* -latest_file_version]
# VCK190 ES1 Board
# set my_board [get_board_parts *vck190_es:part0* -latest_file_version]

# ------ Create Vivado Project ------
create_project project_1 ./project_1 -part [get_property PART_NAME [get_board_parts $my_board]] -force 
set_property board_part $my_board [current_project]
create_bd_design "ext_platform" -mode batch
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design ext_platform -options { Include_LPDDR.VALUE false}
# Other options are default
# - Three clocks
# - 32 interrupt

generate_target all [get_files ext_platform.bd]

update_compile_order -fileset sources_1

write_hw_platform -force ./vck190_custom_hw.xsa
write_hw_platform -hw_emu -force ./vck190_custom_hwemu.xsa


