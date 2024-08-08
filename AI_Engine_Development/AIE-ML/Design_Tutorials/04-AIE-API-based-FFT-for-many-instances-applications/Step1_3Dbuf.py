#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Donato Carlo Giorgio

import vitis
import os

# Current directory and various path variables
cwd = os.getcwd()+'/'
vek280_base = os.environ.get('PLATFORM_REPO_PATHS') + '/xilinx_vek280_base_202410_1/xilinx_vek280_base_202410_1.xpfm'
my_workspace = cwd + 'AIEML_API_FFT_Tutorial'
sources = cwd + 'src/'

#                           Creating the component
#______________________________________________________________________________
# Delete component folder if already exists
os.system('rm -rf '+my_workspace+"/FFT_128_3D")   
# Initializing vitis and creating workspace
client = vitis.create_client()
client.set_workspace(path=my_workspace)
# Creating 3D buffer AIE component
fft_component = client.create_aie_component(name="FFT_128_3D", platform=vek280_base, template="empty_aie_component")
# Importing sources
fft_component.import_files(from_loc=sources, dest_dir_in_cmp=(fft_component.component_location+'/src'))
fft_component.update_top_level_file(top_level_file="src/fft1k_128_graph.cpp")
# Updating the configuration file
aiecomp_cfg = client.get_config_file(fft_component.component_location+'/aiecompiler.cfg')
aiecomp_cfg.set_value(section='aie', key='pl-freq', value="500")


#                           Functional verification
#______________________________________________________________________________
fft_component.build(target="x86sim")


#                            Timing verification
#______________________________________________________________________________
fft_component.build(target="hw")