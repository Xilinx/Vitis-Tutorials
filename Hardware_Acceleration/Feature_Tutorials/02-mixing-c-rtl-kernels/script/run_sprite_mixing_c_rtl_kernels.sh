#!/bin/sh
echo ======================================
echo == STEP 1
echo ======================================
echo
# Change directory
cd ../reference-files/run1
echo ~~~~~~~~~~~~~~~~~~~
echo ~~ Clean
echo ~~~~~~~~~~~~~~~~~~~
make clean
echo
echo

echo ~~~~~~~~~~~~~~~~~~~
echo ~~ Build application
echo ~~~~~~~~~~~~~~~~~~~

make all TARGET=sw_emu

echo ~~~~~~~~~~~~~~~~~~~
echo ~~ Run emulation
echo ~~~~~~~~~~~~~~~~~~~
setenv XCL_EMULATION_MODE sw_emu
./host krnl_vadd.sw_emu.xilinx_u200_gen3x16_xdma_1_202110_1.xclbin


