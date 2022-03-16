 #
 # Copyright 2021 Xilinx, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

ECHO              := @echo

TARGET            ?= hw

XO_DIR            := xo_${TARGET}
XO_DIRS_EXCLUDE   := $(patsubst %/,%,$(dir $(wildcard xo_*/)))
KERNEL_DIRS       := $(patsubst %/,%,$(dir $(wildcard */)))
KERNEL_NAME       := $(filter-out ${XO_DIRS_EXCLUDE},$(KERNEL_DIRS))
KERNEL_NAME_CLEAN := $(addsuffix .clean,$(KERNEL_NAME))

.PHONY: help clean all $(KERNEL_NAME)

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the All IPs used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO_DIR} $(KERNEL_NAME)

$(XO_DIR):
	mkdir    ${XO_DIR}

$(KERNEL_NAME):
	make all -C $@

clean: $(KERNEL_NAME_CLEAN)
	rm -rf $(XO_DIR)

$(KERNEL_NAME_CLEAN):
	make clean -C $(basename $@)

