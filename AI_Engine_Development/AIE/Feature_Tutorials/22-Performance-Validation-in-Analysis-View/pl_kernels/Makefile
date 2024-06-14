#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#

include ../common.mk

.PHONY: clean

$(info "-------------------------------------------------------------------------")
$(info "PL Kernels Build")
$(info "-------------------------------------------------------------------------")
$(info "Width of PLIO I/F   : $(PLIOW)")
$(info "Data Width (Nb Bits): $(DATAW)")
$(info "PL Kernel Frequency : $(FREQ_HZ)")
$(info "-------------------------------------------------------------------------")


# Size computation
NDATA_CLOCK := $(shell echo $(PLIOW)/$(DATAW)  | bc )

GeneratorFunctionName := gen2s_$(DATAW)_$(PLIOW)
GeneratorFileName := $(GeneratorFunctionName).cpp

s2mmFunctionName := s2mm_$(DATAW)_$(PLIOW)
s2mmFileName := $(s2mmFunctionName).cpp

mm2sFunctionName := mm2s_$(DATAW)_$(PLIOW)
mm2sFileName := $(mm2sFunctionName).cpp

CPP_FILES := $(GeneratorFileName) $(s2mmFileName)
XO_FILES := $(subst .cpp,.xo,$(CPP_FILES))

# SYSTEM_VAR :=  PLATFORM=$(PLATFORM) PLIOW=$(PLIOW) DATAW=$(DATAW) FREQ_HZ=$(FREQ_HZ)

# GeneratorFileCreation s2mmFileCreation
all: disp GeneratorFileCreation s2mmFileCreation
	make  $(XO_FILES)

disp:
	echo $(CPP_FILES)
	echo $(XO_FILES)


GeneratorFileCreation:
	cp gen2sCPP.cpp $(GeneratorFileName)
	sed -i "s/#define BUS_SIZE/#define BUS_SIZE $(PLIOW)/g" $(GeneratorFileName)
	sed -i "s/#define DATA_SIZE/#define DATA_SIZE $(DATAW)/g" $(GeneratorFileName)
	sed -i "s/#define ARRAY_SIZE/#define ARRAY_SIZE $(ARRAY_SIZE)/g" $(GeneratorFileName)
	sed -i "s/FUNCTION_NAME/$(GeneratorFunctionName)/g" $(GeneratorFileName)

s2mmFileCreation:
	cp s2mmCPP.cpp $(s2mmFileName)
	sed -i "s/#define BUS_SIZE/#define BUS_SIZE $(PLIOW)/g" $(s2mmFileName)
	sed -i "s/#define DATA_SIZE/#define DATA_SIZE $(DATAW)/g" $(s2mmFileName)
	sed -i "s/#define ARRAY_SIZE/#define ARRAY_SIZE $(ARRAY_SIZE)/g" $(s2mmFileName)
	sed -i "s/FUNCTION_NAME/$(s2mmFunctionName)/g" $(s2mmFileName)

mm2sFileCreation:
	cp mm2sCPP.cpp $(mm2sFileName)
	sed -i "s/#define BUS_SIZE/#define BUS_SIZE $(PLIOW)/g" $(mm2sFileName)
	sed -i "s/#define DATA_SIZE/#define DATA_SIZE $(DATAW)/g" $(mm2sFileName)
	sed -i "s/#define ARRAY_SIZE/#define ARRAY_SIZE $(ARRAY_SIZE)/g" $(mm2sFileName)
	sed -i "s/#define FRAME_SIZE/#define ARRAY_SIZE $(ARRAY_SIZE)/g" $(mm2sFileName)
	sed -i "s/FUNCTION_NAME/$(mm2sFunctionName)/g" $(mm2sFileName)

%.xo : %.cpp
	mkdir -p plkernels
	sed "s/syn.top=/syn.top=$(basename $@)/g" BaseConfig.cfg > plkernels/HLSConfig_$(basename $@).cfg
	sed -i "s/syn.file=/syn.file=..\/$(basename $@).cpp/g" plkernels/HLSConfig_$(basename $@).cfg
	v++ --compile --mode hls --platform $(PLATFORM) --config plkernels/HLSConfig_$(basename  $@).cfg --work_dir plkernels/$(basename $@)
	cp plkernels/$(basename $@)/$(basename $@).xo .

clean:
	rm -rf _x *.xo* *.log .Xil  .crashReporter  plkernels gen2s_* s2mm_* mm2s_*
