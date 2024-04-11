#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to run all target csim & csynth & cosim in turn."
	@echo ""
	@echo "  make csim"
	@echo "  make csynth"
	@echo "  make cosim"
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
SOURCES         := hw.cpp hw.h
SOURCES_TB      := ${SOURCES} tb.cpp

CONFIG 			:= hls_config.cfg
WORK_DIR 		:= top
COMMON_OPTIONS 	:= --mode hls --config ${CONFIG} --work_dir ${WORK_DIR}

all: csim csynth cosim

csynth: ${SOURCES} ${CONFIG}
	v++ --compile ${COMMON_OPTIONS}

csim: clean # clean before csim !
csim cosim: ${SOURCES_TB} ${CONFIG}
	vitis-run --$@ ${COMMON_OPTIONS}

.PHONY: clean
clean:
	rm -rf ${WORK_DIR} vitis_c++/ .Xil/ xcd.log
