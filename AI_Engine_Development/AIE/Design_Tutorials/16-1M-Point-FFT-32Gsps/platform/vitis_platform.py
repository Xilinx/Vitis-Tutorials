#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author: Mark Rollins

import vitis
import sys
import os
from hsi import *
import glob

cmd_args=len(sys.argv)
args=str(sys.argv)

client = vitis.create_client()
client.set_workspace(path="./workspace")

hw_design_name="custom_pfm_vck190/custom_platform_vck190.xsa"
component_name="base_pfm_vck190"

platform = client.create_platform_component(name = component_name,hw_design = hw_design_name,os = "aie_runtime",cpu = "ai_engine")

platform = client.get_component(name=component_name)
domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "aie_runtime",display_name = "aie_runtime")

status = platform.build()

vitis.dispose()
