#!/bin/bash
export XILINX_XRT=/usr
cd /mnt/sd*1
./test_bf_app.exe 0 aie.xclbin
