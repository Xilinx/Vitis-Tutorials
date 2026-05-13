#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author: Mark Rollins

import vitis

client = vitis.create_client()
client.set_workspace(path="./workspace")

platform = client.create_platform_component(name = "sw_platform",hw_design = "$COMPONENT_LOCATION/../../build_hw/base_pfm_vck190.xsa",os = "standalone",cpu = "psv_cortexa72_0",domain_name = "standalone_psv_cortexa72_0",compiler = "gcc")

platform = client.get_component(name="sw_platform")
domain = platform.add_domain(cpu = "ai_engine",os = "aie_runtime",name = "aie_runtime",display_name = "aie_runtime")

status = platform.build()

vitis.dispose()

