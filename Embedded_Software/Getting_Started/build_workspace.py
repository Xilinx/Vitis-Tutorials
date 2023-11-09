#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

import vitis
import os


pwd = os.getcwd()
client = vitis.create_client()
client.set_workspace(path=pwd + "/vitis_ws")

platform = client.create_platform_component(name = "zcu102_platform",hw = "/home/stephenm/cases/versal_linux_vitis/zcu102.xsa",os = "standalone",cpu = "psu_cortexa53_0")

comp = client.create_app_component(name="hello_world",platform = "/home/stephenm/cases/versal_linux_vitis/baremetal_ws/zcu102_platform/export/zcu102_platform/zcu102_platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

platform = client.get_platform_component(name="zcu102_platform")
status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()

