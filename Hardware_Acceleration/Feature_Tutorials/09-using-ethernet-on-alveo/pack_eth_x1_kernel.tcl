# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */


# create ip project with part name in command line argvs
create_project eth_kernel ./eth_kernel -part [lindex $argv 0]

# add design sources into project
add_files -norecurse \
       {                                         \
              ../rtl/ethernet_control_s_axi_x1.v    \
              ../rtl/ethernet_krnl_axis_x1.sv    \
              ../ip_generation/axis_data_fifo_0/axis_data_fifo_0.xci \
              ../ip_generation/xxv_ethernet_0/xxv_ethernet_0.xci \
       }

update_compile_order -fileset sources_1

# create IP packaging project
ipx::package_project -root_dir ./eth_kernel_pack -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true

# AXI bus
ipx::add_bus_interface ap_clk [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:signal:clock_rtl:1.0 [ipx::get_bus_interfaces ap_clk -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:signal:clock:1.0 [ipx::get_bus_interfaces ap_clk -of_objects [ipx::current_core]]
ipx::add_port_map CLK [ipx::get_bus_interfaces ap_clk -of_objects [ipx::current_core]]
set_property physical_name ap_clk [ipx::get_port_maps CLK -of_objects [ipx::get_bus_interfaces ap_clk -of_objects [ipx::current_core]]]
ipx::associate_bus_interfaces -busif s_axi_control -clock ap_clk [ipx::current_core]

# GT data bus
ipx::add_bus_interface gt_port [ipx::current_core]
set_property interface_mode master [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property abstraction_type_vlnv xilinx.com:interface:gt_rtl:1.0 [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:gt:1.0 [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
ipx::add_port_map GRX_P [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_rxp_in [ipx::get_port_maps GRX_P -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]
ipx::add_port_map GTX_N [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_txn_out [ipx::get_port_maps GTX_N -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]
ipx::add_port_map GRX_N [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_rxn_in [ipx::get_port_maps GRX_N -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]
ipx::add_port_map GTX_P [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]
set_property physical_name gt_txp_out [ipx::get_port_maps GTX_P -of_objects [ipx::get_bus_interfaces gt_port -of_objects [ipx::current_core]]]

# GT Differential Reference Clock
ipx::add_bus_interface gt_refclk [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:diff_clock_rtl:1.0 [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:diff_clock:1.0 [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
ipx::add_port_map CLK_P [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property physical_name gt_refclk_p [ipx::get_port_maps CLK_P -of_objects [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]]
ipx::add_port_map CLK_N [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]
set_property physical_name gt_refclk_n [ipx::get_port_maps CLK_N -of_objects [ipx::get_bus_interfaces gt_refclk -of_objects [ipx::current_core]]]

ipx::associate_bus_interfaces -busif rx0_axis -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif tx0_axis -clock ap_clk [ipx::current_core]

set_property xpm_libraries {XPM_CDC XPM_MEMORY XPM_FIFO} [ipx::current_core]
set_property supported_families { } [ipx::current_core]
set_property auto_family_support_level level_2 [ipx::current_core]
ipx::update_checksums [ipx::current_core]

# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force -xo_path ../ethernet_krnl_axis_x1.xo \
           -kernel_name ethernet_krnl_axis_x1 \
           -ctrl_protocol ap_ctrl_hs \
           -ip_directory ./eth_kernel_pack \
           -output_kernel_xml ../ethernet_krnl_axis_x1.xml
