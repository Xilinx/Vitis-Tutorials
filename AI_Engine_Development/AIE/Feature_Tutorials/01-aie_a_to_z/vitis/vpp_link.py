#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
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
    if cmd_args == 2:
        target_sys="hw"
    else:
        target_sys=sys.argv[2]


platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

#
#   Create System Project
#

if (not os.path.isdir("./workspace/simple_aie_application_system_project")):
    proj = client.create_sys_project(name="simple_aie_application_system_project", platform=platform_path, template="empty_accelerated_application")
    proj = client.get_sys_project(name="simple_aie_application_system_project")
    status = proj.add_container(name="binary_container_1")

    proj = proj.add_component(name="s2mm", container_name=["binary_container_1"])
    proj = proj.add_component(name="mm2s", container_name=["binary_container_1"])
    proj = proj.add_component(name="simple_aie_application", container_name=["binary_container_1"])

    cfg = client.get_config_file(proj.project_location+'/hw_link/binary_container_1-link.cfg')
    cfg.set_value(section='advanced', key='param', value='compiler.addOutputTypes=hw_export')
    cfg.add_values(section='connectivity', key='sc', values=['mm2s_1.s:ai_engine_0.mygraph_in'])
    cfg.add_values(section='connectivity', key='sc', values=['ai_engine_0.mygraph_out:s2mm_1.s'])
else:
    proj = client.get_sys_project(name="simple_aie_application_system_project")

# To create the PS Application Vitis needs the XSA of HW. Thus when HW emu is select HW target is also built
if target_sys=="hw_emu":
    proj.build(target = "hw_emu")

proj.build(target = "hw")


