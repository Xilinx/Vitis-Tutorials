#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                = @echo
WORKSPACE			:= workspace


# Makefile input options
TARGET 				:= hw_emu
INCLUDE_ILAS		:= 0
RTL_OUT_BD			:= 0
AXI4S_BROADCAST		:= 0


# XSA and Platform Name
ifeq ($(INCLUDE_ILAS), 0)
#if RTL_OUT_BD = 1 
	ifeq ($(RTL_OUT_BD), 1)
		DESIGN_PART			:= 2
		XSA					:= custom_pfm_strmIn_strmOut_RTL_out.xsa
		VITIS_PLATFORM		:= custom_pfm_strmIn_strmOut_RTL_out_0_1
		TARGET 				:= hw_emu
	else
		ifeq ($(AXI4S_BROADCAST), 1)
			DESIGN_PART			:= 4
			XSA					:= custom_pfm_strmIn_strmOut_AXIS_Broadcast.xsa
			VITIS_PLATFORM		:= custom_pfm_strmIn_strmOut_AXIS_Broadcast_0_1
			TARGET 				:= hw_emu
		else
			XSA					:= custom_pfm_strmIn_strmOut.xsa
			VITIS_PLATFORM		:= custom_pfm_strmIn_strmOut_0_1
			DESIGN_PART			:= 1
			TARGET 				:= hw_emu
		endif

	endif
#if INCLUDE_ILAS =1 	
else
	XSA					:= custom_pfm_strmIn_strmOut_ILAs.xsa
	VITIS_PLATFORM		:= custom_pfm_strmIn_strmOut_ILAs_0_1
	DESIGN_PART			:= 3
	TARGET 				:= hw
endif

.PHONY: clean all vitis

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make pfm"
	$(ECHO) "      Command to generate the Vitis platform"
	$(ECHO) ""
	$(ECHO) "  make aie_app"
	$(ECHO) "      Command to generate the AI Engine application in Vitis IDE"
	$(ECHO) ""
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the full Vitis workspace"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: pfm aie_app vpp_link

pfm:../Vivado/build/xsa_platform/${XSA}
	vitis -s vitis_pfm.py ${VITIS_PLATFORM} ${XSA} ${DESIGN_PART}

aie_app:
	vitis -s aie_app.py ${VITIS_PLATFORM} ${DESIGN_PART}

vpp_link:
	vitis -s vpp_link.py ${VITIS_PLATFORM} ${DESIGN_PART} ${TARGET}

clean:
	rm -rf ${WORKSPACE}_* .Xil .wsdata
