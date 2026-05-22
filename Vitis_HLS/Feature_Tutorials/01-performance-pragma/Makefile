#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to run all target csim & csynth in turn."
	@echo ""
	@echo "  make csim"
	@echo "  make csynth"
	@echo "      Command to run only one step of Vitis HLS. Note csim forces clean first."
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove the generated files."
	@echo ""
	@echo "  make help"
	@echo "      Print this help."
	@echo ""
#######################################################################################

SHELL 			:= bash

CONFIG 			:= ./golden/hls_config.cfg
WORK_DIR 		:= Filter2DKernel
COMMON_OPTIONS 	:= --mode hls --config ${CONFIG} --work_dir ${WORK_DIR}

all: csim csynth ;#cosim

csynth:
	v++ --compile ${COMMON_OPTIONS}

csim: clean # clean before csim !
csim cosim:
	vitis-run --$@ ${COMMON_OPTIONS}

.PHONY: clean
clean:
	rm -rf ${WORK_DIR} vitis_c++/ .Xil/ xcd.log
