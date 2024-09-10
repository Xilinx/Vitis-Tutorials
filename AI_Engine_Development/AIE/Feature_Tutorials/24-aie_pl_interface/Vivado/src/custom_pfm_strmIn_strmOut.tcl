#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

set include_ilas [lindex $argv 0]
set RTL_out_bd [lindex $argv 1]
set AXIS_BROADCAST [lindex $argv 2]

# Create Project
if { ${include_ilas} == "1" } {
	set pfm_name custom_pfm_strmIn_strmOut_ILAs
} elseif { ${RTL_out_bd} == "1" } {
	set pfm_name custom_pfm_strmIn_strmOut_RTL_out
} elseif { ${AXIS_BROADCAST} == "1" } {
	set pfm_name custom_pfm_strmIn_strmOut_AXIS_Broadcast
} else {
	set pfm_name custom_pfm_strmIn_strmOut
}
create_project ${pfm_name} ./build/${pfm_name} -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.3 [current_project]
set_property target_language Verilog [current_project]

# Create Block Design
create_bd_design "custom_pfm_strmIn_strmOut_bd" -mode batch

# Open Versal Extensible Platform CED
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design custom_pfm_strmIn_strmOut_bd -options { Include_AIE.VALUE true Include_BDC.VALUE false Include_DDR.VALUE true}
update_compile_order -fileset sources_1

# Add custom IPs to IP catalog
set_property  ip_repo_paths  ./src/ip_repo [current_project]
update_ip_catalog

# Open Block Design
open_bd_design {./build/${pfm_name}/${pfm_name}.srcs/sources_1/bd/custom_pfm_strmIn_strmOut_bd/custom_pfm_strmIn_strmOut_bd.bd}

if {$RTL_out_bd == "0"} {

	# Add AXI4-Stream Counter and connect clock and reset
	create_bd_cell -type ip -vlnv AMD.com:AMD:AXI4S_Counter:1.0 AXI4S_Counter_0
	connect_bd_net [get_bd_pins AXI4S_Counter_0/m00_axis_aclk] [get_bd_pins clk_wizard_0/clk_out1]
	connect_bd_net [get_bd_pins AXI4S_Counter_0/m00_axis_aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

	# Add Dummy Sink
	create_bd_cell -type ip -vlnv AMD.com:AMD:dummy_sink:1.0 dummy_sink_0
	connect_bd_net [get_bd_pins dummy_sink_0/s00_axis_aclk] [get_bd_pins clk_wizard_0/clk_out1]
	connect_bd_net [get_bd_pins dummy_sink_0/s00_axis_aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

	if {$include_ilas == "1"} {
		# Add ILA to monitor input and output streams
		create_bd_cell -type ip -vlnv xilinx.com:ip:axis_ila:1.2 axis_ila_0
		set_property -dict [list \
		CONFIG.C_MON_TYPE {Interface_Monitor} \
		CONFIG.C_NUM_MONITOR_SLOTS {2} \
		CONFIG.C_SLOT_0_INTF_TYPE {xilinx.com:interface:axis_rtl:1.0} \
		CONFIG.C_SLOT_1_INTF_TYPE {xilinx.com:interface:axis_rtl:1.0} \
		CONFIG.ALL_PROBE_SAME_MU_CNT {2} \
		] [get_bd_cells axis_ila_0]

		connect_bd_intf_net [get_bd_intf_pins axis_ila_0/SLOT_0_AXIS] [get_bd_intf_pins AXI4S_Counter_0/M00_AXIS]
		connect_bd_net [get_bd_pins axis_ila_0/clk] [get_bd_pins clk_wizard_0/clk_out1]
		connect_bd_net [get_bd_pins axis_ila_0/resetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
		connect_bd_intf_net [get_bd_intf_pins axis_ila_0/SLOT_1_AXIS] [get_bd_intf_pins dummy_sink_0/S00_AXIS]
	}

	if { ${AXIS_BROADCAST} == "1" } {
		# Add another Dummy Sink to be connected to AXI4-Stream Broadcaster IP
		create_bd_cell -type ip -vlnv AMD.com:AMD:dummy_sink:1.0 dummy_sink_1
		connect_bd_net [get_bd_pins dummy_sink_1/s00_axis_aclk] [get_bd_pins clk_wizard_0/clk_out1]
		connect_bd_net [get_bd_pins dummy_sink_1/s00_axis_aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

		# Add an AXI4-Stream Broadcaster IP
		create_bd_cell -type ip -vlnv xilinx.com:ip:axis_broadcaster:1.1 axis_broadcaster_0
		set_property CONFIG.NUM_MI {2} [get_bd_cells axis_broadcaster_0]
		connect_bd_net [get_bd_pins axis_broadcaster_0/aclk] [get_bd_pins clk_wizard_0/clk_out1]
		connect_bd_net [get_bd_pins axis_broadcaster_0/aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]
		connect_bd_intf_net [get_bd_intf_pins axis_broadcaster_0/S_AXIS] [get_bd_intf_pins AXI4S_Counter_0/M00_AXIS]
		connect_bd_intf_net [get_bd_intf_pins axis_broadcaster_0/M01_AXIS] [get_bd_intf_pins dummy_sink_1/S00_AXIS]

		# Add AXI4-Stream interfaces to Platform
		set_property PFM.AXIS_PORT {M00_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /axis_broadcaster_0]
		set_property PFM.AXIS_PORT {S00_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /dummy_sink_0]
	
	} else {
		# Add AXI4-Stream interfaces to Platform
		set_property PFM.AXIS_PORT {M00_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /AXI4S_Counter_0]
		set_property PFM.AXIS_PORT {S00_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /dummy_sink_0]
	}

	# Save BD
	validate_bd_design
	update_compile_order -fileset sources_1
	save_bd_design

} else {

	# Create port for clock and Reset
	create_bd_port -dir O -type clk axis_aclk
	connect_bd_net [get_bd_ports axis_aclk] [get_bd_pins clk_wizard_0/clk_out1]

	create_bd_port -dir O -type rst axis_aresetn
	connect_bd_net [get_bd_ports axis_aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

	# Add 2 AXI4-Stream Register Slices Pass-Through
	create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_0
	set_property -dict [list \
		CONFIG.HAS_TLAST {1} \
		CONFIG.HAS_TREADY {1} \
		CONFIG.HAS_TKEEP {0} \
  		CONFIG.HAS_TSTRB {1} \
		CONFIG.HAS_ACLKEN {0} \
  		CONFIG.TDEST_WIDTH {0} \
  		CONFIG.TID_WIDTH {0} \
  		CONFIG.TUSER_WIDTH {0} \
		CONFIG.REG_CONFIG {0} \
		CONFIG.TDATA_NUM_BYTES {8} \
	] [get_bd_cells axis_register_slice_0]

	create_bd_cell -type ip -vlnv xilinx.com:ip:axis_register_slice:1.1 axis_register_slice_1
	set_property -dict [list \
		CONFIG.HAS_TLAST {1} \
		CONFIG.HAS_TREADY {1} \
		CONFIG.HAS_TKEEP {0} \
  		CONFIG.HAS_TSTRB {1} \
		CONFIG.HAS_ACLKEN {0} \
  		CONFIG.TDEST_WIDTH {0} \
  		CONFIG.TID_WIDTH {0} \
  		CONFIG.TUSER_WIDTH {0} \
		CONFIG.REG_CONFIG {0} \
		CONFIG.TDATA_NUM_BYTES {8} \
	] [get_bd_cells axis_register_slice_1]

	connect_bd_net [get_bd_pins axis_register_slice_0/aclk] [get_bd_pins clk_wizard_0/clk_out1]
	connect_bd_net [get_bd_pins axis_register_slice_0/aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

	connect_bd_net [get_bd_pins axis_register_slice_1/aclk] [get_bd_pins clk_wizard_0/clk_out1]
	connect_bd_net [get_bd_pins axis_register_slice_1/aresetn] [get_bd_pins proc_sys_reset_0/peripheral_aresetn]

	# Create BD AXI4-Stream interface ports and connect to the pass-through IPs
	create_bd_intf_port -mode Master -vlnv xilinx.com:interface:axis_rtl:1.0 M00_AXIS
	create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:axis_rtl:1.0 S00_AXIS
	set_property -dict [list CONFIG.TDATA_NUM_BYTES 8 CONFIG.HAS_TKEEP 1 CONFIG.HAS_TLAST 1 CONFIG.HAS_TSTRB 1] [get_bd_intf_ports S00_AXIS]

	set_property CONFIG.ASSOCIATED_BUSIF {M00_AXIS:S00_AXIS} [get_bd_ports /axis_aclk]
	
	connect_bd_intf_net [get_bd_intf_ports S00_AXIS] [get_bd_intf_pins axis_register_slice_0/S_AXIS]
	connect_bd_intf_net [get_bd_intf_ports M00_AXIS] [get_bd_intf_pins axis_register_slice_1/M_AXIS]


	# Add AXI4-Stream interfaces to Platform
	set_property PFM.AXIS_PORT {M_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /axis_register_slice_0]
	set_property PFM.AXIS_PORT {S_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /axis_register_slice_1]

	# Save BD
	validate_bd_design
	update_compile_order -fileset sources_1
	save_bd_design

	# Delete and regenerate BD wrapper to include new BD interface ports
	remove_files ./build/${pfm_name}/${pfm_name}.srcs/sources_1/imports/hdl/custom_pfm_strmIn_strmOut_bd_wrapper.v
	file delete -force ./build/${pfm_name}/${pfm_name}.srcs/sources_1/imports/hdl/custom_pfm_strmIn_strmOut_bd_wrapper.v

	make_wrapper -files [get_files ./build/${pfm_name}/${pfm_name}.srcs/sources_1/bd/custom_pfm_strmIn_strmOut_bd/custom_pfm_strmIn_strmOut_bd.bd] -top
	add_files -norecurse ./build/${pfm_name}/${pfm_name}.gen/sources_1/bd/custom_pfm_strmIn_strmOut_bd/hdl/custom_pfm_strmIn_strmOut_bd_wrapper.v
	update_compile_order -fileset sources_1
}



# Generate BD output products
generate_target all [get_files  ./build/${pfm_name}/${pfm_name}.srcs/sources_1/bd/custom_pfm_strmIn_strmOut_bd/custom_pfm_strmIn_strmOut_bd.bd]

if {$RTL_out_bd == "1"} {
	# Add top level RTL file
	add_files -norecurse ./src/hdl/top_level.v
	set_property top top_level [current_fileset]
	update_compile_order -fileset sources_1

	create_ip -name AXI4S_Counter -vendor AMD.com -library AMD -version 1.0 -module_name AXI4S_Counter_0
	generate_target {instantiation_template} [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/AXI4S_Counter_0/AXI4S_Counter_0.xci]
	generate_target all [get_files  ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/AXI4S_Counter_0/AXI4S_Counter_0.xci]
	export_ip_user_files -of_objects [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/AXI4S_Counter_0/AXI4S_Counter_0.xci] -no_script -sync -force -quiet
	create_ip_run [get_files -of_objects [get_fileset sources_1] ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/AXI4S_Counter_0/AXI4S_Counter_0.xci]
	launch_runs AXI4S_Counter_0_synth_1 
	export_simulation -of_objects [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/AXI4S_Counter_0/AXI4S_Counter_0.xci] -directory /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files/sim_scripts -ip_user_files_dir /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files -ipstatic_source_dir /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files/ipstatic -lib_map_path [list {modelsim=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/modelsim} {questa=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/questa} {xcelium=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/xcelium} {vcs=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/vcs} {riviera=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet
	
	create_ip -name dummy_sink -vendor AMD.com -library AMD -version 1.0 -module_name dummy_sink_0
	generate_target {instantiation_template} [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/dummy_sink_0/dummy_sink_0.xci]
	generate_target all [get_files  ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/dummy_sink_0/dummy_sink_0.xci]
	export_ip_user_files -of_objects [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/dummy_sink_0/dummy_sink_0.xci] -no_script -sync -force -quiet
	create_ip_run [get_files -of_objects [get_fileset sources_1] ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/dummy_sink_0/dummy_sink_0.xci]
	launch_runs dummy_sink_0_synth_1
	export_simulation -of_objects [get_files ./build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.srcs/sources_1/ip/dummy_sink_0/dummy_sink_0.xci] -directory /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files/sim_scripts -ip_user_files_dir /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files -ipstatic_source_dir /group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.ip_user_files/ipstatic -lib_map_path [list {modelsim=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/modelsim} {questa=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/questa} {xcelium=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/xcelium} {vcs=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/vcs} {riviera=/group/xircss/florentw/Work/Support/External/Keysight/test_design/AIE_RTL_Stream_IN_OUT/Vivado/build/custom_pfm_strmIn_strmOut_RTL_out/custom_pfm_strmIn_strmOut_RTL_out.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet
}

# Run Synthesis
update_compile_order -fileset sources_1
launch_runs synth_1  -jobs 28

wait_on_run synth_1

# Export Platform XSA
set_property platform.vendor {AMD} [current_project]
set_property platform.name ${pfm_name} [current_project]
set_property pfm_name "AMD:vck190:$pfm_name:0.0" [get_files -all {custom_pfm_strmIn_strmOut_bd.bd}]
set_property platform.description {Custom PFM with unconnected stream IN and OUT} [current_project]
set_property platform.uses_pr {false} [current_project]
write_hw_platform -force -file ./build/xsa_platform/${pfm_name}.xsa

