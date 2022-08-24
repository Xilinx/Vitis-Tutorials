#!/bin/sh


# this script should be run where the makefile resides 

############################################################
# Hardware emulation run 
############################################################
make all TARGET=hw_emu DEVICE=xilinx_u200_qdma_201910_1 


#############################################################
# Hardware run 
##############################################################

#make all TARGET=hw DEVICE=xilinx_u200_qdma_201910_1 


