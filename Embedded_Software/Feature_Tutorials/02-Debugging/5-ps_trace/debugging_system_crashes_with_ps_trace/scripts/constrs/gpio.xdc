# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#*****************************************************************************************
set_clock_uncertainty -hold  0.2 -from [get_clocks *pl_0] -to [get_clocks *pl_0]
#set_clock_uncertainty -setup  3.0 -from [get_clocks *pl_0] -to [get_clocks *pl_0]

#set_clock_uncertainty -hold  0.2 -from [get_clocks *pl_1] -to [get_clocks *pl_1]
##set_clock_uncertainty -setup  3.0 -from [get_clocks *pl_1] -to [get_clocks *pl_1]
#
#set_clock_uncertainty -hold  0.2 -from [get_clocks *pl_2] -to [get_clocks *pl_2]
##set_clock_uncertainty -setup  3.0 -from [get_clocks *pl_2] -to [get_clocks *pl_2]
#
#set_clock_uncertainty -hold  0.2 -from [get_clocks *pl_3] -to [get_clocks *pl_3]
##set_clock_uncertainty -setup  3.0 -from [get_clocks *pl_3] -to [get_clocks *pl_3]
#

# Misc
#GPIO_LED_0_LS
set_property PACKAGE_PIN H34 [get_ports Dout_0]
set_property IOSTANDARD LVCMOS18 [get_ports {Dout_0}]
#GPIO_LED_1_LS
set_property PACKAGE_PIN J33 [get_ports Dout_1]
set_property IOSTANDARD LVCMOS18 [get_ports {Dout_1}]
#GPIO_LED_2_LS
set_property PACKAGE_PIN K36 [get_ports Dout_2]
set_property IOSTANDARD LVCMOS18 [get_ports {Dout_2}]
#GPIO_LED_3_LS
set_property PACKAGE_PIN L35 [get_ports Dout_3]
set_property IOSTANDARD LVCMOS18 [get_ports {Dout_3}]

#set_property PACKAGE_PIN L33 [get_ports uart1_txd_0]
#set_property PACKAGE_PIN K33 [get_ports uart1_rxd_0]

#set_property IOSTANDARD LVCMOS18 [get_ports uart1_*]