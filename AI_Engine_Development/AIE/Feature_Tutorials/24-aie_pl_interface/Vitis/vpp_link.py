#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
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
# Create Embedded Platform for dummy PS app
#
platform = client.create_platform_component(name = "vck190_hw",hw_design = "vck190",os = "standalone",cpu = "psv_cortexa72_0",domain_name = "standalone_psv_cortexa72_0")
platform = client.get_component(name="vck190_hw")
status = platform.build()

#
# Create dummy PS app
#
comp = client.create_app_component(name="dummy_ps_app",platform = app_path+"/workspace_"+design_part+"/vck190_hw/export/vck190_hw/vck190_hw.xpfm",domain = "standalone_psv_cortexa72_0")
comp = client.get_component(name="dummy_ps_app")
status = comp.import_files(from_loc="./src/ps_app", files=["dummy_ps_app.c", "platform.c", "platform.h"], dest_dir_in_cmp = "src")
comp.build()

#
#   Create System Project
#
proj = client.create_sys_project(name="system_project", platform=platform_path, template="empty_accelerated_application")
proj = client.get_sys_project(name="system_project")
status = proj.add_container(name="binary_container_1")
proj = proj.add_component(name="simple_aie_app", container_name=['binary_container_1'])

#
#   Edit linker configuration
#
cfg = client.get_config_file(proj.project_location+'/hw_link/binary_container_1-link.cfg')
cfg.add_values(section='connectivity', key='sc', values=['master_axi_1:ai_engine_0.PLIO_i_0'])
cfg.add_values(section='connectivity', key='sc', values=['ai_engine_0.PLIO_o_0:slave_axi_1'])

#
#   Edit Packager configuration
#
cfg = client.get_config_file(proj.project_location+'/package/package.cfg')
cfg.set_value(section='package', key='enable_aie_debug', value='false')
cfg.add_values(section='package', key='ps_elf', values=[app_path+"/workspace_"+design_part+"/dummy_ps_app/build/dummy_ps_app.elf,a72-0"])

proj.build(target = target_sys)

vitis.dispose()
