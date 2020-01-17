
NUM_DOCS:= 100000

# Host Application files repository


HOST_SRC_CPP += compute_score_host.cpp
HOST_SRC_CPP += MurmurHash2.c
HOST_SRC_CPP += main.cpp
HOST_SRC_H := common.h
HOST_SRC_H += sizes.h


#Host Compiler Global Settings and Include Libraries

#CXXFLAGS :=
CXXFLAGS += -D__USE_XOPEN2K8
CXXFLAGS += -D__USE_XOPEN2K8
CXXFLAGS += -I$(XILINX_XRT)/include/
CXXFLAGS += -I$(XILINX_VIVADO)/include/
CXXFLAGS += -I$(SRC_REPO)
CXXFLAGS += -O0 -g -pg -Wall -fmessage-length=0 -std=c++14


CXXLDFLAGS := -L$(XILINX_XRT)/lib/
CXXLDFLAGS += -lxilinxopencl -lpthread -lrt

run: build/host 
	./build/host cpu $(NUM_DOCS)

## Host Executable File Generation
build/host: $(HOST_SRC_CPP)  $(HOST_SRC_H)
	mkdir -p build
	xcpp $(CXXFLAGS) $(HOST_SRC_CPP) $(CXXLDFLAGS) -o $@

clean:
	rm -f build/host
