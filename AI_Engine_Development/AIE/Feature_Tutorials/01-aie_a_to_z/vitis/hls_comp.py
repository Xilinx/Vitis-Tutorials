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

part="xcvc1902-vsva2197-2MP-e-S"

client = vitis.create_client()
client.set_workspace(path="./workspace")

if cmd_args == 1:
    platform_name="base_pfm_vck190"
else:
    platform_name=sys.argv[1]

platform_path=app_path+"/workspace/"+platform_name+"/export/"+platform_name+"/"+platform_name+".xpfm"
hls_src_loc=app_path+"/../src/"

#
#   Create MM2S HLS PL Kernel Component
#

if (not os.path.isdir("./workspace/mm2s")):
    comp = client.create_hls_component(name="mm2s", cfg_file = ["mm2s_config.cfg"], template = "empty_hls_component")

comp = client.get_component(name="mm2s")

hls_comp_cfg = client.get_config_file(comp.component_location+'/mm2s_config.cfg')
hls_comp_cfg.set_value(key='part', value=part)
hls_comp_cfg.set_value(section='hls', key='flow_target', value='vitis')
hls_comp_cfg.set_value(section='hls', key='package.output.format', value='xo')
hls_comp_cfg.set_value(section='hls', key='package.output.syn', value='true')
hls_comp_cfg.set_values(section='hls', key='syn.file', values=[hls_src_loc+'mm2s.cpp'])
hls_comp_cfg.set_value(section='hls', key='syn.top', value='mm2s')


comp.run(operation="SYNTHESIS")
comp.run(operation="PACKAGE")

#
#   Create S2MM HLS PL Kernel Component
#

if (not os.path.isdir("./workspace/s2mm")):
    comp = client.create_hls_component(name="s2mm", cfg_file = ["s2mm_config.cfg"], template = "empty_hls_component")
    
comp = client.get_component(name="s2mm")

hls_comp_cfg = client.get_config_file(comp.component_location+'/s2mm_config.cfg')
hls_comp_cfg.set_value(key='part', value=part)
hls_comp_cfg.set_value(section='hls', key='flow_target', value='vitis')
hls_comp_cfg.set_value(section='hls', key='package.output.format', value='xo')
hls_comp_cfg.set_value(section='hls', key='package.output.syn', value='true')
hls_comp_cfg.set_values(section='hls', key='syn.file', values=[hls_src_loc+'s2mm.cpp'])
hls_comp_cfg.set_value(section='hls', key='syn.top', value='s2mm')

comp.run(operation="SYNTHESIS")
comp.run(operation="PACKAGE")

vitis.dispose()



