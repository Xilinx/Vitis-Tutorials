##
##Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
##SPDX-License-Identifier: MIT
##
startgroup
set_property CONFIG.NUM_SI {0} [get_bd_cells LPDDRNoc]
set_property CONFIG.NUM_SI {0} [get_bd_cells DDRNoc]
set_property PFM.CLOCK {clk_out1 {id "0" is_default "true" proc_sys_reset "/psr_104mhz" status "fixed" freq_hz "104166666"} clk_out2 {id "1" is_default "false" proc_sys_reset "/psr_156mhz" status "fixed" freq_hz "156250000"} clk_out3 {id "2" is_default "true" proc_sys_reset "/psr_312mhz" status "fixed" freq_hz "312500000"} clk_out4 {id "3" is_default "false" proc_sys_reset "/psr_78mhz" status "fixed" freq_hz "78125000"} clk_out5 {id "4" is_default "false" proc_sys_reset "/psr_208mhz" status "fixed" freq_hz "208333333"} clk_out6 {id "5" is_default "false" proc_sys_reset "/psr_416mhz" status "fixed" freq_hz "416666666"} clk_out7 {id "6" is_default "false" proc_sys_reset "/psr_625mhz" status "fixed" freq_hz "625000000"}} [get_bd_cells /clk_wizard_0]
set_property PFM.CLOCK {clk_out1 {id "0" is_default "true" proc_sys_reset "/psr_104mhz" status "fixed" freq_hz "104166666"} clk_out2 {id "1" is_default "false" proc_sys_reset "/psr_156mhz" status "fixed" freq_hz "156250000"} clk_out3 {id "2" is_default "false" proc_sys_reset "/psr_312mhz" status "fixed" freq_hz "312500000"} clk_out4 {id "3" is_default "false" proc_sys_reset "/psr_78mhz" status "fixed" freq_hz "78125000"} clk_out5 {id "4" is_default "false" proc_sys_reset "/psr_208mhz" status "fixed" freq_hz "208333333"} clk_out6 {id "5" is_default "false" proc_sys_reset "/psr_416mhz" status "fixed" freq_hz "416666666"} clk_out7 {id "6" is_default "false" proc_sys_reset "/psr_625mhz" status "fixed" freq_hz "625000000"}} [get_bd_cells /clk_wizard_0]
create_bd_cell -type ip -vlnv xilinx.com:user:stream_out:1.0 stream_out_0
create_bd_cell -type ip -vlnv xilinx.com:user:stream_in:1.0 stream_in_0
set_property CONFIG.NUM_MI {7} [get_bd_cells axi_smc_vip_hier/icn_ctrl]
endgroup

connect_bd_net [get_bd_pins stream_out_0/ap_clk] [get_bd_pins clk_wizard_0/clk_out1]
connect_bd_net [get_bd_pins stream_in_0/ap_clk] [get_bd_pins clk_wizard_0/clk_out1]
connect_bd_net [get_bd_pins stream_out_0/ap_rst_n] [get_bd_pins psr_104mhz/peripheral_aresetn]
connect_bd_net [get_bd_pins stream_in_0/ap_rst_n] [get_bd_pins psr_104mhz/peripheral_aresetn]
connect_bd_intf_net [get_bd_intf_pins stream_in_0/s_axi_control] [get_bd_intf_pins axi_smc_vip_hier/icn_ctrl/M06_AXI]

assign_bd_address -target_address_space /S00_AXI [get_bd_addr_segs stream_in_0/s_axi_control/Reg] -force

validate_bd_design
save_bd_design
