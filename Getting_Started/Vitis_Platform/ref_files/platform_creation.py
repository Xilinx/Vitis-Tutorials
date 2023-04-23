#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

# 2023-04-02T21:02:13.258137
import vitis
import os

print("Platform generation")
platform_name = "vck190_custom"
xsa_name="vck190_custom.xsa"
xsa_path=os.path.join(os.getcwd(),"build/vivado",xsa_name)
print(xsa_path)
boot="boot"
boot=os.path.join(os.getcwd(),boot)
sd_dir="sd_dir"
sd_dir=os.path.join(os.getcwd(),sd_dir)
client = vitis.create_client()
print(boot)
print(sd_dir)
client.set_workspace(path=os.getcwd())


platform = client.create_platform(name = platform_name, hw =xsa_path, os = "linux",cpu = "psv_cortexa72")

platform = client.get_platform_component(name=platform_name)
domain = platform.get_domain(name="linux_psv_cortexa72")
status = domain.update_name(new_name="xrt")
status = domain.add_boot_dir(boot_dir=boot)

status = domain.set_sd_dir(path=sd_dir)
status = domain.add_qemu_data(data_dir=boot)
status = domain.generate_bif() 
status = platform.build()

