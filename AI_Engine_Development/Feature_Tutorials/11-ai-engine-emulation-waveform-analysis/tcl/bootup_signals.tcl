# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set cips_intf [get_objects -r $scope_path/CIPS_0/*PMC*tlm -filter {type==proto_inst}]
add_wave $cips_intf

set cips_intf [get_objects -r $scope_path/cips_noc/*S07*tlm -filter {type==proto_inst}]
add_wave $cips_intf

set cips_intf [get_objects -r $scope_path/ai_engine_0/*S00*tlm -filter {type==proto_inst}]
add_wave $cips_intf
