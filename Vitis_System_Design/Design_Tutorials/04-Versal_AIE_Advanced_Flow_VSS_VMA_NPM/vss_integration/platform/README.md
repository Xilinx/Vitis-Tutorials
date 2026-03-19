# Creating a custom platform

This design demonstrates how to create a custom platform for the VSS design that only contains the AI Engine in the VSS. It adds the AI Engine data path required AXI Stream interfaces to the platform so that the modules outside of Vitis Region can connect to the interfaces within Vitis Region.

## Flow Overview

This step is the **second step** in the VSS→VMA→NPM integration flow. It creates a custom hardware platform that will be used later to link with the VSS subsystem.

**Position in the flow**: 
```
AIE (libadf.a) → VSS → Platform (this step) → VMA → Vivado Import → NPM
```

**What this step does**:
1. Starts from VEK280 base platform (extensible platform)
2. Adds AXI Stream interfaces in the Vitis Region
3. Configures Platform Feature Metadata (PFM) properties for v++ linking
4. Exports the platform as an XSA (Xilinx Support Archive) file

**Why this step is needed**: 
- The base VEK280 platform doesn't have the AXI Stream interfaces required for AIE communication
- Custom RTL modules outside the Vitis Region need to connect to AIE interfaces inside the Vitis Region
- The platform must define PFM properties so v++ can properly link the VSS subsystem to the platform interfaces

**Output**: `hw/build/hw.xsa` - A custom platform XSA file that will be used in the link_vma step

## Build

```Bash
cd hw
make all
```

## Source and Configuration Files

[xsa_scripts/xsa.tcl](hw/xsa_scripts/xsa.tcl)

```tcl
# Based on existing extensible platform, add AXIS interfaces and AXIS_VIP
source ../xsa_scripts/add_axis_vip.tcl
set NUM_IN  6
set NUM_OUT 6
create_axi_inputs  $NUM_IN
create_axi_outputs $NUM_OUT

# Generate Platform XSA
source ../xsa_scripts/generate_platform.tcl
```

[xsa_scripts/add_axis_vip.tcl](hw/xsa_scripts/add_axis_vip.tcl)

This file introduces two functions, allowing users to add any numbers of AXI Stream input or output interfaces to the platform BD that needs to connect to AI Engine data path directly.

Functions
* `create_axi_inputs  <num_inputs>`
* `create_axi_outputs  <num_outputs>`

Since the PFM property for v++ link cannot set on BD interfaces directly, we add AXIS_VIP in pass-through mode as the cell placeholder to attach `PFM` properties. The AXIS_VIP properties will be adjusted automatically to match the expectation of AI Engine AXIS interface setup - with TLAST and TKEEP, without TDEST.

## Makefile Commands

### `make all` or `make xsa`
**Command**: `vivado -mode batch -source xsa_scripts/xsa.tcl`

**Output**: `hw/build/hw.xsa`

## Key TCL Commands

**Platform build script (`xsa.tcl`)**:
```tcl
create_axi_outputs $NUM_OUT
```
- Creates AXI Stream interfaces with PFM properties
- Uses AXI4-Stream VIP in pass-through mode (PFM cannot be set directly on BD interfaces)
- Exports platform XSA with required metadata

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>