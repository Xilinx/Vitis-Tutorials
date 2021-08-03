#
# Copyright 2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


##################################### Step 1: create vivado project and add design sources

# create ip project with part name in command line argvs
create_project krnl_aes ./krnl_aes -part [lindex $argv 0]

# add design sources into project
add_files -norecurse \
       {                                    \
        ../rtl/aes_wrapper.sv               \
        ../rtl/axis_interface.sv            \
        ../rtl/krnl_aes_axi_ctrl_slave.v    \
        ../rtl/krnl_aes.sv                  \
        ../../aes/rtl/AesControl.v          \
        ../../aes/rtl/AesDecipher.v         \
        ../../aes/rtl/AesEncipher.v         \
        ../../aes/rtl/AesInvMixColumns.v    \
        ../../aes/rtl/AesInvShiftRows.v     \
        ../../aes/rtl/AesInvSubBytes.v      \
        ../../aes/rtl/AesKeyExpansion.v     \
        ../../aes/rtl/AesMixColumns.v       \
        ../../aes/rtl/AesShiftRows.v        \
        ../../aes/rtl/AesSubBytes.v         \
        ../../aes/rtl/Aes.v                 \
        ../../aes/rtl/SPSR.v                \
        ../ip_generation/clk_gen/clk_gen.xci      \
        ../ip_generation/axi_clock_converter/axi_clock_converter.xci \
        ../ip_generation/axis_clock_converter/axis_clock_converter.xci \
        ../krnl_aes.xdc                     \
       }

update_compile_order -fileset sources_1

# create IP packaging project
ipx::package_project -root_dir ./krnl_aes_ip -vendor xilinx.com -library user -taxonomy /UserIP -import_files -set_current true


##################################### Step 2: Inference clock, reset, AXI interfaces and associate them with clock

# inference clock and reset signals
ipx::infer_bus_interface ap_clk xilinx.com:signal:clock_rtl:1.0 [ipx::current_core]
ipx::infer_bus_interface ap_rst_n xilinx.com:signal:reset_rtl:1.0 [ipx::current_core]

# associate AXI/AXIS interface with clock
ipx::associate_bus_interfaces -busif s_axi_control  -clock ap_clk [ipx::current_core]
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
ipx::add_register CTRL      [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register MODE      [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_LEN   [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register STATUS    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W7    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W6    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W5    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W4    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W3    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W2    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W1    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]
ipx::add_register KEY_W0    [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]

# Set RTL kernel registers property
set_property description    {Control Signals}   [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x000}             [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers CTRL    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES working mode}  [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x010}             [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers MODE    -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key length}    [ipx::get_registers KEY_LEN -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x018}             [ipx::get_registers KEY_LEN -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_LEN -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES engine status} [ipx::get_registers STATUS  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x020}             [ipx::get_registers STATUS  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers STATUS  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 7}    [ipx::get_registers KEY_W7  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x028}             [ipx::get_registers KEY_W7  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W7  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 6}    [ipx::get_registers KEY_W6  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x030}             [ipx::get_registers KEY_W6  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W6  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 5}    [ipx::get_registers KEY_W5  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x038}             [ipx::get_registers KEY_W5  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W5  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 4}    [ipx::get_registers KEY_W4  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x040}             [ipx::get_registers KEY_W4  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W4  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 3}    [ipx::get_registers KEY_W3  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x048}             [ipx::get_registers KEY_W3  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W3  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 2}    [ipx::get_registers KEY_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x050}             [ipx::get_registers KEY_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W2  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 1}    [ipx::get_registers KEY_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x058}             [ipx::get_registers KEY_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W1  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]

set_property description    {AES key word 0}    [ipx::get_registers KEY_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property address_offset {0x060}             [ipx::get_registers KEY_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]
set_property size           {32}                [ipx::get_registers KEY_W0  -of_objects [ipx::get_address_blocks reg0 -of_objects [ipx::get_memory_maps s_axi_control -of_objects [ipx::current_core]]]]


#### Step 4: Package Vivado IP and generate Vitis kernel file

# Set required property for Vitis kernel
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]

# Packaging Vivado IP
ipx::update_source_project_archive -component [ipx::current_core]
ipx::save_core [ipx::current_core]

# Generate Vitis Kernel from Vivado IP
package_xo -force -xo_path ../krnl_aes.xo -kernel_name krnl_aes -ctrl_protocol ap_ctrl_hs -ip_directory ./krnl_aes_ip -output_kernel_xml ../krnl_aes.xml
