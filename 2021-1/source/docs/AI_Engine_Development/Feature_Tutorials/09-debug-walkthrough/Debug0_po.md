<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis-AI™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Migrate Command-Line Based Projects to Vitis IDE

This section documents the necessary steps to migrate command-line based projects to Vitis™ IDE.

[Step 1. Create project](./#Step-1-Create-project)

[Step 2. Import project source code](./#Step-2-Import-project-source-code)

[Step 3. Configure project](./#Step-3-Configure-project)

  [Step 3.1. Configure AI Engine sub-project](./#Step-3_1-Configure-AIE-sub-project)

  [Step 3.2. Configure hardware link](./#Step-3_2-Configure-hardware-link)

  [Step 3.3. Configure hardware kernels](./#Step-3_3-Configure-hardware-kernels)

  [Step 3.4. Configure PS sub-project](./#Step-3_4-Configure-PS-sub-project)

[Step 4. Configure system project in packaging options](./#Step-4-Configure-system-project-in-packaging-options)

## Step 1. Create project

Launch Vitis IDE by issuing the command, `vitis`.

![missing image](./images/po_new.png)

![missing image](./images/po_platform.png)

![missing image](./images/po_name.png)

![missing image](./images/po_template.png)

After the previously listed steps are completed, the created system project is expected to have AI Engine and hardware link sub-projects.

![missing image](./images/po_system.png)

Then we need to create PS and PL sub-projects under system project.

![missing image](./images/po_system_np.png)

![missing image](./images/po_system_ps.png)

Fill in sysroot, rootfs, and image information for PS application.

![missing image](./images/po_system_ps_sysroot.png)

Select "Empty Application" under "Acceleration templates with PL and AIE accelerators" for PS sub-project.
After the previously listed steps are completed, you should have AI Engine, PS, PL and hardware link sub-projects under system project.

![missing image](./images/po_expect_project.png)


## Step 2. Import project source code

Import source code for AI Engine sub-projects.

![missing image](./images/po_import.png)

![missing image](./images/po_import1.png)

Continue to import source code/data for PS, PL, and hardware link sub-projects.

**Note:** If the PS sub-project requires `defer-aie-run` option in packaging, `aie_control_xrt.cpp` is required to be imported in the PS sub-project.

## Step 3. Configure project

### Step 3.1. Configure AI Engine sub-project

Configure top level graph.
![missing image](./images/po_graph.png)

Configure include paths.

![missing image](./images/po_aie_c_config.png)

![missing image](./images/po_aie_c_config1.png)

### Step 3.2. Configure hardware link

Configure hardware link with `v++` option

![missing image](./images/po_hw_config.png)

![missing image](./images/po_hw_config1.png)

Import `system.cfg` file to the proper project/location.

![missing image](./images/po_hw_config2.png)

### Step 3.3. Configure hardware kernels

Add hardware functions to project.

![missing image](./images/po_hw_config3.png)

Note: If hardware functions are inside the graph, check mark "PL Kernel in AIE Graph" from this configuration.

Configure number of instances for each PL kernel.
![missing image](./images/po_hw_config4.png)

Update (comment out) `nk=kernel:1:kernel_n` line from the `system.cfg` file. This is because the Vitis IDE relies on the previous step to determine the number of instances for each PL kernel. Redundant configuration of number of kernels in `system.cfg` results compilation errors.
![missing image](./images/po_hw_config5.png)

### Step 3.4. Configure PS sub-project

![missing image](./images/po_ps_c_config.png)

Configure PS application include paths.

![missing image](./images/po_ps_c_config2.png)

Configure PS application link libraries.

![missing image](./images/po_ps_c_config3.png)

## Step 4. Configure system project in packaging options

![missing image](./images/po_system_config.png)

**Note:** This configuration is optional and depends on the design.

1. If initialize is required, run, and end from PS, then use the `--package.defer_aie_run` option.

2. If data files need to be packaged to the sd_card, then use the `--package.sd_dir ${PROJECT_PATH}/data` option.

# License

Licensed under the Apache License, Version 2.0 (the "License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD005 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
