#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


open_run impl_1
#Utilisation
report_utilization -file utilization_hierarchical.txt -hierarchical_depth 2 -hierarchical
report_utilization -file utilization_report.txt

#Power
report_power -file power_hierarchical.txt -hierarchical_depth 3 
report_power -file {power.txt} -xpe {power.xpe} -rpx {power.rpx}
