#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

set cdc_list [get_bd_cells -hierarchical "*cdc*"]
foreach i $cdc_list {
	set_property -dict [list CONFIG.IS_ACLK_ASYNC {1} CONFIG.ACLK_RATIO {1:2}] [get_bd_cells $i]
}
