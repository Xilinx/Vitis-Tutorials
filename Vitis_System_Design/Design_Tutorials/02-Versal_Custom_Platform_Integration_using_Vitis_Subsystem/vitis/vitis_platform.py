#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author Derek Hagen

# Vitis CLI to build a platform for embedded SW
import os
import vitis

client = vitis.create_client()
client.set_workspace(path="build_pfm")

curr_dir = os.getcwd()
xsa_fixed = os.environ['XSA_FIXED']
linux_machine = os.environ['LINUX_MACHINE']
dtsi_dir = curr_dir+"/../linux/src/recipes-bsp/device-tree/files/system-user.dtsi"
adv_options = client.create_advanced_options_dict(user_dtsi=dtsi_dir,dt_overlay="0",dt_zocl="1")
print(adv_options)
#platform = client.create_platform_component(name = "pfm",hw_design = curr_dir+"/../vivado/build/xsa_platform/vck190_thin_fixed.xsa",os = "linux",cpu = "psv_cortexa72",domain_name = "linux_xrt",generate_dtb = True,advanced_options = adv_options)
platform = client.create_platform_component(name = "pfm",hw_design = curr_dir+"/"+xsa_fixed,os = "linux",cpu = "psv_cortexa72",domain_name = "linux_xrt",generate_dtb = True,advanced_options = adv_options)
xrt_domain = platform.get_domain(name="linux_xrt")
status = xrt_domain.set_bif(path=curr_dir+"/../linux/src/boot_custom.bif")
status = xrt_domain.set_boot_dir(path=curr_dir+"/../linux/"+linux_machine+"/images/linux")
bare_domain = platform.add_domain(cpu = "psv_cortexa72_0",os = "standalone",name = "standalone",display_name = "standalone")

status = platform.build()

vitis.dispose()
