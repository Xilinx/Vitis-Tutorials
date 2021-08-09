#
# © Copyright 2021 Xilinx, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Remove all waveforms before adding new ones
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] -regexp ".*"]

# Set the appropriate paths based upon the platform being used
set scope_path "/xilinx_vck190_base_wrapper_sim_wrapper/xilinx_vck190_base_wrapper_i/xilinx_vck190_base_i"

# Create a wave group called CIPS and add all signals for the CIPS_0 to it
set CIPS [add_wave_group CIPS]
set cips_intf [get_objects -r $scope_path/CIPS_0/* -filter {type==proto_inst}]
add_wave -into $CIPS $cips_intf

# Create a wave group called NOISE and add all signals of the random_noise_1 to it
set NOISE [add_wave_group NOISE]
set noise_intf [get_objects -r $scope_path/random_noise_1/* -filter {type==proto_inst}]
add_wave -into $NOISE $noise_intf

# Create a wave group called S2MM and add all signals of the S2MM kernel to it
set S2MM [add_wave_group S2MM]
set s2mm_intf [get_objects -r $scope_path/s2mm_1/* -filter {type==proto_inst}]
add_wave -into $S2MM $s2mm_intf

# Create a wave group called CIPS_NOC and all signals of the CIPS NoC to it
set CIPS_NOC [add_wave_group CIPS_NOC]
set cips_intf [get_objects -r $scope_path/cips_noc/* -filter {type==proto_inst}]
add_wave -into $CIPS_NOC $cips_intf

# Create a wave group called DDR4 and all signals to/from DDR4
set DDR4 [add_wave_group DDR4]
set ddr4_intf [get_objects -r $scope_path/noc_ddr4/* -filter {type==proto_inst}]
add_wave -into $DDR4 $ddr4_intf

# Create a wave group called AIENGINE and all signals of the AI Engine block to it
set AIENGINE [add_wave_group AIENGINE]
set aie_intf [get_objects -r $scope_path/ai_engine_0/* -filter {type==proto_inst}]
add_wave -into $AIENGINE $aie_intf
