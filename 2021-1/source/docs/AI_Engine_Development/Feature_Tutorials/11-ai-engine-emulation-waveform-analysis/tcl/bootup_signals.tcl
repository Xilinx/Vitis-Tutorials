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

# Removes all waves from the viewer
remove_wave -of [get_wave_config] [get_waves -of [get_wave_config] *]

set cips_intf [get_objects -r $scope_path/CIPS_0/*PMC*tlm -filter {type==proto_inst}]
add_wave $cips_intf

set cips_intf [get_objects -r $scope_path/cips_noc/*S07*tlm -filter {type==proto_inst}]
add_wave $cips_intf

set cips_intf [get_objects -r $scope_path/ai_engine_0/*S00*tlm -filter {type==proto_inst}]
add_wave $cips_intf