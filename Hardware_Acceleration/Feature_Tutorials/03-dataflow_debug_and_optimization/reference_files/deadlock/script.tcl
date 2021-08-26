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
open_project -reset proj

# Add design files
add_files example.cpp
# Add test bench & files
add_files -tb example_test.cpp

# Set the top-level function
set_top example
open_solution -reset solution1

# Define technology and clock rate
set_part virtex7
create_clock -period "75MHz"

csim_design 
csynth_design
cosim_design 
