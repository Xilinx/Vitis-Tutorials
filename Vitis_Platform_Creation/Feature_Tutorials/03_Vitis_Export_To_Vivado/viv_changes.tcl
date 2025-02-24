set target [lindex $argv 0]

open_project my_project/my_project.xpr
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1
open_bd_design {./my_project/my_project.srcs/sources_1/bd/vitis_design/vitis_design.bd}
set_property  ip_repo_paths  {./custom_ip} [current_project]
update_ip_catalog
startgroup
create_bd_cell -type ip -vlnv xilinx.com:hls:stream_out:1.0 stream_out_0
endgroup
startgroup
set_property CONFIG.NUM_MI {7} [get_bd_cells axi_smc_vip_hier/icn_ctrl]
set_property CONFIG.NUM_CLKS {2} [get_bd_cells axi_smc_vip_hier/icn_ctrl]
connect_bd_net [get_bd_pins axi_smc_vip_hier/icn_ctrl/aclk1] [get_bd_pins clk_wizard_0/clk_out3]
endgroup
connect_bd_net [get_bd_pins stream_out_0/ap_clk] [get_bd_pins clk_wizard_0/clk_out1_o2]
connect_bd_net [get_bd_pins stream_out_0/ap_rst_n] [get_bd_pins proc_sys_reset_1/peripheral_aresetn]
set_property PFM.AXIS_PORT {s {type "M_AXIS" sptag "AIE_IN" is_range "false"}} [get_bd_cells /stream_out_0]
set_property CONFIG.NUM_SI {1} [get_bd_cells noc_ddr4]
set_property -dict [list CONFIG.CONNECTIONS {MC_1 {read_bw {500} write_bw {500} read_avg_burst {4} write_avg_burst {4}}}] [get_bd_intf_pins /noc_ddr4/S00_AXI]
set_property -dict [list CONFIG.CONNECTIONS {MC_0 {read_bw {5} write_bw {5} read_avg_burst {4} write_avg_burst {4}}}] [get_bd_intf_pins /noc_ddr4/S00_INI]
set_property -dict [list CONFIG.CONNECTIONS {MC_1 {read_bw {5} write_bw {5} read_avg_burst {4} write_avg_burst {4}}}] [get_bd_intf_pins /noc_ddr4/S01_INI]
set_property -dict [list CONFIG.CONNECTIONS {MC_2 {read_bw {5} write_bw {5} read_avg_burst {4} write_avg_burst {4}}}] [get_bd_intf_pins /noc_ddr4/S02_INI]
set_property -dict [list CONFIG.CONNECTIONS {MC_3 {read_bw {5} write_bw {5} read_avg_burst {4} write_avg_burst {4}}}] [get_bd_intf_pins /noc_ddr4/S03_INI]
connect_bd_net [get_bd_pins noc_ddr4/aclk0] [get_bd_pins clk_wizard_0/clk_out3]
assign_bd_address
validate_bd_design -force
save_bd_design
set_property synth_checkpoint_mode None [get_files  ./my_project/my_project.srcs/sources_1/bd/vitis_design/vitis_design.bd]
generate_target all [get_files  ./my_project/my_project.srcs/sources_1/bd/vitis_design/vitis_design.bd]
make_wrapper -files [get_files ./my_project/my_project.srcs/sources_1/bd/vitis_design/vitis_design.bd] -top
add_files -norecurse ./my_project/my_project.gen/sources_1/bd/vitis_design/hdl/vitis_design_wrapper.v
update_compile_order -fileset sources_1
if {$target eq "hw_emu"} {
write_hw_platform -hw_emu -force -file hw_emu.xsa
} else {
write_hw_platform -hw -force -file hw.xsa
}
