CXX:=g++
############## Check the version of gcc avaiable and select
ifneq ($(shell expr $(shell g++ -dumpversion) \>= 5), 1)
   ifndef XILINX_VIVADO
        $(error [ERROR]: g++ version older. Please use 5.0 or above.)
   else
      CXX := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/bin/g++
      $(warning [WARNING]: g++ version is older. Using g++ provided by the Vitis tool : $(CXX))
   endif
endif


APP          = Filter2D.exe

# -----------------------------------------------------------------------------

SRC = ../src

APP_SOURCE_FILES  += ${SRC}/host_sw_only.cpp 
APP_SOURCE_FILES  += ${SRC}/filter2d_sw.cpp 
APP_SOURCE_FILES  += ${SRC}/cmdlineparser.cpp 
APP_HEADER_FILES  += ${SRC}/*.h 

APP_COMP_OPTIONS  += -std=c++14
APP_LINK_OPTIONS  += -lpthread -lrt -lstdc++ -fopenmp

# -----------------------------------------------------------------------------

# Compile and build the host application (.exe file)
${APP}: ${APP_SOURCE_FILES} ${APP_HEADER_FILES}
	$(CXX) -O3 -o $@ ${APP_SOURCE_FILES} ${APP_COMP_OPTIONS} ${APP_LINK_OPTIONS}

app: ${APP}

clean:
	rm -rf ${APP} emconfig.json *csv vitis* xclbin.run_summary xrt.ini *log

