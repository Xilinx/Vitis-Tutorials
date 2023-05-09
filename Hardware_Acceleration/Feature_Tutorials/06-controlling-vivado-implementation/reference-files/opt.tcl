#/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#*/

#DCP files in different stages of Vivado have been written by v++ linker automatically with option "-R2"
open_checkpoint ./_x/link/vivado/vpl/prj/prj.runs/impl_1/pfm_top_wrapper_routed.dcp
#Run post-route physical optimization
phys_opt_design -directive AggressiveExplore
write_checkpoint -force ./_x/link/vivado/routed.dcp
