#
# Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis
import sys
import os

app_path= os.getcwd()

client = vitis.create_client()

platform_name=sys.argv[1]
design_part=sys.argv[2]
target_sys=sys.argv[3]

client.set_workspace(path="./workspace_"+design_part)

platform_path=app_path+"/workspace_"+design_part+"/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"

#
#   Create System Project
#
proj = client.create_sys_project(name="system_project", platform=platform_path, template="empty_accelerated_application", packaging_mode="petalinux")
proj = client.get_sys_project(name="system_project")
status = proj.add_container(name="binary_container_1")
proj = proj.add_component(name="simple_aie_app", container_name=['binary_container_1'])

#
#   Edit linker configuration
#
cfg = client.get_config_file(proj.project_location+'/hw_link/binary_container_1-link.cfg')
cfg.add_values(section='connectivity', key='sc', values=['master_axi_1:ai_engine_0.PLIO_i_0'])
cfg.add_values(section='connectivity', key='sc', values=['ai_engine_0.PLIO_o_0:slave_axi_1'])

if (design_part=="4"):
    cfg.add_values(section='debug', key='aie.chipscope', values=['PLIO_i_0'])
    cfg.add_values(section='debug', key='aie.chipscope', values=['PLIO_o_0'])
#
#   Edit Packager configuration
#
cfg = client.get_config_file(proj.project_location+'/package/package.cfg')

#if (target_sys=="hw"):
#    cfg.set_value(section='package', key='enable_aie_debug', value='false')

proj.build(target = target_sys)

vitis.dispose()
