#!/bin/sh

# Script assumes SDX being in search path as well as the device
# it is also assumed that the script is run in the reference-files directory

# Make kernel
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 kernel

# Build and run pipeline version
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 pipeline
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 pipelineRun

# Build and run sync version
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 sync
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 syncRun

# Build and run buf version
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 buf
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 SIZE=14 bufRun
make TARGET=hw DEVICE=xilinx_u200_gen3x16_xdma_1_202110_1 bufRunSweep
