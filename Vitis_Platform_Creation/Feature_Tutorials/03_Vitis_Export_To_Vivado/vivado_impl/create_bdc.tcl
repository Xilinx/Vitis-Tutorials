# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
validate_bd_design

set curdesign [current_bd_design]
create_bd_design -cell [get_bd_cells /ExtensibleRegion] ExtensibleRegion
current_bd_design $curdesign
set new_cell [create_bd_cell -type container -reference ExtensibleRegion ExtensibleRegion_temp]
replace_bd_cell [get_bd_cells /ExtensibleRegion] $new_cell
delete_bd_objs  [get_bd_cells /ExtensibleRegion]
set_property name ExtensibleRegion $new_cell

current_bd_design [get_bd_designs ExtensibleRegion]
foreach intf [get_bd_intf_ports] {
	set_property HDL_ATTRIBUTE.LOCKED TRUE [get_bd_intf_ports $intf]
}
set_property PFM_NAME {xilinx.com:vck190:flat:0.5} [get_files ExtensibleRegion.bd]

validate_bd_design
save_bd_design

current_bd_design ${curdesign}
upgrade_bd_cells [get_bd_cells ExtensibleRegion]
validate_bd_design

