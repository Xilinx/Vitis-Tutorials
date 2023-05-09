#!/bin/sh


# this script should be run where the makefile resides 

############################################################
# Hardware emulation run 
############################################################
make all TARGET=hw_emu DEVICE=xilinx_u250_gen3x16_xdma_4_1_202210_1 


#############################################################
# Hardware run 
##############################################################

#make all TARGET=hw DEVICE=xilinx_u250_gen3x16_xdma_4_1_202210_1 


