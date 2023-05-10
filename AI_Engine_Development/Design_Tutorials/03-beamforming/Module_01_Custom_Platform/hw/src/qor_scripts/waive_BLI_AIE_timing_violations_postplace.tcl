

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

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

