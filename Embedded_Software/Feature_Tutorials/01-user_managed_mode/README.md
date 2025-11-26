<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Embedded Software Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# User Managed Mode in Vitis Unified Embedded IDE

***Version: Vitis 2025.2***

This is a new feature with the Vitis Unified Embedded IDE, where users can add a makefile and argument that can be used to create custom builds. This allows the user more control over their project builds instead of letting the tools generate the components.

The biggest advantages of user managed mode are being able to do the following:

1. Quickly rebuild a Vitis Unified Workspace in the GUI.
2. Quickly call makefile actions.

This tutorial describes three use cases for the **User Managed Mode**.

## Use Case 1 - Regenerate a previously created Vitis Unified Workspace using a Python Script

### Let’s get started

Launch Vitis 2025.2, select **open_workspace**, and navigate to the `Vitis-Tutorials/Embedded_Software/Feature_Tutorials/01-user_managed_mode` directory. The workspace will load in the 'User Maged Mode' view.

![](./images/user_managed_mode.PNG)

### Generating Build Configuration

In this use case, we shall be using the Python script introduced in the *04-vitis_scripting_flows* to build a workspace and with a simple Hello World application template. Therefore firstly, you will want to copy the `unified_workspace.py` and `create_xsa.tcl` files to your *01-user_managed_mode* workspace.However, users can also utilize the Python script `logs/workspace_journal.py` that gets generated for every Vitis Unified IDE workspace.  Make sure you have already executed the **create_xsa.tcl** to create the hardware design.

On the left hand side you should see the **User Managed Mode View**. If the **User Managed Mode View** is not there then select the three dots in the explorer view and select **User Managed** which will load the **User Managed Mode View**. 

![](./images/three_dots.png)

Right click in the **Explorer** view, right-click and select **Edit Build Configurations** and **New Build Configuration** and set similar as shown below. 

![](./images/ws_build_config.PNG)

### Running a Build Configuration

To launch the build configuration, right click on the **Explorer** view, and then select **Build**.

**Note:** If there is only a single build configuration, then this will launch automatically. If there is more than one build configuration, a list is shown that you can choose from. 

This will generate the Vitis Unified IDE workspace based on the XSA.

### Deploying the application on the target

Next, we can create a **Launch Configuration** to test the application created above on our hardware target.

First you will need to launch the **unified_workspace** that was created in the previous step. Next right click in the **Explorer** view, and select **Edit Launch Configurations** and **New Launch Configuration** and select **Embedded Baremetal Application**. In **Debug Options** select **Configure the device and start debugging** and browse to your XSA file used to create the workspace above..

![](./images/create_launch_config.PNG)

Here, we can add the FSBL (as this is for a Zynq UltraScale Device)

![](./images/launch_config.PNG)

Users can select on the + icon and add the ELF as shown below

![](./images/add_elf.PNG)

Once the launch configuration is complete, users can select the **debug** icon to deploy and debug the hello world application

![](./images/debug.PNG)

The Debug view will be launched, and users can debug as normal

![](./images/debug_view.PNG)

## Use Case 2 - Running Platform Creation tutorials

In this use case, we can build the example using the scripts given [here](https://github.com/Xilinx/Vitis-Tutorials/tree/2024.2/Vitis_Platform_Creation/Design_Tutorials/02-Edge-AI-ZCU104/ref_files/step1_vivado). In particular, we will build the **Edge-AI-ZCU104** Platform.

### Lets get started

Launch Vitis 2025.2, select **open_workspace**, and navigate to the `Vitis-Tutorials/Embedded_Software/Feature_Tutorials/01-user_managed_mode` directory

Select the **Open Explorer View**

![](./images/user_managed_mode.PNG)

### Generating Build Configuration

In this use case, copy the **Makefile**, **export_xsa.tcl** and the **system_step1.tcl** from the [Edge-AT-ZCU104 repository](https://github.com/Xilinx/Vitis-Tutorials/tree/2024.2/Vitis_Platform_Creation/Design_Tutorials/02-Edge-AI-ZCU104/ref_files/step1_vivado) into the workspace.

Right click in the **Explorer** view, and select **Edit Build Configurations** and **New Build Configurations** and set as shown below:

![](./images/zcu104.PNG)

### Running a Build Configuration

To launch the build configuration, right click on the **Explorer** view and select **Build**. 

**Note:** If there is only a single build configuration, then this will launch automatically. If there are more than one build configurations, then the user will be presented with a list to choose from. 

This will generate the XSA into the **Build** folder.

Users can target these newly generated XSA filed in Vitis Unified IDE. Users can switch views **View** → **Vitis Components**, and target the platform for their development.


## Use Case 3 - Generate and deploy u-boot from Git sources

In this demo I have a simple makefile to generate and deploy u-boot using the metadata extracted from the XSA file. The script will do the following

* Download Git sources
* Generate the Platform (with FSBL)
* Generate the device tree files
* Compile device-tree
* Compile ARM Trusted Firmware
* Compile U-boot
* Generate JTAG boot script

Each section above can be built independently. For example, if users want to make a change to the device-tree, then only the DT needs to be compiled and re-deployed on the board. This allows the users to speed up any u-boot debug or testing. 

### Lets get started

Launch Vitis 2025.2, select **open_workspace**, and navigate to the `Vitis-Tutorials/Embedded_Software/Feature_Tutorials/01-user_managed_mode` directory

Select the **Open Explorer View**

![](./images/user_managed_mode.PNG)


### Creating a Build Configuration

If users want to create a Build Configuration, then right click in Explorer view -> Edit Build Configurations, and select the + icon. Users will be presented with an empty Build Configuration. For example, if we wanted to created a build configuration to run the boot script in XSCT

```
jtag_boot:
	vitis -s ./boot_script.py
``` 

**Note:** You must run `make generate_boot_script` before running `make jtag_boot`

The build files we are using are within the **scripts** directory therefore you will need to append 'cd scripts &&' before calling the make command. Create the build configuration as follows

![](./images/jtag_boot_build_config.PNG)


### Running a Build Configuration

To run a build configuration, right click in Explorer View and select <i>Build</i> and select your build configuration from the list

![](./images/build_configs.PNG)

### Running a Clean Configuration

Similarly, to run a clean configuration, right click in Explorer View and select <i>Clean</i> and select your Clean configuration from the list


## Sharing Build Configurations.

If users are using Git to maintain their work, then users may want to maintain or share the build configurations. This metadata is stored in the **_ide/.theia/build.json** file

## Summary

The examples above should act as an introduction to how to utilize the User Managed Mode for different design flows. However, whether the users preference is to use the GUI or Command Line to build and maintain their Embedded System, they can still avail of all the debug features within Vitis Unified IDE with ease. 

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>



