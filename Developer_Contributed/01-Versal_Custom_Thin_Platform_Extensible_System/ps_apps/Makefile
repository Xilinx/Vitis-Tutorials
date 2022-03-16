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

APP_DIR           := exe
APP_DIRS_EXCLUDE  := $(patsubst %/,%,$(dir $(wildcard exe*/)))
APP_DIRS          := $(patsubst %/,%,$(dir $(wildcard */)))
APP_NAME          := $(filter-out ${APP_DIRS_EXCLUDE},$(APP_DIRS))
APP_NAME_CLEAN    := $(addsuffix .clean,$(APP_NAME))

.PHONY: clean help all $(APP_NAME) $(APP_NAME_CLEAN)

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the ps_apps used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${APP_DIR} $(APP_NAME)

$(APP_DIR):
	mkdir ${APP_DIR}

$(APP_NAME):
	make all -C $@

clean: $(APP_NAME_CLEAN)
	rm -rf $(APP_DIR)

$(APP_NAME_CLEAN):
	make clean -C $(basename $@)

