#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# Common Setup of Platform, e.g. $(XSA) definition
include ../platform.mk

TCL_SOURCE=xsa_scripts/xsa.tcl

.PHONY: all xsa

all xsa : $(XSA)

$(XSA): $(TCL_SOURCE)
	$(VIVADO) -mode batch -notrace -source $(TCL_SOURCE) -tclargs $(PLATFORM_NAME) $(VER) $(PRE_SYNTH)

clean:
	$(RM) -r vivado* build .Xil *dynamic* *.log *.xpe 
