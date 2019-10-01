# Definition of include file locations
OPENCL12_INCLUDE:= $(XILINX_SDX)/runtime/include/1_2

# Library directories
SDA_LIB:=$(XILINX_SDX)/lib/lnx64.o

opencl_CXXFLAGS=-I$(OPENCL12_INCLUDE)

OPENCL_LIB:=$(XILINX_SDX)/runtime/lib/x86_64
opencl_LDFLAGS=-L$(OPENCL_LIB) -L$(SDA_LIB) -lOpenCL -pthread
