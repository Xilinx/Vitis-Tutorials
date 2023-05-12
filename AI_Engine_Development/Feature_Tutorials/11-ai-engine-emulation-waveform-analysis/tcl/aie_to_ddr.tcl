# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set S2MM [add_wave_group S2MM]
set s2mm_intf [get_objects -r $scope_path/s2mm_1/m_axi_gmem -filter {type==proto_inst}]
add_wave -into $S2MM $s2mm_intf

set s2mm_intf [get_objects -r $scope_path/s2mm_1/s -filter {type==proto_inst}]
add_wave -into $S2MM $s2mm_intf

set DDR4 [add_wave_group DDR4]
set ddr4_intf [get_objects -r $scope_path/noc_ddr4/S00_AXI -filter {type==proto_inst}]
add_wave -into $DDR4 $ddr4_intf

set AIENGINE [add_wave_group AIENGINE]
set aie_intf [get_objects -r $scope_path/ai_engine_0/M00_AXIS -filter {type==proto_inst}]
add_wave -into $AIENGINE $aie_intf

set CIPS_NOC [add_wave_group CIPS_NOC]
set cips_intf [get_objects -r $scope_path/cips_noc/*S00_AXI*tlm -filter {type==proto_inst}]
add_wave -into $CIPS_NOC $cips_intf

set cips_intf [get_objects -r $scope_path/cips_noc/*M00_AXI*tlm -filter {type==proto_inst}]
add_wave -into $CIPS_NOC $cips_intf