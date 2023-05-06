# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

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
