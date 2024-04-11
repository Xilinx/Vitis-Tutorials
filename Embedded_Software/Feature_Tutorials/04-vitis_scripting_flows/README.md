<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Embedded Software Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a> </td>
 </tr>
</table>

***Version: Vitis 2023.2***

# Vitis Embedded Scripting Flows

Once users are comfortable using the GUI to generate their components, then the next step might be to automate this from command line. The Vitis has a CommandLine Interface (CLI) that users can use to do everything that was done in the Vitis GUI on commandline.

There are commandline API for creating Vitis components, API for extracting the hardware metadata from the SDT or XSA. There are also API for deploying and debugging the application in the XSDB debugger. There is a table of contents below to aid users to a specific set of API.

## Table of Contents

* [Component creation using the Vitis commands](#vitis-commands)
* [Metadata extraction using HSI API](#metadata-commands)
* [Deploy and Debug in XSDB from commandline](#xsdb-commands)

## Building Demo XSA file

There is a script attached to this tutorial if users want to rebuild the XSA used in this tutorial.

Launch Vivado 2023.2 and use the TCL commands below

```
cd scripts
source ./create_xsa.tcl
```

This will create a **design_1_wrapper.xsa** in the ../zcu102 folder

### Launching Vitis CommandLine Interface (CLI)

```
vitis -i
```

The Vitis Interactive shell is used in this tutorial for demonstration purposes. However, users can deploy a Python script from Vitis too using the command below

```
vitis -s path/to/python_script.py
```

<a name="vitis-commands"/>

## Component creation using the Vitis commands

The Vitis Unified IDE will compile a `builder.py` script with all the Vitis Python API commands to rebuild a workspace in the logs directory in the workspace. This can be used as a reference to generating the Vitis Python API script.

### Create Vitis Client

```
client = vitis.create_client()
```

### Client Help

Users can view the help as shown below

```
help(client)
```



### Create Vitis workspace

In Vitis, the default workspace is the current working directory. In the command line, we can just set this up

```
workspace = "path/to/workspace"
client.set_workspace(workspace)
```

Users can use the commands below to add the `vitis_ws` workspace to the current directory

```
import os
...
pwd = os.getcwd()
client.set_workspace(pwd + "/vitis_ws")
```

### Create Vitis Platform

> **Note**: Users can use the HSI Python API to extract the available processors from the XSA. This is discussed later in this tutorial

The Boot Artifacts will be automatically generated. For Zynq Ultrascale this will be the FSBL. If users dont want this, then use the `no_boot_bsp = True`

```
platform = client.create_platform_component(name = "base_platform",hw = "design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0")
```

### Opening an existing Vitis Platform

There are two use-cases here. If the platform was created above and the user just wants to show the details. Or, if we are opening a workspace and use/manipulate and existing platform

```
# If we have an existing platform, we can add this to the repo
platform_dirs = ['add path to platform']
client.add_platform_repos(platform_dirs)
 
# target the platform
platform = client.get_component(name = "name of platform")
 
# print platform details
platform.report()
```

### Add Software Repository

Users can add a software repository to add custom driver, bsp or libraries to the workspace

```
client.set_sw_repo(level="local", path="path to repo")
```

### List/get Vitis domains in a Platform

Users can list all the domains in the platform

```
#To list all domains in a platform
platform.list_domains()
```

This will return a list of all the domains within a platform. Users can iterate through this

```
for domain in platform.list_domains():
    print(domain)
```

Each domain is returned as a dictionary with keys; 'domain_name', 'display_name', 'processor', 'os. For example, we can return the domain names

```
for domain in platform.list_domains():
    print(domain['domain_name'])
```

Users can return a specific domain within a platform using the metadata above 

```
domain_object = platform.get_domain(name = "domain_name")
```

### Create Vitis domains in a Platform

Users can create a domain for a specific processor and OS. Supported OS are standalone, linux


> **Note**: Users can use the HSI Python API to extract the available processors from the XSA. This is discussed later in this tutorial.


```
domain = platform.add_domain(cpu = "psu_cortexa53_0",os = "standalone",name = "my_domain",display_name = "my_domain")
```

### Update hardware in existing Vitis Platform

When we generate a platform, the metadata is extracted from the XSA, and this is used to generate the S-DT. This will also rebuild any domains within the platform

```
platform.update_hw(hw = "path to new XSA")
```

### Building Platform

The command below will build the platform and all its domains. The platform should be built after all the domains are created.

```
platform = client.get_component(name = "name of platform")
status = platform.build()
```

### View or Manipulate Vitis Domains

There is a wide array of API that can be used here. These can be seen in the link above. I will just show a simple subset of these. For example, we can view the OS, drivers, libs

```
domain_object.get_os()
domain_object.get_drivers()
domain_object.get_libs()
```

### get_os

This will be delivered as a list. Users can iterate through this

```
for os in domain_object.get_os():
    print(os)
```

Each object in the list will be delivered as a dictionary with keys; 'os', 'current_path', 'possibleOptions'. For example, users can return all the OS

```
for os in domain_object.get_os():
    print(os['os'])
```

### get_drivers

This will be delivered as a list. Users can iterate through this

```
for driver in domain_object.get_drivers():
    print(driver)
```

Each object in the list will be delivered as a dictionary with keys; 'name', 'ip_type', 'path'. For example, users can return all the driver names

```
for driver in domain_object.get_drivers():
    print(driver['name'])
```

### get_libs

This will be delivered as a list. Users can iterate through this

```
for lib in domain_object.get_libs():
    print(lib)
```

Each object in the list will be delivered as a dictionary with keys; 'name', 'current_path. For example, users can return all the library names

```
for lib in domain_object.get_libs():
    print(lib['name'])
```

### Library/driver parameters

Users can view the parameters of a library/driver

```
# lib can be; lib, os, proc
domain_object.list_params("lib", "library name")
 
# for example, to list the os params
domain_object.list_params("os", "os name")
 
# for example, to list the processor params
domain_object.list_params("proc", "processor name")
```

For example, users can list all the os parameters. This will return a list of all the parameters. Each set of parameters is delivered as a dictionary with keys; 'parameter_name', 'description', 'default_value', 'value', 'possible_options', 'datatype', 'permission

> **Note**: Users can do dict_object.keys() to see a list of all the keys in a dictionary


```
os_params = domain_object.list_params("os", "standalone")
```

Users can iterate through the list, and list all the parameter names

```
for param in os_params:
    print(param['parameter_name'])
```

### get_config

For example, if users want to find a parameter value of a lib, os, or . 

```
stdout = domain_object.get_config(option = 'os', param = 'standalone_stdout')
stdout['value']
```

Users can use the same API to get a parameter on a specific library

```
ticktimer = domain_object.get_config(option = 'lib', lib_name = 'xiltimer', param = 'XILTIMER_en_interval_timer')
ticktimer['value']
```

### set_config

Users can set parameters on lib, os or proc.

For example, to set the stdin to a specific uart.

```
domain_object.set_config(option = 'os', param = 'standalone_stdout', value = 'psu_uart_1')
```

### Getting Compiler option on Domain

Users can use the `get_config` API to get the compiler options for the domain. This is useful if debugging drivers, or libraries

```
domain_object.get_config(option = 'proc', param = 'proc_compiler_flags')
domain_object.get_config(option = 'proc', param = 'proc_extra_compiler_flags')
```

### Setting Compiler option on Domain

Users can use the `set_config` API to set the compiler options for the domain. This is useful if debugging drivers, or libraries

```
domain_object.set_config(option = 'proc', param = 'proc_compiler_flags', value = 'value here')
domain_object.set_config(option = 'proc', param = 'proc_extra_compiler_flags', value = 'value here')
```

### Get list of Embedded Application Templates

> **Note**: Valid types are 'ACCL_APP'(Accelerated applications), 'HLS','AIE', 'EMBD_APP'(Embedded applications). 'ACCL_APP' is default type.


```
apps = client.get_templates(type = "EMBD_APP")
```

This will list of all the Embedded Application templates:

```
 'ddr_self_refresh',
 'dhrystone',
 'empty_application',
 'freertos_hello_world',
 'freertos_lwip_echo_server',
 'freertos_lwip_tcp_perf_client',
 'freertos_lwip_tcp_perf_server',
 'freertos_lwip_udp_perf_client',
 'freertos_lwip_udp_perf_server',
 'hello_world',
 'libmetal_echo_demo',
 'lwip_echo_server',
 'lwip_tcp_perf_client',
 'lwip_tcp_perf_server',
 'lwip_udp_perf_client',
 'lwip_udp_perf_server',
 'memory_tests',
 'openamp_echo_test',
 'openamp_matrix_multiply',
 'openamp_rpc_demo',
 'peripheral_tests',
 'rsa_auth_app',
 'srec_bootloader',
 'srec_spi_bootloader',
 'versal_plm',
 'versal_psmfw',
 'zynq_dram_test',
 'zynq_fsbl',
 'zynqmp_dram_test',
 'zynqmp_fsbl',
 'zynqmp_pmufw',
 'linux_hello_world'
```

### Creating Vitis Application

The API below will create an application built upon the domain created above. Users can use the `platform.list_domains` API to get a list of domains within the platform. The platform should be built `platform.build()` before this API is ran.

```
app = client.create_app_component(name="hello_world",platform = "base_platform/base_platform.xpfm",domain = "my_domain",template = "hello_world")
```

### Setting Application Compiler option

Users can set the application compiler options. For exmaple

```
app.set_app_config(key='USER_COMPILE_DEBUG_LEVEL', values=['-g3'])
app.set_app_config(key='USER_COMPILE_OTHER_FLAGS', values='"-fmessage-length=0 -MT"$@" -k"')
```

### Building Vitis Application

```
app = client.get_component(name="hello_world")
app.build()
```

<a name="metadata-commands"/>

## Metadata extraction using HSI API

The Vitis Python commands above allow users to create portable scripts that will build their Vitis workspace from commandline. Users can also take advantage of the Python HSI package to allow users to enhance their scripts to build workspaces across devices and even architectures. The Python HSI package allows users to extract the hardware metadata from the XSA. This can include the IP (called cells), IP pins, IP interfaces and even the interconnectivity between IP (nets) using some of the techniques discussed below

### Import HSI Python Package

```
import hsi
```

### Import XSA file

Users need to import the XSA container file exported from Vivado, and create a `HwDesign` object. Users can use the XSA in the prebuilt platforms in Vitis.

> **Note**: This created a Hardware Manager Object called "HwDesign" that users can use to extract the required HW metadata needed for their software system.

```
HwDesign = hsi.HwManager.open_hw_design("design_1_wrapper.xsa")
```

### Close Hardware Design

```
HwDesign.close()
```

### Get System Info from XSA

Users can read the system level information on the HwDesign object

```
Vitis [0]: HwDesign.report_property()
```

```
Property         Type     Read-only  Value
ADDRESS_TAG      string*  true       
BOARD            string   true       xilinx.com:zcu102:part0:3.4
CLASS            string   true       hw_design
DEVICE           string   true       xczu9eg
FAMILY           string   true       zynquplus
NAME             string   true       design_1_wrapper
PACKAGE          string   true       ffvb1156
PART             string   true       xczu9eg-ffvb1156-2-e
PATH             string   true       design_1.hwh
SPEEDGRADE       string   true       -2
SW_REPOSITORIES  string*  true       
TIMESTAMP        string   true       Wed Aug 16 16:42:32 2023
VIVADO_VERSION   string   true       2023.2
```

Users can return a specific property, such as the `FAMILY`

```
Vitis [0]: arch = HwDesign.FAMILY
```

```
Vitis [0]: print(arch)
zynquplus
```

### Get all IP in XSA

Users can get the hardware objects such as the cells, nets, pins, ports, interface pins, interface ports, ect. Below the `get_cells` is used to get all the cells, (or IP) in an XSA file.

```
cells = HwDesign.get_cells(hierarchical='true')
```

Users can use type(cells) or type(cells[0]) to see the object type

```
Vitis [0]: type(cells)
Out[0]: PyContainer

Vitis [0]: type(cells[0])
Out[0]: HwCell
```

### Get cell properties

```
cells[0].report_property()
```

This will look similar to below
```
Property                      Type     Read-only  Value
ADDRESS_TAG                   string   true       
BD_TYPE                       string   true       
CLASS                         string   true       cell
CONFIG_C_ALL_INPUTS           string   true       0
CONFIG_C_ALL_INPUTS_2         string   true       0
CONFIG_C_ALL_OUTPUTS          string   true       1
CONFIG_C_ALL_OUTPUTS_2        string   true       0
CONFIG_C_BASEADDR             string   true       0xA0000000
CONFIG_C_DOUT_DEFAULT         string   true       0x00000000
CONFIG_C_DOUT_DEFAULT_2       string   true       0x00000000
CONFIG_C_FAMILY               string   true       zynquplus
CONFIG_C_GPIO2_WIDTH          string   true       32
CONFIG_C_GPIO_WIDTH           string   true       8
CONFIG_C_HIGHADDR             string   true       0xA000FFFF
CONFIG_C_INTERRUPT_PRESENT    string   true       0
CONFIG_C_IS_DUAL              string   true       0
CONFIG_C_S_AXI_ADDR_WIDTH     string   true       9
CONFIG_C_S_AXI_DATA_WIDTH     string   true       32
CONFIG_C_TRI_DEFAULT          string   true       0xFFFFFFFF
CONFIG_C_TRI_DEFAULT_2        string   true       0xFFFFFFFF
CONFIG_Component_Name         string   true       design_1_axi_gpio_0_0
CONFIG_EDK_IPTYPE             string   true       PERIPHERAL
CONFIG_GPIO2_BOARD_INTERFACE  string   true       Custom
CONFIG_GPIO_BOARD_INTERFACE   string   true       led_8bits
CONFIG_USE_BOARD_FLOW         string   true       true
CONFIGURABLE                  bool     true       0
CORE_REVISION                 string   true       30
DRIVER_MODE                   string   true       
HIER_NAME                     string   true       
IP_NAME                       string   true       axi_gpio
IP_TYPE                       enum     true       PERIPHERAL
ISPDEFINST                    bool     true       0
IS_HIERARCHICAL               bool     true       0
IS_PL                         bool     true       1
MULTISOCKETSMP                string   true       
NAME                          string   true       axi_gpio_0
PRODUCT_GUIDE                 string   true       http://www.xilinx.com/cgi-bin/docs/ipdoc?c=axi_gpio;v=v2_0;d=pg144-axi-gpio.pdf
SLAVES                        string*  true       
SLR_NUMBER                    int      true       -1
VLNV                          string   true       xilinx.com:ip:axi_gpio:2.0
```

Again, users can print a specific cell object property

```
Vitis [0]: print(cells[0].CONFIG_C_BASEADDR)
0xA0000000
```


Users can also just use the filter the cells. For example, if users wanted to find a specific cell with IP_NAME as axi_gpio

```
axi_gpio = HwDesign.get_cells(hierarchical='true',filter='IP_NAME==axi_gpio')
```

This will return (if found) the cell object for the cell with property IP_NAME as axi_gpio

```
Vitis [0]: print(axi_gpio)
axi_gpio_0
```

A more useful filter, would be to filter by the IP_TYPE. This could be PERIPHERAL, PROCESSOR, ect. Users can use this filter to find all processors in the XSA

```
procs = HwDesign.get_cells(hierarchical='true',filter='IP_TYPE==PROCESSOR')
```

```
Vitis [0]: print(procs)
psu_cortexa53_0 psu_cortexa53_1 psu_cortexa53_2 psu_cortexa53_3 psu_cortexr5_0 psu_cortexr5_1 psu_pmu_0
```

### Get all IP in Address Map of Processor

Another popular use case is to get all the IP on a processor memory map. The procs object container created above.

```
Vitis [0]: memmap = HwDesign.get_mem_ranges(of_object=procs[0])
```

Here, users can see the list of all the IP on the memory map

```
Vitis [0]: print(memmap)
axi_gpio_0 psu_acpu_gic psu_adma_0 psu_adma_1 psu_adma_2 psu_adma_3 psu_adma_4 psu_adma_5 psu_adma_6 psu_adma_7 psu_afi_0 psu_afi_1 psu_afi_2 psu_afi_3 psu_afi_4 psu_afi_5 psu_afi_6 psu_ams psu_apm_0 psu_apm_1 psu_apm_2 psu_apm_5 psu_apu psu_can_1 psu_cci_gpv psu_cci_reg psu_coresight_0 psu_crf_apb psu_crl_apb psu_csu_0 psu_csudma psu_ctrl_ipi psu_ddr_0 psu_ddr_1 psu_ddr_phy psu_ddr_qos_ctrl psu_ddr_xmpu0_cfg psu_ddr_xmpu1_cfg psu_ddr_xmpu2_cfg psu_ddr_xmpu3_cfg psu_ddr_xmpu4_cfg psu_ddr_xmpu5_cfg psu_ddrc_0 psu_dp psu_dpdma psu_efuse psu_ethernet_3 psu_fpd_gpv psu_fpd_slcr psu_fpd_slcr_secure psu_fpd_xmpu_cfg psu_fpd_xmpu_sink psu_gdma_0 psu_gdma_1 psu_gdma_2 psu_gdma_3 psu_gdma_4 psu_gdma_5 psu_gdma_6 psu_gdma_7 psu_gpio_0 psu_gpu psu_i2c_0 psu_i2c_1 psu_iou_scntr psu_iou_scntrs psu_iousecure_slcr psu_iouslcr_0 psu_ipi_0 psu_lpd_slcr psu_lpd_slcr_secure psu_lpd_xppu psu_lpd_xppu_sink psu_mbistjtag psu_message_buffers psu_ocm psu_ocm_ram_0 psu_ocm_xmpu_cfg psu_pcie psu_pcie_attrib_0 psu_pcie_dma psu_pcie_high1 psu_pcie_high2 psu_pcie_low psu_pmu_global_0 psu_qspi_0 psu_qspi_linear_0 psu_r5_0_atcm_global psu_r5_0_btcm_global psu_r5_1_atcm_global psu_r5_1_btcm_global psu_r5_tcm_ram_global psu_rcpu_gic psu_rpu psu_rsa psu_rtc psu_sata psu_sd_1 psu_serdes psu_siou psu_smmu_gpv psu_smmu_reg psu_ttc_0 psu_ttc_1 psu_ttc_2 psu_ttc_3 psu_uart_0 psu_uart_1 psu_usb_0 psu_usb_xhci_0 psu_wdt_0 psu_wdt_1
```

### Get Pins on an IP

Users can use the `get_pins` API to get the pins in an XSA file. These can be pins at a high level

```
Vitis [0]: pins = HwDesign.get_pins(hierarchical='true')
```

```
Vitis [0]: print(pins)
s_axi_aclk s_axi_aresetn s_axi_awaddr s_axi_awvalid s_axi_awready s_axi_wdata s_axi_wstrb s_axi_wvalid s_axi_wready s_axi_bresp s_axi_bvalid s_axi_bready s_axi_araddr s_axi_arvalid s_axi_arready s_axi_rdata s_axi_rresp s_axi_rvalid s_axi_rready gpio_io_o ACLK ARESETN ...
```

Similar to the cells object previously, users can return the pin properties

```
Vitis [0]: pins[0].report_property()
```

```
Property      Type    Read-only  Value
CLASS         string  true       port
CLK_FREQ      string  true       99990005
DIRECTION     string  true       I
INTERFACE     bool    true       0
IRQID         string  true       
IS_CONNECTED  bool    true       1
LEFT          string  true       
NAME          string  true       s_axi_aclk
POLARITY      enum    true       
RIGHT         string  true       
SENSITIVITY   enum    true       
TYPE          enum    true       clk
```

Users can use this to make a better filter. For example, if users wantred to return all clk pins

```
Vitis [0]: clk_pins = HwDesign.get_pins(hierarchical='true',filter='TYPE==clk')
```

```
Vitis [0]: print(clk_pins)
s_axi_aclk ACLK S00_ACLK M00_ACLK slowest_sync_clk maxihpm0_fpd_aclk pl_clk0
```

Users can return the pins on a specfic object such as the axi_gpio cell used above

```
Vitis [0]: print(cells[0])
axi_gpio_0
```

```
Vitis [0]: axi_gpio_pins = HwDesign.get_pins(of_object=cells[0],filter='TYPE==clk')
```

```
Vitis [0]: print(axi_gpio_pins)
s_axi_aclk
```

Filtering on the `TYPE` pin property is also useful to find all interrupt pins

```
Vitis [0]: axi_gpio_pins = HwDesign.get_pins(of_object=cells[0],filter='TYPE==INTERRUPT')
```

Users can filter on the pin DIRECTION && TYPE

```
Vitis [0]: axi_gpio_in_clks = HwDesign.get_pins(of_object=cells[0],filter='DIRECTION==I&&TYPE==clk')
```

```
Vitis [0]: print(axi_gpio_in_clks)
s_axi_aclk
```

### Get Interface Pins on an IP

Users can use the `get_intf_pins()` API to read the interface pins of an IP such as the AXI Interfaces. Here, I used thre axi_gpio cell object discovered above

```
Vitis [0]: axi_gpio_intf_pins = HwDesign.get_intf_pins(of_object=cells[0])
```

```
Vitis [0]: print(axi_gpio_intf_pins)
S_AXI GPIO
```

Again, users can read the interface pin properties to read the downstream AXI interface of the axi_gpio cell object

```
Vitis [0]: ds_axi_gpio_intf_pins = HwDesign.get_intf_pins(of_object=cells[0],filter='TYPE==SLAVE')
```

```
Vitis [0]: print(ds_axi_gpio_intf_pins)
S_AXI_0
```

Similar API are shown below

* get_nets()
* get_intf_nets()
* get_ports()
* get_intf_ports()


<a name="xsdb-commands"/>

## Deploy and Debug firmware using XSDB Python API

All the commands available in the Vitis IDE for debugging are also available in the Vitis commandline (CLI) such as connect to a target, list targets, download data file, read/write to memory (or register), add breakpoints and debug. Users can use the HSI API above to create dynamic scripts that will work across different architectures. For example, users can extract the architecture from the XSA and use this info to property to connect to the target and download its binaries. Users can also read the device over the debugger.

### Import xsdb python package

```
import xsdb
```

### Start a session

```
session = xsdb.start_debug_session()
```

### XSDB Help

```
xsdb.help("functions")
```

### Connect to a hw_server

Users can connect to a hw_server running on a localhost, or remote (as shown below)

```
session.connect(url="TCP:lentinus15:3121")
```

If users are debugging remotely where the host machine and target device are not on the same machine, then it is recommended to use a symbol server

```
session.connect("--symbol", url="TCP:lentinus15:3121")
```


### Get all JTAG Targets

```
Vitis [0]: jtag = session.jtag_targets()
```

```
 1  Digilent JTAG-SMT2NC 210308A7B222 ()
  2  xczu9 (idcode 24738093 irlen 12 fpga)
  3  arm_dap (idcode 5ba00477 irlen 4)
```

Users can filter on the JTAG Targets

```
Vitis [0]: jtag = session.jtag_target(filter='name==arm_dap')
```

```
  3  arm_dap (idcode 5ba00477 irlen 4)
```

Users can return the target properties for a jtag target using the --target_properties (or -t) switch

```
Vitis [0]: jtag = session.jtag_target('-t', filter='name==arm_dap')
```

```
Vitis [0]: print(jtag)
{'jsn-JTAG-SMT2NC-210308A7B222-5ba00477-0': {'target_ctx': 'jsn-JTAG-SMT2NC-210308A7B222-5ba00477-0', 'level': 1, 'node_id': 3, 'is_open': 1, 'is_active': 1, 'is_current': 0, 'name': 'arm_dap', 'jtag_cable_name': 'Digilent JTAG-SMT2NC 210308A7B222', 'state': '', 'jtag_cable_manufacturer': 'Digilent', 'jtag_cable_product': 'JTAG-SMT2NC', 'jtag_cable_serial': '210308A7B222', 'idcode': '5ba00477', 'irlen': '4', 'is_fpga': 0, 'is_pdi_programmable': 0}}
```

This is a nested dict. If users wanted to get the idcode, then they would need traverse through the key

```
Vitis [0]: print(jtag["jsn-JTAG-SMT2NC-210308A7B222-5ba00477-0"]["idcode"])
```

Users can make this more dynamic.

```
Vitis [0]: key = list(jtag.keys())

Vitis [0]: print(jtag[key[0]]["idcode"])
5ba00477
```

Users can use this info to determine the arch of the target device.

```
Vitis [0]: if idcode == '4ba00477':
        ...:     print('zynq')
        ...: elif idcode == '5ba00477':
        ...:     print('zynqmp')
        ...: elif idcode == '6ba00477':
        ...:     print('versal')
        ...: else:
        ...:     print('error: unknown dap')
        ...: 
```

### Get all Targets

Users can list all the targets with the following command:

```
print(session.targets())
```

```
Vitis [0]: print(session.targets())
     1  PS TAP 
        2  PMU 
        3  PL 
     4  PSU 
        5  RPU (Reset) 
           6  Cortex-R5 #0 (RPU Reset)
           7  Cortex-R5 #1 (RPU Reset)
        8  APU (L2 Cache Reset) 
           9  Cortex-A53 #0 (APU Reset)
          10  Cortex-A53 #1 (APU Reset)
          11  Cortex-A53 #2 (APU Reset)
          12  Cortex-A53 #3 (APU Reset)
```

### Filter on a specific Target

To set a specific target, users can use the filter example as shown below

```
session.targets("--set", filter="name =~ *Cortex-A53 #0*")
```

### Dowload ELF to target

```
session.dow("test.elf")
```

### Dowload PDI to Versal target

```
session.device_program("test.pdi")
```

### Dowload Data file to target

The command below will download the test.bin to address 0x10000000 of the current target

```
session.dow("test.bin", "--data", addr=0x10000000)
```

### Memory read/write

For example, to read from address 0x0

```
session.mrd(address=0x0)
```

For example, to write 0x12345678 to address 0x0

```
session.mwr(address=0x0, words=0x12345678)
```

### Program Hello World ELF onto Zynq Ultrascale+

In the use case below, the XSDB API are used to connect to a Zynq Ultrascale target, The PMU is enabled in the target and the PMUFW ELF is downloaded. The FSBL ELF is downloaded to the Cortex A53 processor. There is a breakpoint added at the exit function in the FSBL. This will ensure that the FSBL is complete. Finally, the Hello World application is downloaded and executed.

```
import xsdb
session = xsdb.start_debug_session()
session.connect(url="TCP:lentinus15:3121")
 
# Add the Microblaze PMU to target
psu = session.targets("--set", filter="name =~ *PSU*")
psu.mwr(address=0xFFCA0038, words=0x1FF)

# Download PMUFW to PMU
pmu = session.targets("--set", filter="name =~ *MicroBlaze PMU*")
pmu.dow("zynqmp_pmufw.elf")
pmu.con()
 
# Download FSBL to A53 #0, block untill FSBL completes
a53 = session.targets("--set", filter="name =~ *Cortex-A53 #0*")
session.rst(type='cores')
a53.dow("zynqmp_fsbl.elf")
a53.bpadd(addr='XFsbl_Exit')
a53.con("--block", timeout = 60)

# Download hello world to A53 #0
a53 = session.targets("--set", filter="name =~ *Cortex-A53 #0*")
a53.dow("hello_world.elf")
a53.con()
```

### Program PDI onto Versal

Programming the Versal using a PDI is alot more staightforward than Zynq Ultrascale as all the binaries are placed in the PDI

```
import xsdb
session = xsdb.start_debug_session()
session.connect(url="TCP:morel18:3121")

# Download PDI to Versal Device
versal = session.targets("--set", filter="name =~ *Versal*")
versal.device_program("project_1.pdi")
```

### Using example script

There is an example `build_ws.py()` script that users can use as a reference to build a Vitis Unified IDE workspace. The script will create a hello_world application for a target XSA. The script also utilizes the HSI Python API to extract the HW metadata such as the architecture and processors from the XSA. It uses this metadata to control how the workspace is created.

Use the command below to launch the script

```
cd scripts
vitis -s create_unified_workspace.py path/to/xsa
```

> **Note**: This script is to be used as a reference only.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
