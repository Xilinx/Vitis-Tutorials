# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import vitis
import shutil
import os

workspace = os.path.join(os.getcwd(), "linux_debug_ws")
workspace = os.path.normpath(workspace)
    
print("Creating workspace at " + workspace)
if os.path.exists(workspace):
    print("workspace exists. Deleting ...")
    shutil.rmtree(workspace)

client = vitis.create_client()
client.set_workspace(workspace)

platform = client.create_platform_component(name = "zcu102_test",hw = "zcu102.xsa",os = "linux",cpu = "psu_cortexa53",no_boot_bsp = True)
platform = client.get_platform_component(name="zcu102_test")
status = platform.build()

sysroots = os.path.join(os.getcwd(), "sysroots/cortexa72-cortexa53-xilinx-linux")
sysroots = os.path.normpath(sysroots)

status = client.set_preference(level = "WORKSPACE", device = "ZynqMP", key = "Sysroot", value = sysroots)

xpfm = os.path.join(os.getcwd(), "linux_debug_ws/zcu102_test/export/zcu102_test/zcu102_test.xpfm")
xpfm = os.path.normpath(xpfm)
comp = client.create_app_component(name="linux_test_app",platform = xpfm,domain = "linux_psu_cortexa53",template = "linux_empty_application")

status = comp.set_sysroot(sysroot=sysroots)

comp = client.get_component(name="linux_test_app")
status = comp.import_files(from_loc="src", files=["linux_test_application.c"], dest_dir_in_cmp = "src")