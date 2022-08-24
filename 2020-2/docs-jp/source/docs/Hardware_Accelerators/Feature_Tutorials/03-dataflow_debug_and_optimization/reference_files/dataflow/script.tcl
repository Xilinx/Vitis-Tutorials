##
 # Copyright 2021 Xilinx, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #   http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
##

# Create a project
open_project proj -reset

# Add design files
add_files diamond.cpp
# Add test bench & files
add_files -tb diamond_test.cpp
add_files -tb result.golden.dat
# Set the top-level function
set_top diamond

# ########################################################
# Create a solution
open_solution -reset solution1
# Define technology and clock rate
set_part {xcvu9p-flga2104-2-i}
create_clock -period 5 -name default

csim_design
csynth_design
cosim_design

exit
