

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

create_clock -period 2.222 -name m_axis_clk -waveform {0.000 1.111} [get_ports m_axis_clk]
create_clock -period 10.000 -name s_axi_aclk -waveform {0.000 5.000} [get_ports s_axi_aclk]
