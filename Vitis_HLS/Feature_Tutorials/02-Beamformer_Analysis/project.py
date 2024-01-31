#!/usr/bin/env python3
import vitis
import os
import time
import shutil
from datetime import datetime

print("\n-----------------------------------------------------")
print("HLS Feature Tutorial 01: Beamformer Analysis\n")

# Create names
tmpdir = 'tmp'
date = datetime.now().strftime("%Y%m%d%I%M%S")
workspace_name = 'workspace_'+date
workspace = os.path.join(tmpdir, workspace_name)
comp_name = "hls_component"
cwd = os.getcwd()

# Delete the workspace if already exists
if (os.path.isdir(tmpdir)):
    shutil.rmtree(tmpdir, ignore_errors=True)
    print(f"Deleted workspaces in {tmpdir}")

# Print names
print(f"CWD is {cwd}")
print(f"Workspace path is {workspace}")
print(f"Creating component {comp_name}")

# Create a client object
client = vitis.create_client()

# Set Workspace
client.set_workspace(workspace)

# Create HLS Component
comp = client.create_hls_component(name=comp_name)

# Get config file object
cfg_path = os.path.join(workspace, comp_name, 'hls_config.cfg')
cfg_obj = client.get_config_file(cfg_path)

# Set part
cfg_obj.set_value('',key = 'part', value='xcvp1202-vsva2785-1LP-i-L')

# Add kernel sources, test bench files, set top
source_dir = os.path.join(cwd, "reference_files")
cfg_obj.add_lines('hls',['syn.file='+source_dir+'/beamformer.cpp'])
cfg_obj.add_lines('hls',['tb.file='+source_dir+'/beamformer_tb.cpp'])
cfg_obj.add_lines('hls',['tb.file='+source_dir+'/result.golden_float.cpp'])

# Set options
cfg_obj.set_value('hls',key='syn.top', value='beamformer')
cfg_obj.set_value('hls',key='clock', value='3ns')
cfg_obj.set_value('hls',key='flow_target', value='vivado') #
cfg_obj.set_value('hls',key='package.output.format', value='ip_catalog') #
cfg_obj.set_value('hls',key='package.output.syn', value='false') #

# Add compiler directives
cfg_obj.add_lines('hls',['syn.directive.pipeline=beamformer/L1 II=1'])
arrays = ['rx_q', 'rx_i', 'beamso_q', 'beamso_i']
for array in arrays:
    cfg_obj.add_lines('hls',['syn.directive.array_reshape=beamformer '+array+' dim=2 type=complete'])

# Execute the desired steps:
comp.execute('C_SIMULATION') 
comp.execute('SYNTHESIS')

# Clean up, delete the workspace
if (os.path.isdir(workspace)):
    shutil.rmtree(workspace, ignore_errors=True)
    print(f"Deleted workspaces in {workspace}")

# Close the client connection and terminate the vitis server
vitis.dispose()
