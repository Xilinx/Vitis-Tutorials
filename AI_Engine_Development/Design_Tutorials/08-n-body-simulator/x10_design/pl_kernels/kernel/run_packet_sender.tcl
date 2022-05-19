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

#create new project
open_project -reset packet_sender

#specify the top-level function
set_top packet_sender

# add design files
add_files packet_sender.cpp

# add testbench files
add_files -tb packet_sender_test.cpp
add_files -tb input_i0.dat

# create the solution named solution1 
open_solution solution1 -reset -flow_target vitis

# associate the device to solution1
set_part {xcvc1902-vsva2197-1LHP-i-L}

# associate clock with 2.5ns period for solution1
create_clock -period 2.5ns

# run C simulation
csim_design

# sythesize the design 
csynth_design

# perform C/RTL Cosimulation
cosim_design

#export RTL as IP
#export_design -flow impl -format xo

exit
