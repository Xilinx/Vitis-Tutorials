#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vitis
import sys
import os
from hsi import *
import glob

cmd_args=len(sys.argv)
args=str(sys.argv)

client = vitis.create_client()
client.set_workspace(path="./workspace")

if cmd_args == 1:
    component_name=base_pfm_vek385
    xsa="custom_platform_vek385.xsa"
else:
    component_name=sys.argv[1]
    xsa=sys.argv[2]
    

if (not os.path.isdir("./workspace/"+component_name)):
    platform = client.create_platform_component(name = component_name,hw_design = "../vivado/build/xsa_platform/"+xsa,os = "aie_runtime",cpu = "ai_engine")

platform = client.get_component(name=component_name)
status = platform.build()

vitis.dispose()