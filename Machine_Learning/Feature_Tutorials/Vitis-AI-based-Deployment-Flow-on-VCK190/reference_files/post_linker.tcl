# /*******************************************************************************
# /*                                                                         
# * Copyright 2019 Xilinx Inc.                                               
# *                                                                          
# * Licensed under the Apache License, Version 2.0 (the "License");          
# * you may not use this file except in compliance with the License.         
# * You may obtain a copy of the License at                                  
# *                                                                          
# *    http://www.apache.org/licenses/LICENSE-2.0                            
# *                                                                          
# * Unless required by applicable law or agreed to in writing, software      
# * distributed under the License is distributed on an "AS IS" BASIS,        
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# * See the License for the specific language governing permissions and      
# * limitations under the License.                                           
# */
# *******************************************************************************/

# Setup AIE frequency
set_property -dict [list CONFIG.AIE_CORE_REF_CTRL_FREQMHZ {1333}] [get_bd_cells ai_engine_0]

# Setup new NoC connection
startgroup
set_property -dict [list CONFIG.NUM_SI {27} CONFIG.NUM_CLKS {13} CONFIG.NUM_MC {1} CONFIG.MC_XPLL_CLKOUT1_PHASE {210.41257367387036} CONFIG.MC_XPLL_CLKOUT1_PERIOD {1018} CONFIG.MC_MEMORY_TIMEPERIOD0 {509} CONFIG.MC_MEMORY_TIMEPERIOD1 {509} CONFIG.MC_INPUT_FREQUENCY0 {201.501} CONFIG.MC_INPUTCLK0_PERIOD {4963} CONFIG.MC_MEMORY_SPEEDGRADE {LPDDR4-4267} CONFIG.MC_TCKEMIN {15} CONFIG.MC_TCKE {15} CONFIG.MC_TFAW {30000} CONFIG.MC_TRTP_nCK {16} CONFIG.MC_TXPMIN {15} CONFIG.MC_TXP {15} CONFIG.MC_TZQLAT_div4 {15} CONFIG.MC_TZQLAT_nCK {59} CONFIG.MC_TRRD {7500} CONFIG.MC_CASLATENCY {36} CONFIG.MC_CASWRITELATENCY {18} CONFIG.MC_ADDR_BIT9 {CA7} CONFIG.MC_TFAWMIN {30000} CONFIG.MC_TZQCAL_div4 {492} CONFIG.MC_ECC_SCRUB_PERIOD {0x004CBF} CONFIG.MC_ODTLon {8} CONFIG.MC_F1_CASLATENCY {36} CONFIG.MC_F1_CASWRITELATENCY {18} CONFIG.MC_F1_TFAW {30000} CONFIG.MC_F1_TFAWMIN {30000} CONFIG.MC_F1_TRRD {7500} CONFIG.MC_F1_TRRDMIN {7500} CONFIG.MC_F1_LPDDR4_MR1 {0x000} CONFIG.MC_F1_LPDDR4_MR2 {0x000} CONFIG.MC_F1_LPDDR4_MR3 {0x000} CONFIG.MC_F1_LPDDR4_MR13 {0x0C0} CONFIG.MC_WRITE_BANDWIDTH {7858.546} CONFIG.MC_DDR_INIT_TIMEOUT {0x00035ECC}] [get_bd_cells NOC_0]
set_property -dict [list CONFIG.CONNECTIONS {MC_0 { read_bw {64} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S15_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_1 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S16_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_2 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S17_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_3 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S18_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_0 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S19_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_1 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S20_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_2 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S21_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_3 { read_bw {6000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} } CONFIG.R_TRAFFIC_CLASS {LOW_LATENCY}] [get_bd_intf_pins /NOC_0/S22_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_0 { read_bw {6000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} } CONFIG.R_TRAFFIC_CLASS {LOW_LATENCY}] [get_bd_intf_pins /NOC_0/S23_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_1 { read_bw {6000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} } CONFIG.R_TRAFFIC_CLASS {LOW_LATENCY}] [get_bd_intf_pins /NOC_0/S24_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_2 { read_bw {6000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} } CONFIG.R_TRAFFIC_CLASS {LOW_LATENCY}] [get_bd_intf_pins /NOC_0/S25_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_3 { read_bw {1000} write_bw {32} read_avg_burst {4} write_avg_burst {4}} }] [get_bd_intf_pins /NOC_0/S26_AXI]
set_property -dict [list CONFIG.ASSOCIATED_BUSIF {S15_AXI:S16_AXI:S17_AXI:S18_AXI:S19_AXI:S20_AXI:S21_AXI:S22_AXI:S23_AXI:S24_AXI:S25_AXI:S26_AXI}] [get_bd_pins /NOC_0/aclk12]
endgroup

#Delete default AXI's connection
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m00_instr_axi] [get_bd_intf_nets DPUCVDX8G_m00_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S15_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S15_AXI]
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m00_wgt_axi] [get_bd_intf_nets DPUCVDX8G_m01_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S16_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S16_AXI]
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m01_wgt_axi] [get_bd_intf_nets DPUCVDX8G_m02_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S17_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S17_AXI]
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m02_wgt_axi] [get_bd_intf_nets DPUCVDX8G_m03_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S18_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S18_AXI]
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m03_wgt_axi] [get_bd_intf_nets DPUCVDX8G_m04_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S19_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S19_AXI]
delete_bd_objs [get_bd_intf_nets DPUCVDX8G_m00_bias_axi] [get_bd_intf_nets DPUCVDX8G_m05_img_axi] [get_bd_intf_nets axi_ic_NOC_0_S20_AXI_M00_AXI] [get_bd_cells axi_ic_NOC_0_S20_AXI]

#Create AXI's connection
startgroup
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m00_instr_axi] [get_bd_intf_pins NOC_0/S15_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m00_img_axi] [get_bd_intf_pins NOC_0/S16_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m01_img_axi] [get_bd_intf_pins NOC_0/S17_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m02_img_axi] [get_bd_intf_pins NOC_0/S18_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m03_img_axi] [get_bd_intf_pins NOC_0/S19_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m04_img_axi] [get_bd_intf_pins NOC_0/S20_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m05_img_axi] [get_bd_intf_pins NOC_0/S21_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m00_wgt_axi] [get_bd_intf_pins NOC_0/S22_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m01_wgt_axi] [get_bd_intf_pins NOC_0/S23_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m02_wgt_axi] [get_bd_intf_pins NOC_0/S24_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m03_wgt_axi] [get_bd_intf_pins NOC_0/S25_AXI]
connect_bd_intf_net [get_bd_intf_pins DPUCVDX8G/m00_bias_axi] [get_bd_intf_pins NOC_0/S26_AXI]
endgroup

# Add new clk_wiz for DPU
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wizard:1.0 clk_wizard_0
endgroup
set_property name clk_wiz_accel [get_bd_cells clk_wizard_0]
set_property -dict [list CONFIG.PRIM_SOURCE {Global_buffer} CONFIG.USE_RESET {true} CONFIG.RESET_TYPE {ACTIVE_LOW} CONFIG.CLKOUT_USED {true,true,false,false,false,false,false} CONFIG.CLKOUT_PORT {clk_out_333,clk_out_666,clk_out3,clk_out4,clk_out5,clk_out6,clk_out7} CONFIG.CLKOUT_REQUESTED_OUT_FREQUENCY {333.000,666.000,100.000,100.000,100.000,100.000,100.000} CONFIG.CLKOUT_REQUESTED_PHASE {0.000,0.000,0.000,0.000,0.000,0.000,0.000} CONFIG.CLKOUT_REQUESTED_DUTY_CYCLE {50.000,50.000,50.000,50.000,50.000,50.000,50.000} CONFIG.CLKOUT_DRIVES {BUFG,BUFG,BUFG,BUFG,BUFG,BUFG,BUFG} CONFIG.CLKOUT_GROUPING {Auto,Auto,Auto,Auto,Auto,Auto,Auto} CONFIG.CLKOUT_DYN_PS {None,None,None,None,None,None,None} CONFIG.CLKOUT_MATCHED_ROUTING {false,false,false,false,false,false,false} CONFIG.BANDWIDTH {LOW} CONFIG.CLKFBOUT_MULT {166.500000} CONFIG.DIVCLK_DIVIDE {5} CONFIG.CLKOUT1_DIVIDE {10.000000} CONFIG.CLKOUT2_DIVIDE {5.000000}] [get_bd_cells clk_wiz_accel]

connect_bd_net [get_bd_pins clk_wiz_accel/resetn] [get_bd_pins CIPS_0/pl0_resetn]
connect_bd_net [get_bd_pins clk_wiz_accel/clk_in1] [get_bd_pins CIPS_0/pl0_ref_clk]

# Add new rst_processor for DPU Clock
copy_bd_objs /  [get_bd_cells {rst_processor_150MHz}]
set_property name rst_processor_333MHz [get_bd_cells rst_processor_150MHz1]
copy_bd_objs /  [get_bd_cells {rst_processor_333MHz}]
set_property name rst_processor_666MHz [get_bd_cells rst_processor_333MHz1]

connect_bd_net [get_bd_pins clk_wiz_accel/clk_out_333] [get_bd_pins rst_processor_333MHz/slowest_sync_clk]
connect_bd_net [get_bd_pins clk_wiz_accel/clk_out_666] [get_bd_pins rst_processor_666MHz/slowest_sync_clk]
connect_bd_net [get_bd_pins rst_processor_666MHz/ext_reset_in] [get_bd_pins CIPS_0/pl0_resetn]
connect_bd_net [get_bd_pins rst_processor_333MHz/ext_reset_in] [get_bd_pins CIPS_0/pl0_resetn]

#Deleted default clock and reset connection####################################################################################
#Clock
disconnect_bd_net /net_mb_ss_0_clk_out2 [get_bd_pins ai_engine_0/aclk0]
disconnect_bd_net /net_mb_ss_0_clk_out2 [get_bd_pins DPUCVDX8G/ap_clk]
disconnect_bd_net /clk_wiz_clk_out2 [get_bd_pins DPUCVDX8G/ap_clk_2]
disconnect_bd_net /net_mb_ss_0_clk_out2 [get_bd_pins DPUCVDX8G/dpu_2x_clk]

#Reset
disconnect_bd_net /net_mb_ss_0_dcm_locked [get_bd_pins DPUCVDX8G/ap_rst_n]
disconnect_bd_net /rst_processor_1_100M_peripheral_aresetn [get_bd_pins DPUCVDX8G/ap_rst_n_2]
disconnect_bd_net /net_mb_ss_0_dcm_locked [get_bd_pins DPUCVDX8G/dpu_2x_resetn]

#Creat new clock and reset connection##########################################################################################
#S_axi_clk is set as 150M
connect_bd_net [get_bd_pins DPUCVDX8G/ap_clk_2] [get_bd_pins clk_wiz/clk_out_150]
connect_bd_net [get_bd_pins DPUCVDX8G/ap_rst_n_2] [get_bd_pins rst_processor_150MHz/peripheral_aresetn]

#Default ap_clk is 333M
connect_bd_net [get_bd_pins DPUCVDX8G/ap_clk] [get_bd_pins clk_wiz_accel/clk_out_333]
connect_bd_net [get_bd_pins DPUCVDX8G/ap_rst_n] [get_bd_pins rst_processor_333MHz/peripheral_aresetn]
connect_bd_net [get_bd_pins DPUCVDX8G/dpu_2x_clk] [get_bd_pins clk_wiz_accel/clk_out_333]
connect_bd_net [get_bd_pins DPUCVDX8G/dpu_2x_resetn] [get_bd_pins rst_processor_333MHz/peripheral_aresetn]
connect_bd_net [get_bd_pins NOC_0/aclk12] [get_bd_pins clk_wiz_accel/clk_out_333]
connect_bd_net [get_bd_pins ai_engine_0/aclk0] [get_bd_pins clk_wiz_accel/clk_out_333]

