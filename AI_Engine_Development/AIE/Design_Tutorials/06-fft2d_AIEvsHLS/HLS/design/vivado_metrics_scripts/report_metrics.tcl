#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


open_run impl_1
#Utilisation
report_utilization -file utilization_hierarchical.txt -hierarchical_depth 3 -hierarchical
report_utilization -file utilization_report.txt

#Power
report_power -file power_hierarchical.txt -hierarchical_depth 4 
report_power -file {power.txt} -xpe {power.xpe} -rpx {power.rpx}
