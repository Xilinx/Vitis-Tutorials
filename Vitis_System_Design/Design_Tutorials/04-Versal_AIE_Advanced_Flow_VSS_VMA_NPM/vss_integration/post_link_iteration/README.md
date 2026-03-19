# Vivado project import VMA

This step works on the custom Vivado design based on the Vitis region VMA.

## Flow Overview

This step is the **fourth step** in the VSS→VMA→NPM integration flow. It imports the VMA into Vivado and adds custom RTL components to create a complete design.

**Position in the flow**: 
```
AIE (libadf.a) → Platform → VSS → VMA → Vivado Import (this step) → NPM
```

**What this step does**:
1. Imports the VMA into a Vivado extensible platform project
2. Adds custom RTL components:
   - `axis_sink.v`: Consumer for AIE AXI Stream output
   - `axis_ila_0.xci`: ILA (Integrated Logic Analyzer) core for debugging AIE output
   - `top.v`: Top-level RTL connecting custom logic with VMA wrapper
3. Synthesizes and implements the complete design
4. Generates device image and exports fixed XSA
5. Packages final PDI with PL device image and AIE CDO

**Why this step is needed**: 
- **Custom RTL integration**: After Vitis generates the base design, you often need to add custom logic, debugging cores, or additional interfaces
- **Complete design**: Combines Vitis-generated design with custom RTL into a single implementable design
- **Debugging support**: Adds ILA cores for real-time debugging of AIE interfaces



**Output**: 
- Fixed XSA: `project_1/top.xsa` (used in NPM step)
- Final device image: `package_hw/a.pdi` (contains PDI file)

**Note**: Making a copy of the original Vivado platform project is optional. Making a copy is helpful for source control and verification for this demo. Please decide whether to make a copy according to your design requirements.

## Build

```Bash
make all
```

## Makefile Commands

### `make all`
- `make build`: Imports VMA and builds design
- `make package`: Generates final PDI

### `make build`
**Command**: `vivado -mode batch <PLATFORM_PROJ> -source ./import_vma.tcl`

**Output**: `project_1/top.xsa`

### `make package`
**Command**: `v++ -p -t hw -f project_1/top.xsa --package.no_image --package.out_dir package_hw <VMA_FILE> <LIBADF_FILE>`

**Output**: `package_hw/a.pdi`

## Key TCL Commands

**VMA import (`import_vma.tcl`)**:
```tcl
vitis::import_archive ../link_vma/post_link.vma
```
- Imports VMA, creates block design `vitis_design_vma.bd`
- `generate_target all`: Generates HDL wrapper and constraints
- `make_wrapper`: Creates top-level wrapper
- `launch_runs impl_1`: Synthesizes, places, routes
- `write_hw_platform -fixed`: Exports fixed XSA

## Source Files

### `import_vma.tcl`
Main script that imports the VMA into Vivado:

```tcl
vitis::import_archive ../link_vma/post_link.vma
```

**What it does**:
- Uses `vitis::import_archive` Tcl command to import VMA
- Creates block design from VMA metadata
- Generates wrapper files for the VMA design
- Sets up the extensible platform project structure

### `vivado_build.tcl`
Script for rebuilding without re-importing (used by `make rebuild`):
- Runs synthesis
- Runs implementation
- Exports fixed XSA

### Custom RTL Files

**`axis_sink.v`**: 
- Consumer module for AIE AXI Stream output
- Receives data from AIE through AXI Stream interface
- Demonstrates how to connect custom logic to AIE interfaces

**`axis_ila_0.xci`**: 
- ILA (Integrated Logic Analyzer) IP core configuration
- Used for debugging AIE AXI Stream output
- Captures real-time signal data for analysis

**`top.v`**: 
- Top-level RTL module
- Connects `axis_sink.v` with `vitis_design_vma_wrapper.v` (generated from VMA)
- Instantiates ILA core for debugging
- Creates the complete design hierarchy

## Build Process

1. Import VMA using `vitis::import_archive` (creates block design)
2. Add custom RTL files (axis_sink.v, top.v, ILA core)
3. Synthesize and implement design
4. Export fixed XSA file
5. Package with AIE library to generate PDI

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>