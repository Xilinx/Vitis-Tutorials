# Vivado Non-Project Mode Flow

This step demonstrates how to use Vitis exported VMA with Vivado Non-Project Mode (NPM).

## Flow Overview

This step is the **fifth and final step** in the VSS→VMA→NPM integration flow. It builds the design using Vivado Non-Project Mode for advanced control and customization.

**Position in the flow**: 
```
AIE (libadf.a) → Platform → VSS → VMA → Vivado Import → NPM (this step)
```

**What this step does**:
1. Reads the block design generated from VMA import (from post_link_iteration step)
2. Reads custom RTL files (axis_sink.v, top.v, ILA core)
3. Synthesizes the design using NPM (entirely in-memory, no project files)
4. Implements the design (place and route)
5. Generates device image and exports XSA
6. Packages final PDI with PL device image and AIE CDO

**Why this step is needed**: 
- **Advanced control**: NPM provides maximum control over synthesis, placement, routing, and timing closure
- **Manual state management**: You control every aspect of the implementation flow via Tcl scripts
- **No project overhead**: Runs entirely in-memory without project file management
- **Customization**: Essential for complex designs requiring manual optimization and custom implementation strategies
- **Reproducibility**: Tcl scripts provide complete control and reproducibility

**Key characteristic**: NPM runs entirely in-memory. There are no project files (`.xpr`), no automatic intermediate file management, and no design state tracking. Everything is controlled explicitly through Tcl scripts.

**Output**: 
- Device image: `top.pdi`
- Fixed XSA: `top.xsa`
- Final PDI: `package_hw/a.pdi` (complete programmable device image)

**When to use NPM**: 
- You need full control over the implementation flow
- You want to customize synthesis, placement, or routing strategies
- You prefer script-based workflows over GUI-based project management
- You need reproducible, version-controlled build processes

## Dependencies

**Required files from post_link_iteration step**:
- `vitis_design_vma.bd`: Block design file generated during VMA import
- `axis_sink.v`: Custom RTL consumer module
- `top.v`: Top-level RTL module
- `axis_ila_0.xci`: ILA IP core configuration

**Why these dependencies**: NPM reads the same design files created in post_link_iteration, but builds them using Non-Project Mode instead of project mode.

## Makefile Commands

### `make all`
- `make build`: Runs NPM build script
- `make package`: Generates final PDI

### `make build`
**Command**: `vivado -mode batch -source ./vivado_npm.tcl`

**Output**: `top.xsa`

### `make package`
**Command**: `v++ -p -t hw -f ./top.xsa --package.no_image --package.out_dir package_hw <VMA_FILE> <LIBADF_FILE>`

**Output**: `package_hw/a.pdi`

## Key TCL Commands

**NPM build script (`vivado_npm.tcl`)**:
```tcl
read_bd <BD_FILE>                    # Loads BD without project file
generate_target all [get_files <BD>]  # Generates wrapper and constraints
make_wrapper -files [get_files <BD>] # Creates top-level wrapper
read_verilog <FILE>                   # Adds custom RTL
read_ip <XCI_FILE>                    # Loads IP core
synth_design -top top -part <PART>    # Synthesizes (in-memory)
opt_design                            # Optimizes
place_design                          # Places design
route_design                          # Routes design
write_device_image -file ./top.pdi   # Generates PDI
write_hw_platform -fixed -file ./top.xsa  # Exports fixed XSA
```
**NPM**: Runs entirely in-memory, no `.xpr` project file, full control via Tcl

## Build Process

1. Execute `vivado_npm.tcl` script in batch mode
2. Read block design and RTL files (no project file needed)
3. Synthesize, place, and route design
4. Generate device image and export XSA
5. Package with AIE library to create final PDI

**NPM characteristics**: Runs entirely in-memory, no `.xpr` project file, full control via Tcl scripts.

## Source Files

**`vivado_npm.tcl`**: Main NPM build script that orchestrates the entire build process.

**Files read from post_link_iteration**:
- `vitis_design_vma.bd`: Block design from VMA import
- `axis_sink.v`: Custom RTL consumer
- `top.v`: Top-level RTL
- `axis_ila_0.xci`: ILA IP core

**Why read from post_link_iteration**: The block design and custom RTL are created in post_link_iteration. NPM reads these same files but builds them using Non-Project Mode instead of project mode.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–206 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>