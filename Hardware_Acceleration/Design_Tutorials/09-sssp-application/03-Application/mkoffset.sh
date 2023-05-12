# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

#! /bin/bash

CURRENT_DIR=$(cd `dirname $0`; pwd)
CSR_DIR=$CURRENT_DIR/csr_data/
COO_DIR=$CURRENT_DIR/coo_data/
HOST_DIR=$CURRENT_DIR/coo2csr/

cd $HOST_DIR


for line in `cat mkoffset.cpp |sed -n 's|.*coo_data/||p'|sed -n 's/";.*$//p'`
do
sed -i "s/$line/$1/" mkoffset.cpp
done

for line in `cat mkoffset.cpp |sed -n 's|.*csr_data/||p'|sed -n 's/";.*$//p'`
do
sed -i "s/$line/$2/" mkoffset.cpp
done


File="$HOST_DIR/mkoffset"
if [ ! -f "$File" ]
then
        echo "Compiling..."
else
        rm $File
        echo "Compiling..."
fi

g++ mkoffset.cpp -o mkoffset
./mkoffset

echo "Compiled"

cd $CSR_DIR
if [ -f "$2" ]
then
        echo "Offset is done!"
else
        echo "Error. Please chek."
        exit 1
fi
