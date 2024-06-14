#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#

include ../common.mk

.PHONY: clean

ConfigName := system_$(NAntenna).cfg

$(info "-------------------------------------------------------------------------")
$(info "Hw Configuration File Build: $(ConfigName)")
$(info "-------------------------------------------------------------------------")
$(info "Number of Antenna   : $(NAntenna)")
$(info "Width of PLIO I/F   : $(PLIOW)")
$(info "Data Width (Nb Bits): $(DATAW)")
$(info "-------------------------------------------------------------------------")




all:  SystemFileCreation 


SystemFileCreation:
	head --lines=7  BaseSystemCFG.cfg > $(ConfigName)
	echo "nk=gen2s_$(DATAW)_$(PLIOW):$(NAntenna)" >> $(ConfigName)
	echo "nk=s2mm_$(DATAW)_$(PLIOW):$(NAntenna)" >> $(ConfigName)
	number=1 ; id0=0 ; while [[ $$number -le $(NAntenna) ]] ; do \
        echo "stream_connect=gen2s_$(DATAW)_$(PLIOW)_$$number.s:ai_engine_0.Input_$$id0"  >> $(ConfigName) ; \
		echo "stream_connect=ai_engine_0.Output_$$id0:s2mm_$(DATAW)_$(PLIOW)_$$number.s"  >> $(ConfigName) ; \
        echo " "  >> $(ConfigName) ; \
        ((number = number + 1)) ; \
        ((id0 = id0 + 1)) ; \
    done
	tail --line=6 BaseSystemCFG.cfg >> $(ConfigName)

clean:
	rm -rf system*.cfg
