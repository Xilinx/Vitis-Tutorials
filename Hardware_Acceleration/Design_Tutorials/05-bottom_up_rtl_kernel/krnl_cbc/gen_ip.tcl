#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# set the device part from command line argvs
set_part [lindex $argv 0]

# ----------------------------------------------------------------------------
# generate axi master vip
# ----------------------------------------------------------------------------
create_ip -name axi_vip \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axi_vip_mst \
          -dir ./ip_generation
          
set_property -dict [list CONFIG.INTERFACE_MODE {MASTER} \
                         CONFIG.PROTOCOL {AXI4LITE} \
                         CONFIG.SUPPORTS_NARROW {0} \
                         CONFIG.HAS_BURST {0} \
                         CONFIG.HAS_LOCK {0} \
                         CONFIG.HAS_CACHE {0} \
                         CONFIG.HAS_REGION {0} \
                         CONFIG.HAS_QOS {0} \
                         CONFIG.HAS_PROT {0} \
                         CONFIG.HAS_WSTRB {1}] \
             [get_ips axi_vip_mst]
             
generate_target all [get_files  ./ip_generation/axi_vip_mst/axi_vip_mst.xci]

# ----------------------------------------------------------------------------
# generate axi slave vip
# ----------------------------------------------------------------------------
create_ip -name axi_vip \
          -vendor xilinx.com \
          -library ip \
          -version 1.1 \
          -module_name axi_vip_slv \
          -dir ./ip_generation
          
set_property -dict [list CONFIG.INTERFACE_MODE {SLAVE} \
                         CONFIG.PROTOCOL {AXI4} \
                         CONFIG.ADDR_WIDTH {64} \
                         CONFIG.DATA_WIDTH {128} \
                         CONFIG.SUPPORTS_NARROW {0} \
                         CONFIG.HAS_LOCK {0} \
                         CONFIG.HAS_CACHE {0} \
                         CONFIG.HAS_REGION {0} \
                         CONFIG.HAS_QOS {0} \
                         CONFIG.HAS_PROT {0}] \
             [get_ips axi_vip_slv]
             
generate_target all [get_files  ./ip_generation/axi_vip_slv/axi_vip_slv.xci]
