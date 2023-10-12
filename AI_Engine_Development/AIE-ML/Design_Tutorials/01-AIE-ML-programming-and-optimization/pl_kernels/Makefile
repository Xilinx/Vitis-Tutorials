#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#



.PHONY: clean

sizeM ?= 64
sizeK ?= 64
sizeN ?= 64
PLIOW ?= 128
NIterations ?= 16

# Testing variable definition

ifndef sizeM
$(error "sizeM should be defined")
endif

ifndef sizeK
$(error  "sizeK should be defined")
endif

ifndef sizeN
$(error  "sizeN should be defined")
endif

ifndef PLIOW
$(error  "PLIOW should be defined")
endif

ifndef NIterations
$(error  "NIterations should be defined")
endif


# Variable definition
SETTINGS8 := system_settings_8.h
SETTINGS16 := system_settings_16.h
SETTINGS32 := system_settings_32.h

# Size computation
InA := $(shell echo $(sizeM)*$(sizeK)*$(NIterations)  | bc )
InB := $(shell echo $(sizeK)*$(sizeN)*$(NIterations)  | bc )
OutC := $(shell echo $(sizeM)*$(sizeN)*$(NIterations) | bc )

ifeq ($(shell expr $(InA) \>= $(InB)), 1)
	InSize := $(InA)
else
	InSize := $(InB)
endif

OutSize := $(OutC)

SystemSettings:
	echo " "
	echo "#pragma once" > $(SETTINGS8)
	echo "" >> $(SETTINGS8)
	echo "#define InputArraySize " $(InSize) >> $(SETTINGS8)
	echo "#define OutputArraySize " $(OutSize) >> $(SETTINGS8)
	echo "#define NBitsData 8" >> $(SETTINGS8)
	echo " "
	echo "#pragma once" > $(SETTINGS16)
	echo "" >> $(SETTINGS16)
	echo "#define InputArraySize " $(InSize) >> $(SETTINGS16)
	echo "#define OutputArraySize " $(OutSize) >> $(SETTINGS16)
	echo "#define NBitsData 16" >> $(SETTINGS16)
	echo " "
	echo "#pragma once" > $(SETTINGS32)
	echo "" >> $(SETTINGS32)
	echo "#define InputArraySize " $(InSize) >> $(SETTINGS32)
	echo "#define OutputArraySize " $(OutSize) >> $(SETTINGS32)
	echo "#define NBitsData 32" >> $(SETTINGS32)

CPP_FILES := mm2s_8_$(PLIOW).cpp s2mm_16_$(PLIOW).cpp s2mm_32_$(PLIOW).cpp
XO_FILES := $(subst .cpp,.xo,$(CPP_FILES))

#$(wildcard *_$(PLIOW).cpp)
all: SystemSettings $(XO_FILES)

%.xo: %.cpp
	v++ -c --platform ${PLATFORM} --clock.defaultFreqHz ${FREQ_HZ} --hls.clock ${FREQ_HZ}:$(basename $<) -k $(basename $<) $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log .Xil system_settings.h .crashReporter system_settings*
