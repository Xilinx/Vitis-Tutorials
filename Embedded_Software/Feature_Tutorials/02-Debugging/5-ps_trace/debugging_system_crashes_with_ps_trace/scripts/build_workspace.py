# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#*****************************************************************************************
import vitis
import os
import shutil

client = vitis.create_client()
workspace = os.path.join(os.getcwd(), "./unified_workspace")
workspace = os.path.normpath(workspace)
client.set_workspace(path=workspace)

platform = client.create_platform_component(name = "vck190_platform",hw_design = "$COMPONENT_LOCATION/../../edt_versal/edt_versal_wrapper.xsa",os = "standalone",cpu = "psv_cortexa72_0",domain_name = "standalone_psv_cortexa72_0",compiler = "gcc")

platform = client.get_component(name="vck190_platform")
status = platform.build()

comp = client.create_app_component(name="gpio_led_blink",platform = "$COMPONENT_LOCATION/../vck190_platform/export/vck190_platform/vck190_platform.xpfm",domain = "standalone_psv_cortexa72_0")

comp = client.get_component(name="gpio_led_blink")
status = comp.import_files(from_loc="", files=["main.c"], is_skip_copy_sources = False)

comp.build()

old_xparameters = workspace + "/vck190_platform/psv_cortexa72_0/standalone_psv_cortexa72_0/bsp/include/xparameters.h"
os.remove(old_xparameters)

new_xparameters = "xparameters.h"
xparameters_destination = workspace + "/vck190_platform/psv_cortexa72_0/standalone_psv_cortexa72_0/bsp/include/"

shutil.move(new_xparameters, xparameters_destination)

platform = client.get_component(name="vck190_platform")
status = platform.build()
comp = client.get_component(name="gpio_led_blink")
comp.build()
