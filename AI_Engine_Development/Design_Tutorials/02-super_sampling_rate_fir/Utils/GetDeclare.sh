#!/bin/bash
#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#



row=0
col=0
for row in {0..7}
do
  echo " "
  echo "Row ${row}"
for col in {23..30}
do
  #echo "Tile $col $row"
  File="${col}_${row}/src/${col}_${row}.cc"
  cat $File | grep DoubleStream::
done
done
