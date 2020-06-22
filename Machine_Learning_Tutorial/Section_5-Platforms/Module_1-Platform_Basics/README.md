# Section 5 - Designing Vitis Embedded Platforms

*2020.1*

## Module 1 - Platform Basics

A platform is a starting point of your Vitis design. Vitis applications are built on top of the platforms.

The embedded platform components and architecture introduction can be referred to [Using 2020.1 Embedded Platforms]().

Xilinx has provided base platforms for Xilinx demo boards. The platforms can be downloaded from Xilinx Download Center. Source code of base platforms can be downloaded from Xilinx github.

User can create custom embedded platforms from scratch, or modify Xilinx embedded platforms. 

To create a custom embedded platform for acceleration flow from scratch, these base components need to be prepared.

- A base hardware design exported from Vivado
- Software platform components, e.g. Linux kernel, root file system, device tree, etc.

Base components need to boot up the board successfully. Base components of Xilinx demo boards can be provided by PetaLinux BSP.

The Vitis platform creation is to add some additional properties to base components to meet the requirements of Vitis. In general, the steps of platform creation would involve the following steps.

1. Add hardware interface parameters and interrupt support in Vivado project and export XSA
2. Update software platform components to enable application acceleration software stacks: enable XRT, update device tree, etc
3. Package and generate platform with XSCT commands or Vitis GUI

The properties and software stacks added above are used by Vitis to recognize the resources in platforms and take control of them.

The following paragraphs will introduce the requirements of each step.

## Requirements of Each Platform Creation Step

### Step 0: Base Components

As long as the base components can boot your board properly, they have  been well prepared. The base system can be simple or complex. Vitis will not touch what exist in the base system but only to add more components and link to its open interfaces.

Below is a table of workflows you may go through to make the base system work.

|                         | Development                                                  | Validation                                                   |
| ----------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Basic board bring-up    | Processor basic parameter setup                              | Standalone Hello world and Memory Test application run properly |
| Advanced hardware setup | Enable advanced IO in CIPS (USB, Ethernet, Flash, PCIe RC, etc)Add IO related IP in PL (MIPI, EMAC, HDMI_TX, etc)Add non-Vitis IP (AXI BRAM Controller, VPSS, etc)Setup NOC for best performance connection | If these IP has standalone driver, test them.                |
| Base software setup     | Create PetaLinux project based on hardware platformEnable kernel driversConfigure boot modeConfigure rootfs | Linux boot up successfully. Peripheral work properly in Linux |

#### Base Components Requirements

- Every hardware platform design must contain a Processing System IP block from the IP catalog.
  - Versal ACAP, Zynq® UltraScale+™ MPSoC and Zynq-7000 SoC devices are supported.
  - MicroBlaze processors are not supported for controlling acceleration kernels, but can be part of the base hardware.

#### Directory Structure

While not in any way strictly mandated, since platform creation requires the integration of multiple toolchain outputs for this tutorial we'll use the following directory structure:

- **vivado** will contain the Vivado project itself
- **petalinux** will contain the PetaLinux project for the base software image
- **generics** will contain some 'boilerplate' template files that are used during the platform generation process
- **scripts** will contain all of the "additive" Tcl scripts required to package the platform

### Step 1: Add Hardware Interfaces

Here we come the real Vitis platform baking process.

The following table shows what Vitis can understand and what's minimal required for an acceleration embedded platform.

|                      | Vitis can understand                                         | Minimum Requirements for AXI MM Kernels |
| -------------------- | ------------------------------------------------------------ | --------------------------------------- |
| Control Interfaces   | AXI Master Interfaces from PS or from AXI Interconnect IP or SmartConnect IP | One AXI Lite Master for control         |
| Memory Interfaces    | AXI Slave Interfaces                                         | One memory interface                    |
| Streaming Interfaces | AXI Stream Interfaces                                        | Not required                            |
| Clock                | Multiple clock signals                                       | One clock                               |
| Interrupt            | Multiple interrupt signals                                   | One Interrupt                           |



#### General Requirements

- Every IP used in the platform design that is not part of the standard Vivado® IP catalog must be local to the Vivado Design Suite project. References to IP repository paths external to the project are not supported by the `write_hw_platform` command.
- Any external hardware port interface to the Vitis platform must be an AXI4, AXI4-Lite, AXI4-Stream, clock, reset type interface.
  - Custom bus-type or hardware interfaces must remain internal to the platform design and cannot be declared for use by the `v++` linker.

#### Control Interface Requirements

- Every platform must declare at least one general-purpose AXI master port (M_AXI_GP) from the Processing System IP, or have an interconnect IP connected to such an AXI master port. These are used for software control of accelerator IP.
  - AXI Master port (M_AXI_GP) is defined as an element of `PFM.AXI_PORT` .
  - sptags property for M_AXI_GP port is not supported.

#### Memory Interface Requirements

- Every platform must declare at least one memory interface with AXI Slave port (S_AXI). It's used to exchange data between ARM processors in PS and kernels in PL. PS DDR is used in most cases.

  - AXI Slave port (S_AXI) is defined as an element of `PFM.AXI_PORT` 
  - sptags property for S_AXI interfaces is supported by `v++` . `v++` can find the specific memory interface during linking stage by sptags name. If sptags property is not specified in hardware design, v++ can still link the design with default linking strategy.

- Example of PFM.AXI_PORT setting for control interface and memory interface

```tcl
set_property PFM.AXI_PORT {
M_AXI_HPM1_FPD {memport "M_AXI_GP"} 
S_AXI_HPC0_FPD {memport "S_AXI_HPC" sptag "HPC0" memory "ps_e HPC0_DDR_LOW"}  
S_AXI_HPC1_FPD {memport "S_AXI_HPC" sptag "HPC1" memory "ps_e HPC1_DDR_LOW"}  
S_AXI_HP0_FPD {memport "S_AXI_HP" sptag "HP0" memory "ps_e HP0_DDR_LOW"}  
S_AXI_HP1_FPD {memport "S_AXI_HP" sptag "HP1" memory "ps_e HP1_DDR_LOW"}  
S_AXI_HP2_FPD {memport "S_AXI_HP" sptag "HP2" memory "ps_e HP2_DDR_LOW"}
} [get_bd_cells /ps_e]
```

  



#### Streaming Interface Requirements

- AXI streaming interface is supported in Vitis platforms but it's not required.
- Streaming kernel interfaces are specified with the `PFM.AXIS_PORT` sptag interface property and a matching `connectivity.sc` command argument to the `v++` linker.

#### Clock Requirements

- At least one clock needs to be enabled in `PFM.CLOCK` property.
- At least one clock needs to be set as default in `PFM.CLOCK` property

Example code

```tcl
set_property PFM.CLOCK {pl_clk0 {id "0" is_default "true" proc_sys_reset "/proc_sys_reset_0" status "fixed"}} [get_bd_cells /zynq_ultra_ps_e_0]
```



#### Interrupt Requirements

Vitis provides a way to automatic connect kernel's output IRQ signal to interrupt controller automatically during link stage. It's accomplished by a post link script `dynamic_postlink.tcl`. The interrupt controller setup needs to follow the requirements of this script so that the automation can work in Vitis link stage. From 2020.1, this tcl script is not required explicitly in Vivado project as a hook script because it's included by Vivado tools. But the design guideline and requirements remain the same.

- Use `dynamic_postlink.tcl` as `platform.post_sys_link_tcl_hook` (not required since 2020.1 because it's a default action for v++ --link) 
- An AXI interrupt controller IP is required to collect kernel IRQ. Connect output of AXI Interrupt Controller to PS interrupt input port.
- The input of AXI interrupt IP should follow these requirements so that 
  - hierarchy name: **interrupt_concat** 
  - Concat IP name: **xlconcat_interrupt_0** 
  - Concat inputs are driven by constant IP with name **xlconstant_gnd** 
  - Signal between **xlconcat_interrupt_0** and **xlconstant_gnd** should be named as **xlconcat_interrupt_0**
- To let software can utilize these interrupts, device tree needs to updated to include zocl node. It's explained in [Software Components requirement chapter](#update-device-tree-for-zocl).



#### Versal Specific Requirements



#### AI Engine Requirements

### Step 2: Update Software Components

#### Add XRT to Root Filesystem

Vitis acceleration application uses XRT to control hardware. XRT provides a unified programming interface across data center to embedded use cases.

We need to add XRT kernel driver (zocl) and user space library (xrt-dev) to rootfs and sysroot. Package xrt-dev would enable user to compile Vitis applications that utilizes XRT API.

#### Update Device Tree for ZOCL

Zocl requires a device tree node to enable the interrupt connection.



Example of zocl device node

```
&amba {
	zyxclmm_drm {
		compatible = "xlnx,zocl";
		status = "okay";
		interrupt-parent = <&axi_intc_0>;
		interrupts = <0  4>, <1  4>, <2  4>, <3  4>,
			     <4  4>, <5  4>, <6  4>, <7  4>,
			     <8  4>, <9  4>, <10 4>, <11 4>,
			     <12 4>, <13 4>, <14 4>, <15 4>,
			     <16 4>, <17 4>, <18 4>, <19 4>,
			     <20 4>, <21 4>, <22 4>, <23 4>,
			     <24 4>, <25 4>, <26 4>, <27 4>,
			     <28 4>, <29 4>, <30 4>, <31 4>;
	};
};
```

Please refer to XRT documentation https://xilinx.github.io/XRT/master/html/yocto.html for more details.

#### Declare Platform with /etc/xocl.txt

XRT checks `/etc/xocl.txt` in embedded platform rootfs to know which platform it is. Platform name needs to be written into `/etc/xocl.txt`. This information will be used for compatibility check when loading xclbin file for kernel configuration.

#### Setup XILINX_XRT environment variable

XRT uses XILINX_XRT environment variable to find the XRT libraries. On embedded platforms, XILINX_XRT needs to point to `/usr`.

#### Adjust CMA Size

XRT uses CMA for buffer object allocation. Please reserve sufficient memory for CMA in bootargs or device tree to prevent running out of memory during acceleration application run time.

### Step 3: Wrap a Vitis Acceleration Platform

With all requirements prepared for Vitis acceleration platforms, we can wrap them together and generate the final Vitis acceleration platform. This can be done in either Vitis GUI or XSCT.

Platform is an encapsulation of multiple hardware and software components. It's easier to hand off deliveries from hardware oriented engineers to application developers with this capsulation.

The following files and information will be packaged into platform.

- Hardware Specification: Unified XSA file
- Software components are added to the platform as a Linux domain that enables OpenCL runtime. It includes
  - Linux components
    - rootfs (optional)
      - Additional user files to be packed into SD card
    - Kernel image (optional)
    - Sysroot (optional) for cross compiling
  - Boot Components
    - BIF file
    - boot components directory that includes all the files described in BIF file
  - Emulation support files (optional)

#### Root Filesystem

FAT32 and Ext4 partition types are supported by Vitis. The root filesystem is optional in platform creation step because it can be assigned during Vitis application creation step. 

A base rootfs image directory needs to be set during platform creation. All contents in this directory will be packaged into final SD card image. If the target file system is FAT32, the files will be placed to SD card root directory; if the target file system is Ext4, the files will be placed to root directory of the first FAT32 partition.

#### Boot Components

A BIF file needs to be provided so that the application build process can package the boot image. 

Example of BIF file:

```
/* linux */
the_ROM_image:
{
  [fsbl_config] a53_x64
  [bootloader] <fsbl.elf>
  [pmufw_image] <pmufw.elf>
  [destination_device=pl] <bitstream>
  [destination_cpu=a53-0, exception_level=el-3, trustzone] <bl31.elf>
  [destination_cpu=a53-0, exception_level=el-2] <u-boot.elf>
}

```

A boot components directory that includes all the files described in the BIF should also be provided. In this example, it should provide fsbl.elf, pmufw.elf, bl31.elf and u-boot.elf. These boot components can be generated by PetaLinux.



### Step 4: Test Your Platform

Please run some basic platform tests to make it works properly for acceleration applications before delivering the platform to application developers.

Generally, we need to make sure the platform can pass these tests:

- Boot test: Vivado project generated implementation result BIT file by step 1 and PetaLinux generated images by step 2 should be able to boot till Linux console successfully.
- Platforminfo test: The platform generated by step 3 should have proper platforminfo report for clock information and memory information.
- XRT basic test: XRT utility `xbutil query` should be able to run on target board and report platform information properly.
- Vadd test: Use Vitis to generate vector addition sample application with the platform. The generated application and xclbin should print test pass on the board.



### Special Considerations

#### Divide logic functions to Platform and Kernel

While the designs on FPGA and SoC are getting more complex, it's common that multiple developers or teams would work on a design together. Vitis software platform provides a clear boundary for application developers and platform developers. The platform developers may include board developers, BSP developers, system software developers, etc.

In system architect's view, some logic functions may be in a grey area: they can be packaged grouped with platforms, or they can work as an acceleration kernel. To help divide the system blocks, here are some general guidelines.

- The basic consideration for classify a function as a kernel or platform is whether this is an application related logic. For example, an application algorithm acceleration module is probably a kernel, while a general acceleration module that provides multiple acceleration features for application developers to choose during run time is probably better in platform.
- Platforms should be stable. Application function changes should only happen in software and kernel.
- Platforms abstract hardware. When changing a hardware board, application should need very little change to target to the new hardware.
- Follow constraints and limitations of Vitis, e.g.
  - Only three types of interfaces are supported by Vitis acceleration kernels: AXI MM, AXI Lite, AXI stream
  - AXI Kernel doesn't support external IO pins (GT kernel not included)



| Logic                                                        | Platform              | Kernel                                                     |
| ------------------------------------------------------------ | --------------------- | ---------------------------------------------------------- |
| Hard Processors(PS of ZYNQ and MPSoC)                        | Only in Platform      |                                                            |
| Soft Processors                                              | Preferred in Platform | OK as an RTL kernel                                        |
| IO Block (External pins, MIPI interface IP, etc)             | Only in Platform      |                                                            |
| 2nd IP for IO Block (DMA for PCIe, MAC for Ethernet, Video Pipeline IP, etc) | OK in Platform        | OK as Kernel                                               |
| IP with non-AXI interface                                    | Only in Platform      | OK if the interface can be changed to AXI MM or AXI Stream |
| Traditional memory mapped IP which has Linux driver(VPSS, etc) | Only in Platform      |                                                            |
| HLS AXI memory mapped IP                                     | OK in Platform        | Preferred as Kernel                                        |
| Acceleration memory mapped IP follows Vitis kernel register standard and open to XRT |                       | Preferred as Kernel                                        |
| Vitis Libraries                                              |                       | Only work as Kernel                                        |
| Free running IP with AXI Stream interface                    | OK                    | OK                                                         |



#### Emulation