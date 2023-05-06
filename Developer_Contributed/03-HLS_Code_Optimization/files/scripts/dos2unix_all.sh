#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Dos2Unix conversion just to avoid any trouble with file formats
for file in $(find . -name "*.sh" );   do dos2unix ${file}; done
for file in $(find . -name "*.tcl");   do dos2unix ${file}; done
for file in $(find . -name "*.h"  );   do dos2unix ${file}; done
for file in $(find . -name "config" ); do dos2unix ${file}; done
for file in $(find . -name "*.c*" );   do dos2unix ${file}; done
for file in $(find . -name "*.m" );    do dos2unix ${file}; done
for file in $(find . -name "*.dat" );  do dos2unix ${file}; done
for file in $(find . -name "*.txt" );  do dos2unix ${file}; done
for file in $(find . -name "*akefile" ); do dos2unix ${file}; done
