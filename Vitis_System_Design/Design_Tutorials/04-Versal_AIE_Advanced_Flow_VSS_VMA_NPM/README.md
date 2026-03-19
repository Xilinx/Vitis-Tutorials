# Flexible AI Engine Development: From Vitis to Vivado with Advanced Control


### Overview

This project demonstrates a complete workflow for integrating AI Engine (AIE) designs using the **Vitis Subsystem (VSS) → Vitis Metadata Archive (VMA) → Vivado Non-Project Mode (NPM)** flow. This workflow enables independent AIE development, seamless transition from Vitis to Vivado, and advanced control for experienced hardware designers.


### Key Concepts

**VSS (Vitis Subsystem)**
- Makes AIE development **independent** and self-contained
- **No dependency on platform** during AIE development
- AIE design becomes a **standalone IP** that can be integrated later
- Enables parallel development of AIE and other components

**VMA (Vitis Metadata Archive)**
- Archive format containing **Vitis metadata** exported after completing AIE/HLS and Platform linking development in Vitis
- Enables seamless **transition from Vitis to Vivado** for timing closure, debugging, and integration


**NPM (Vivado Non-Project Mode)**
- Designed for **advanced users** who want full control over design data and manual state tracking
- Vivado runs entirely **in-memory** without project-based management
- No automatic intermediate files or reports - you control everything via Tcl scripts
- Typical usage: `vivado -mode batch -source script.tcl`
- Project features disabled: source/run management, OOC synthesis, cross-probing, design state reporting
- **Note:** If block design is already generated with OOC option, it can be added to non-project flow. Otherwise, BD must be generated before adding to non-project flow.

## Architecture Flow

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  AIE Graph  │ --> │  VSS Build  │ --> │  Platform   │ --> │  VMA        │ --> │  NPM Flow   │
│  (libadf.a) │     │  (vss)      │     │  (hw.xsa)   │     │  (post_link)│     │  (Vivado)   │
└─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘
```

### Design Components

1. **[aie_dummy](./aie_dummy)**: AIE kernel implementation
   - Simple dummy kernel that generates AXI Stream data output
   - Demonstrates basic AIE graph creation and configuration
   - Output: `libadf.a` (AIE library archive)

2. **[vss_integration](./vss_integration)**: Complete Vitis-Vivado integration pipeline
   - **vss/**: Vitis Subsystem build with AIE graph (platform-independent, can be used as standalone IP)
   - **platform/**: Custom platform creation with AXI Stream interfaces
   - **link_vma/**: VMA generation (links VSS with platform)
   - **post_link_iteration/**: Vivado project import and RTL integration
   - **npm/**: Vivado Non-Project Mode workflow

## Prerequisites

- **Version: Vitis 2025.2** 
- **Version: Vivado 2025.2** 
- **Versal device** (tested on VEK280)

## Quick Start

### 1. Build AIE Kernel

```bash
# From project root
cd aie_dummy
make compile
cd ..
```

This generates `libadf.a` required for VSS build.

### 2. Build Complete Flow

```bash
# Build all integration steps
cd vss_integration
make all
```

This will execute the complete pipeline:
1. VSS build (Vitis Subsystem with AIE, platform-independent)
2. Platform build (custom platform with AXI Stream interfaces)
3. VMA generation (Vitis Metadata Archive for Vivado import)
4. Post-link iteration (Vivado import VMA and RTL integration)
5. NPM build (Vivado Non-Project Mode)

### 3. Individual Step Builds

```bash
# Build specific steps
make vss              # Step 1: Build VSS (platform-independent)
make platform         # Step 2: Build platform
make link_vma         # Step 3: Generate VMA
make post_link_iteration  # Step 4: Import VMA to Vivado
make npm              # Step 5: Build with NPM
```

## Project Structure

```
VIvado_NPM_Vitis/
├── aie_dummy/              # AIE kernel source
│   ├── src/
│   │   ├── kernels/        # AIE kernel implementations
│   │   ├── inc/            # Header files
│   │   └── test.cpp        # Graph definition
│   └── Makefile
├── vss_integration/        # Vitis-Vivado integration
│   ├── platform/           # Custom platform creation
│   ├── vss/                # VSS build
│   ├── link_vma/           # VMA generation
│   ├── post_link_iteration/# Vivado import
│   ├── npm/                # Non-Project Mode
│   └── tests/              # Test scripts
└── Makefile                # Top-level build orchestration
```

## Detailed Documentation

For detailed information about each step, refer to:

- **VSS Integration Overview**: [vss_integration/README.md](vss_integration/README.md)
- **Platform Build**: [vss_integration/platform/README.md](vss_integration/platform/README.md)
- **VSS Build**: [vss_integration/vss/README.md](vss_integration/vss/README.md)
- **VMA Generation**: [vss_integration/link_vma/README.md](vss_integration/link_vma/README.md)
- **Post-Link Iteration**: [vss_integration/post_link_iteration/README.md](vss_integration/post_link_iteration/README.md)
- **NPM Flow**: [vss_integration/npm/README.md](vss_integration/npm/README.md)

## Build Targets

### Top-Level Makefile

```bash
make all              # Build all components (AIE + VSS Integration)
make aie_dummy        # Build AIE dummy component only
make vss_integration  # Build Vitis-Vivado Integration designs
make clean            # Clean all build artifacts
make help             # Show available targets
```

### VSS Integration Makefile

```bash
cd vss_integration
make all              # Build all integration steps
make vss              # Build VSS component
make platform         # Build Platform component
make link_vma         # Build Link VMA component
make post_link_iteration  # Build Post Link Iteration
make npm              # Build NPM component
make clean            # Clean all components
make help             # Show help message
```

## Key Features

-  **Complete VSS→VMA→NPM workflow** demonstration
-  **Independent AIE development** with VSS (no platform dependency)
-  **Seamless Vitis-to-Vivado transition** via VMA (Vitis Metadata Archive)
-  **Advanced control** with Vivado Non-Project Mode for experienced users
-  **Custom platform creation** with AXI Stream interfaces
-  **AIE-only design** (no PL kernels required)


## Troubleshooting

### Common Issues

1. **AIE compilation fails**: Ensure `libadf.a` is generated before VSS build
2. **Platform build errors**: Verify base platform (VEK280) is available
3. **VMA import fails**: Check that all dependencies are built in order

### Build Order

Always follow this order:
```
aie_dummy → vss → platform → link_vma → post_link_iteration → npm
```

**Note**: VSS is platform-independent and can be built before platform. VSS and platform can be built in parallel.

## References

- **UG994**: [Vivado IPI with Non-Project Mode](https://docs.amd.com/r/en-US/ug994-vivado-ip-subsystems/Creating-a-Flow-in-Non-Project-Mode)
- **Vitis Tutorials**: [Versal Vitis Subsystem Flow](https://github.com/Xilinx/Vitis-Tutorials/tree/2025.1/Vitis_System_Design/Design_Tutorials/02-Versal_Vitis_Subsystem_Flow)
- **Vitis Documentation**: [Vitis Unified Software Platform Documentation](https://docs.amd.com/)



<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>