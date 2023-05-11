# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */


# get the part and board information from command line argvs
set_part [lindex $argv 0]
set board [lindex $argv 1]

# ----------------------------------------------------------------------------
# generate Ethernet System IP of one channel
# ----------------------------------------------------------------------------
create_ip -name xxv_ethernet \
          -vendor xilinx.com \
          -library ip  \
          -version 4.* \
          -module_name xxv_ethernet_0 \
          -dir ./ip_generation

set_property -dict [list CONFIG.LINE_RATE {10} \
                         CONFIG.BASE_R_KR {BASE-R} \
                         CONFIG.INCLUDE_AXI4_INTERFACE {0} \
                         CONFIG.ENABLE_PIPELINE_REG {1}] \
             [get_ips xxv_ethernet_0]

if {[string compare -nocase $board "u200"] == 0} {
# U200 Valid Quad_X1Y11[44:47], Quad_X1Y12[48:51]
    set_property -dict [list CONFIG.GT_REF_CLK_FREQ {161.1328125}  \
                             CONFIG.GT_GROUP_SELECT {Quad_X1Y12} \
                             CONFIG.LANE1_GT_LOC {X1Y48}] \
                 [get_ips xxv_ethernet_0]
} elseif {[string compare -nocase $board "u250"] == 0} {
# U250 Valid Quad_X1Y10[40:43], Quad_X1Y11[44:48]
    set_property -dict [list CONFIG.GT_REF_CLK_FREQ {161.1328125} \
                             CONFIG.GT_GROUP_SELECT {Quad_X1Y10} \
                             CONFIG.LANE1_GT_LOC {X1Y40}] \
                 [get_ips xxv_ethernet_0]
} elseif {[string compare -nocase $board "u280"] == 0} {
# U280 Valid Quad_X1Y10[40:43], Quad_X1Y11[44:48]
    set_property -dict [list CONFIG.GT_REF_CLK_FREQ {156.25} \
                             CONFIG.GT_DRP_CLK {50} \
                             CONFIG.GT_GROUP_SELECT {Quad_X0Y10} \
                             CONFIG.LANE1_GT_LOC {X0Y40}] \
                 [get_ips xxv_ethernet_0]	
} elseif {[string compare -nocase $board "u50"] == 0} {
# U50 Valid Quad_X0Y7[28:31]
    set_property -dict [list CONFIG.GT_REF_CLK_FREQ {161.1328125} \
                             CONFIG.GT_GROUP_SELECT {Quad_X0Y7} \
                             CONFIG.LANE1_GT_LOC {X0Y28}] \
                 [get_ips xxv_ethernet_0]
} else {
    puts "Unknown board $board"
    exit
}


# ----------------------------------------------------------------------------
# generate AXI stream data FIFO IP
# ----------------------------------------------------------------------------
create_ip -name axis_data_fifo \
          -vendor xilinx.com \
          -library ip \
          -version 2.0 \
          -module_name axis_data_fifo_0 \
          -dir ./ip_generation

set_property -dict [list CONFIG.IS_ACLK_ASYNC {1} \
                         CONFIG.TDATA_NUM_BYTES {8} \
                         CONFIG.TUSER_WIDTH {1} \
                         CONFIG.FIFO_MODE {2} \
                         CONFIG.HAS_TSTRB {0} \
                         CONFIG.HAS_TKEEP {1} \
                         CONFIG.HAS_TLAST {1}] \
             [get_ips axis_data_fifo_0]


