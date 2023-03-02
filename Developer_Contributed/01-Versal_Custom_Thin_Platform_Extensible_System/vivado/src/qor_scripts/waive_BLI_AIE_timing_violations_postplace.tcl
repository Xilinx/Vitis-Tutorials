# © Copyright 2020 Xilinx, Inc.
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

# Last update; 2019/09/12
# Last change: added conditions to prevent ERRORs when no BLI or AIE_PL cells are found after place_design
#

set bliFD [get_cells -quiet -hier -filter "REF_NAME=~FD* && LOC=~BLI_*"]
if {$bliFD == {}} { puts "Warning - No registers placed on BLI sites found" }

set aiePL [get_cells -quiet -hier -filter "REF_NAME=~AIE_PL_*"]
if {$aiePL == {}} { puts "Warning - No AIE_PL_* cells found" }

if {$bliFD != {} && $aiePL != {}} {
  set_property IS_LOC_FIXED 1 $bliFD
  set_false_path -from $bliFD -to $aiePL
  set_false_path -from $aiePL -to $bliFD
}

