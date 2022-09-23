#! /bin/bash

# Copyright 2021 Xilinx Inc.
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
