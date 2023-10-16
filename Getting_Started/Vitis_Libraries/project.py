import vitis
import os
import shutil


client = vitis.create_client()
workspace='./myProject'

#Delete the workspace if already exists.
if (os.path.isdir(workspace)):
    shutil.rmtree(workspace)
    print(f"Deleted workspace {workspace}")

client.set_workspace(workspace)

cwd = os.getcwd()
print(f"CWD is {cwd}")

cfg_path = os.path.join(cwd, 'project.cfg')
print(f"Config file is {cfg_path}")

comp = client.create_hls_component(name="impulseTest", cfg_file = cfg_path)
comp.execute(operation="C_SIMULATION")
comp.execute(operation="SYNTHESIS")
comp.execute(operation="CO_SIMULATION")

