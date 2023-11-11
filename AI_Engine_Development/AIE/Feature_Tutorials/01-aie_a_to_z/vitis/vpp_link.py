#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

app_path= os.getcwd()
cmd_args=len(sys.argv)
args=str(sys.argv)

client = vitis.create_client()
client.set_workspace(path="./workspace")

if cmd_args == 1:
    platform_name="base_pfm_vck190"
else:
    platform_name=sys.argv[1]

platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"
s2mm_config = app_path+"/src/s2mm_hls_config.cfg"
mm2s_config = app_path+"/src/mm2s_hls_config.cfg"

#
#   Create MM2S HLS PL Kernel Component
#

comp = client.create_hls_component(name="mm2s", cfg_file = [mm2s_config], template = "empty_hls_component")

comp = client.get_component(name="mm2s")
comp.execute(operation="SYNTHESIS")
comp.execute(operation="PACKAGE")

#
#   Create S2MM HLS PL Kernel Component
#

comp = client.create_hls_component(name="s2mm", cfg_file = [s2mm_config], template = "empty_hls_component")

comp = client.get_component(name="s2mm")
comp.execute(operation="SYNTHESIS")
comp.execute(operation="PACKAGE")

#
#   Create System Project
#

proj = client.create_sys_project(name="simple_aie_application_system_project", platform=platform_path, template="empty_accelerated_application")
proj = client.get_sys_project(name="simple_aie_application_system_project")

status = proj.add_container(name="binary_container_1")

#Next Steps are not ready in Vitis 2023.2
#proj = proj.add_component(name="s2mm", containers=["binary_container_1"])
#proj = proj.add_component(name="mm2s", containers=["binary_container_1"])
#proj = proj.add_component(name="simple_aie_application", containers=["binary_container_1"])
#status = proj.import_files(from_loc=app_path+"/src/", files=["hw_link_config.cfg"], dest_dir_in_cmp = "hw_link")
#status = proj.add_cfg_files(cfg_files=["hw_link_config.cfg"], name="binary_container_1")
#proj.build(target = "hw_emu")


