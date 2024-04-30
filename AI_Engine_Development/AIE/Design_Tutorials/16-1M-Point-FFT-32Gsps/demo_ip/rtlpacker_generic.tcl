#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

set_param messaging.defaultLimit 1000
set PARTNUM [lindex $argv 0]
set XONAME  [lindex $argv 1]
set IPNAME  [lindex $argv 2]
set CLKDAT  [lindex $argv 3]
set CLKREG  [lindex $argv 4]
set ADDMEM  [lindex $argv 5]
set ADDPSW  [lindex $argv 6]

create_project -force ip_${IPNAME} ./ip_${IPNAME} -part ${PARTNUM}

add_files ./rtl/${IPNAME}

if { ${ADDMEM} == 1 } { add_files [glob ./rtl/mem/*.mem] }
if { ${ADDPSW} == 1 } { add_files ./rtl/pktid_constants.vh}

import_files -force -norecurse

set_property top ${IPNAME} [current_fileset]

update_compile_order -fileset sources_1

ipx::package_project -root_dir ./ip_repo/${IPNAME} -vendor user.org -library user -taxonomy /UserIP -generated_files -import_files

if { ${CLKDAT} == 1} {set_property name dat_clk [ipx::get_bus_interfaces datclk -of_objects [ipx::current_core]]}
if { ${CLKREG} == 1} {set_property name reg_clk [ipx::get_bus_interfaces regclk -of_objects [ipx::current_core]]}

set_property core_revision 1 [ipx::current_core]
ipx::create_xgui_files [ipx::current_core]
ipx::update_checksums [ipx::current_core]
ipx::save_core [ipx::current_core]

set_property  ip_repo_paths  ./ip_repo/${IPNAME} [current_project]
update_ip_catalog

package_xo -xo_path ${XONAME}.xo -kernel_name ${IPNAME} -ctrl_protocol "user_managed"  -ip_directory ./ip_repo/${IPNAME}

