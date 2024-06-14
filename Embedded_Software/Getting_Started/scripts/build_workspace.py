#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

import vitis
import os
import shutil


workspace = os.path.join(os.getcwd(), "./unified_workspace")
workspace = os.path.normpath(workspace)
            
print("Creating workspace at " + workspace)
if os.path.exists(workspace):
    print("workspace exists. Deleting ...")
    shutil.rmtree(workspace)

client = vitis.create_client()
client.set_workspace(workspace)

platform = client.create_platform_component(name = "zcu102_platform",hw_design = "zcu102",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

xpfm = os.path.join(os.getcwd(), "./unified_workspace/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm")
xpfm = os.path.normpath(xpfm)
comp = client.create_app_component(name="hello_world",platform = xpfm, domain = "standalone_psu_cortexa53_0",template = "hello_world")

platform = client.get_component(name="zcu102_platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()
