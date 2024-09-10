#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

cmd_args=len(sys.argv)
args=str(sys.argv)

component_name=sys.argv[1]
xsa=sys.argv[2]
design_part=sys.argv[3]

client = vitis.create_client()
client.set_workspace(path="./workspace_"+design_part)


    

if (not os.path.isdir("./workspace_"+design_part+"/"+component_name)):
    platform = client.create_platform_component(name = component_name,hw_design = "../Vivado/build/xsa_platform/"+xsa,os = "aie_runtime",cpu = "ai_engine")
    domain = platform.add_domain(cpu = "psv_cortexa72_0",os = "standalone",name = "baremetal",display_name = "baremetal")

platform = client.get_component(name=component_name)
status = platform.build()
