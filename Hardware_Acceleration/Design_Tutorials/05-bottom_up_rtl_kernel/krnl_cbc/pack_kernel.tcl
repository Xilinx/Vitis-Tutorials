#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

##################################### Step 1: create vivado project and add design sources

# create ip project with part name in command line argvs
create_project krnl_cbc ./krnl_cbc -part [lindex $argv 0]

# add design sources into project
add_files -norecurse \
       {                                         \
              ../rtl/axi_master_counter.sv       \
              ../rtl/axi_read_master.sv          \
              ../rtl/axi_write_master.sv         \
              ../rtl/cbc_engine.v                \
              ../rtl/engine_control.v            \
              ../rtl/FifoCtlType0.v              \
              ../rtl/FifoType0.v                 \
              ../rtl/krnl_cbc.v                \
              ../rtl/krnl_cbc_ctrl_slave.v       \
              ../rtl/TPSR.v                      \
       }

update_compile_order -fileset sources_1

# create IP packaging project
ipx::package_project -root_dir ./krnl_cbc_ip -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true


##################################### Step 2: Inference clock, reset, AXI interfaces and associate them with clock

# inference clock and reset signals
ipx::infer_bus_interface ap_clk xilinx.com:signal:clock_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface ap_rst_n xilinx.com:signal:reset_rtl:1.0 [ipx::current_core]

# associate AXI/AXIS interface with clock
ipx::associate_bus_interfaces -busif s_axi_control  -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axi_rmst       -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axi_wmst       -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_mst0      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_mst1      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_mst2      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_mst3      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_slv0      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_slv1      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_slv2      -clock ap_clk [ipx::current_core]
ipx::associate_bus_interfaces -busif axis_slv3      -clock ap_clk [ipx::current_core]

# associate reset signal with clock
ipx::associate_bus_interfaces -clock ap_clk -reset ap_rst_n [ipx::current_core]


##################################### Step 3: Set the definition of AXI control slave registers, including CTRL and user kernel arguments

# Add RTL kernel registers
ipx::add_register CTRL         [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register MODE         [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register CBC_MODE     [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register IV_W3        [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register IV_W2        [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register IV_W1        [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register IV_W0        [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register WORDS_NUM    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register SRC_ADDR     [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register DEST_ADDR    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]

# Set RTL kernel registers property
set_property description    {Control Signals}   [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x000}             [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {working mode}      [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x010}             [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {intial vector w3}  [ipx::get_registers IV_W3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x018}             [ipx::get_registers IV_W3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers IV_W3 -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {initial vector w2} [ipx::get_registers IV_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x020}             [ipx::get_registers IV_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers IV_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {initial vector w1} [ipx::get_registers IV_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x028}             [ipx::get_registers IV_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers IV_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {initial vector w0} [ipx::get_registers IV_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x030}             [ipx::get_registers IV_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers IV_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {number of words}   [ipx::get_registers WORDS_NUM  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x038}             [ipx::get_registers WORDS_NUM  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers WORDS_NUM  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {read addr     }    [ipx::get_registers SRC_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x040}             [ipx::get_registers SRC_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {64}                [ipx::get_registers SRC_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {write addr    }    [ipx::get_registers DEST_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x048}             [ipx::get_registers DEST_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {64}                [ipx::get_registers DEST_ADDR  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {cbc mode}          [ipx::get_registers CBC_MODE  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x050}             [ipx::get_registers CBC_MODE  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers CBC_MODE  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]


##################################### Step 4: associate AXI master port to pointer argument and set data width

# define association between pointer arguments (SRC_ADDR, DEST_ADDR) and axi masters (axi_rmst, axi_wmst)
ipx::add_register_parameter ASSOCIATED_BUSIF [ipx::get_registers SRC_ADDR -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property value          {axi_rmst}          [ipx::get_register_parameters ASSOCIATED_BUSIF     \
                                    -of_objects [ipx::get_registers SRC_ADDR                      \
                                    -of_objects [ipx::get_address_blocks reg0                      \
                                    -of_objects [ipx::get_memory_maps s_axi_control                 \
                                    -of_objects [ipx::current_core]]]]]

ipx::add_register_parameter ASSOCIATED_BUSIF [ipx::get_registers DEST_ADDR -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property value          {axi_wmst}          [ipx::get_register_parameters ASSOCIATED_BUSIF     \
                                    -of_objects [ipx::get_registers DEST_ADDR                      \
                                    -of_objects [ipx::get_address_blocks reg0                      \
                                    -of_objects [ipx::get_memory_maps s_axi_control                 \
                                    -of_objects [ipx::current_core]]]]]

ipx::add_bus_parameter DATA_WIDTH [ipx::get_bus_interfaces axi_wmst -of_objects [ipx::current_core]]
set_property value          {128} [ipx::get_bus_parameters DATA_WIDTH -of_objects [ipx::get_bus_interfaces axi_wmst -of_objects [ipx::current_core]]]

ipx::add_bus_parameter DATA_WIDTH [ipx::get_bus_interfaces axi_rmst -of_objects [ipx::current_core]]
set_property value          {128} [ipx::get_bus_parameters DATA_WIDTH -of_objects [ipx::get_bus_interfaces axi_rmst -of_objects [ipx::current_core]]]

#### Step 5: Package Vivado IP and generate Vitis kernel file

# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force -xo_path ../krnl_cbc.xo -kernel_name krnl_cbc -ctrl_protocol ap_ctrl_chain -ip_directory ./krnl_cbc_ip -output_kernel_xml ../krnl_cbc.xml
