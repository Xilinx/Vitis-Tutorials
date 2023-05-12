# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set CIPS [add_wave_group CIPS]
set pl_intf [get_objects -r $scope_path/CIPS_0/*M_AXI*FPD*tlm -filter {type==proto_inst}]
add_wave -into $CIPS $pl_intf

set NOISE [add_wave_group NOISE]
set pl_intf [get_objects -r $scope_path/random_noise_1/* -filter {type==proto_inst}]
add_wave -into $NOISE $pl_intf

set AIENGINE [add_wave_group AIENGINE]
set cips_intf [get_objects -r $scope_path/ai_engine_0/*S00*tlm -filter {type==proto_inst}]
add_wave -into $AIENGINE $cips_intf
