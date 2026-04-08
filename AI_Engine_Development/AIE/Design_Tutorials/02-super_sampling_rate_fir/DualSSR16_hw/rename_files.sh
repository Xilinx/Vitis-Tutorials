#!/bin/bash
# Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

for f in data/*.txt; do
   for col in "$@"; do
      cp "$f" "${f%.txt}_$col.txt"
   done
done

