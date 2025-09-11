#
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

import vitis
import os

client = vitis.create_client()

pwd = os.getcwd()
client.set_workspace(pwd + "/vitis_ws")

platform = client.create_platform_component(name = "zcu104_platform", hw_design = "zcu104/design_1_wrapper.xsa", os = "standalone",cpu = "psu_cortexa53_0")
platform = client.get_component(name="zcu104_platform")
platform.build()
platform.list_domains()

app = client.create_app_component(name="hello_world",platform = "vitis_ws/zcu104_platform/export/zcu104_platform/zcu104_platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")
app = client.get_component(name="hello_world")
app.build()
