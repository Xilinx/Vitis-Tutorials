#
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Luke Millar

SHELL := /bin/bash
VERSION ?= 2025.2

all: clean create_git_repo create_working_folder push_working_folder clone_and_rebuild

create_design: clean create_hw_design create_vitis_ws

create_git_repo:
					mkdir repo && \
					cd repo && \
					git init --bare

create_working_folder:
					mkdir work
					cd work && \
					git init && \
					git remote add origin ../repo && \
					vivado -mode tcl -source ../hw_design.tcl && \
					vitis -s ../vitis_ws.py

create_hw_design:
					vivado -mode tcl -source hw_design.tcl

create_vitis_ws:
					vitis -s vitis_ws.py

push_working_folder:
					cd work && \
					git config user.name "Demo User" && \
					git config user.email "demo@example.com" && \
					git add . && \
					git commit -m "Initial Commit" && \
					git push origin HEAD

clone_and_rebuild:
					mkdir test
					cd test && \
					git clone ../repo

clean:
					$(RM) -rf ./zcu102 ./vitis_ws ./repo ./test ./work 

