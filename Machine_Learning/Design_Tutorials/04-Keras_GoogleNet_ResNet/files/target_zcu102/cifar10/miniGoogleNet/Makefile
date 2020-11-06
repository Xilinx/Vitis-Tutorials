## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.


CNN=miniGoogleNet

PROJECT = $(CNN)


override CROSS_COMPILE := /opt/petalinux/2020.1/sysroots/x86_64-petalinux-linux/usr/bin/aarch64-xilinx-linux/aarch64-xilinx-linux-

# If need to compile the model on the board directly, just comment out SYSROOT
override SYSROOT := /opt/petalinux/2020.1/sysroots/aarch64-xilinx-linux

ifeq ($(SYSROOT), )
CXX       :=   g++
CC        :=   gcc

LDFLAGS   =   $(shell pkg-config --libs opencv)
LDFLAGS   +=  -lhineon -ln2cube -lpthread -lopencv_videoio
CFLAGS    :=  -O2 -Wall -Wpointer-arith -std=c++11 -ffast-math

else
CXX       :=   $(CROSS_COMPILE)g++
CC        :=   $(CROSS_COMPILE)gcc

CFLAGS    :=   --sysroot=$(SYSROOT) -O2 -Wall -Wpointer-arith -std=c++11 -ffast-math
CFLAGS    +=   -I$(SYSROOT)/usr/include -I$(SYSROOT)/usr/local/include -I./include

LDFLAGS   :=  --sysroot=$(SYSROOT) -L$(SYSROOT)/usr/local/lib/ -L${SYSROOT}/lib -L${SYSROOT}/usr/lib
LDFLAGS   +=  -ln2cube -lopencv_videoio  -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_core -lpthread

endif

CUR_DIR =   $(shell pwd)

MODDIR  =   $(CUR_DIR)/model
BUILD   =   $(CUR_DIR)/build
VPATH   =   $(SRC)
C_DIR  :=   $(shell find $(SRC) -name *.c)
OBJ     =   $(patsubst %.c, %.o, $(notdir $(C_DIR)))
CC_DIR :=   $(shell find $(SRC) -name *.cc)
OBJ    +=   $(patsubst %.cc, %.o, $(notdir $(CC_DIR)))
CPP_DIR :=   $(shell find $(SRC) -name *.cpp)
OBJ    +=   $(patsubst %.cpp, %.o, $(notdir $(CPP_DIR)))
ARCH	=   $(shell uname -m | sed -e s/arm.*/armv71/ \
				  -e s/aarch64.*/aarch64/ )
CFLAGS +=  -mcpu=cortex-a53

MODEL = $(CUR_DIR)/model/dpu_$(CNN)_0.elf
#SRC   = $(CUR_DIR)/src
SRC   = $(CUR_DIR)

.PHONY: all clean

all: $(BUILD) $(PROJECT)

$(PROJECT) : $(OBJ)
	$(CXX) $(CFLAGS) $(addprefix $(BUILD)/, $^) $(MODEL) -o $@ $(LDFLAGS)

%.o : %.cc
	$(CXX) -c $(CFLAGS) $< -o $(BUILD)/$@
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $(BUILD)/$@

clean:
	$(RM) -rf $(BUILD)
	$(RM) $(PROJECT)

$(BUILD) :
	-mkdir -p $@
