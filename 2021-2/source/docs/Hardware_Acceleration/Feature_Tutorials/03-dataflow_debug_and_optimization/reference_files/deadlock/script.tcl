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

# ##########################################################
# expect_error
#
# Takes a command to run and an expected error 
# message string (wilcards are supported) and checks 
# that the error was actually issued in the log file.
#
# ##########################################################
proc expect_error {command errorMsgStr {projClose 1}} {
    set ret [catch {uplevel 1 $command} err] 

    set log_name vitis_hls.log
    set report [file join [get_project -directory] .. $log_name]

    if { ![file exists $report] } {
        error "$report does not exist"
    }

    set handle [open $report r]
    set report_buf [read -nonewline $handle]
    close $handle

    if {[regexp $errorMsgStr $report_buf]} {
        puts "PASSED: Expected Error Message \"\[$errorMsgStr\]\" received "
        if {$projClose} { 
            close_project
            exit 0
        }
    } else {
        puts "FAILED: Expected Error Message \"\[$errorMsgStr\]\" not received "
        if {$projClose} { 
            close_project
            exit 1
        }              
    }
}

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

# Cosim will fail with a deadlock
expect_error cosim_design "HLS 200-742"

