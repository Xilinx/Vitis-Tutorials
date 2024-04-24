#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

import vitis

client = vitis.create_client()
client.set_workspace(path="./workspace")
    

proj = client.get_sys_project(name="simple_aie_application_system_project")

proj_cfg =  client.get_config_file(proj.project_location+'/package/package.cfg')
proj_cfg.set_value(section='package', key='ps_elf', value='../../../../A-to-Z_app/build/A-to-Z_app.elf,a72-0')
proj_cfg.set_value(section='package', key='defer_aie_run', value='true')
proj_cfg.set_value(section='package', key='enable_aie_debug', value='false')

#proj.build(target="hw")

vitis.dispose()