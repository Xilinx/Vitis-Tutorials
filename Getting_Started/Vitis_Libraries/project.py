#!/usr/bin/env python3
import vitis
import os
import time
import shutil
from datetime import datetime


# Create names
tmpdir = './tmp'
date = datetime.now().strftime("%Y%m%d%I%M%S")
workspace_name = 'workspace_'+date
workspace = os.path.join(tmpdir, workspace_name)
comp_name = "hls_component"
cwd = os.getcwd()
cfg_name = 'project.cfg'
cfg_path = os.path.join(cwd, cfg_name)

# Delete the workspace if already exists
if (os.path.isdir(tmpdir)):
    shutil.rmtree(tmpdir, ignore_errors=True)
    print(f"Deleted workspaces in {tmpdir}")

# Print names
print(f"CWD is {cwd}")
print(f"Workspace path is {workspace}")
print(f"Creating component {comp_name}")
print(f"Using config file {cfg_path}")

# Create a client object
client = vitis.create_client()

# Set Workspace
client.set_workspace(workspace)

# Create HLS Component
comp = client.create_hls_component(name=comp_name, cfg_file = cfg_path)

# Execute the desired steps:
comp.run('C_SIMULATION')
comp.run('SYNTHESIS')
comp.run('CO_SIMULATION')
#comp.run('PACKAGE')
#comp.run('IMPLEMENTATION')

# Clean up, delete the workspace
if (os.path.isdir(tmpdir)):
    shutil.rmtree(tmpdir, ignore_errors=True)
    print(f"Deleted workspaces in {tmpdir}")

# Close the client connection and terminate the vitis server
vitis.dispose()
