#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

set hostname [lindex $argv 0]
set images_path [lindex $argv 1]
connect -host $hostname -port 3121
targets -set -nocase -filter {name =~ "*Versal*"}
device program "$images_path/BOOT.BIN"
stop
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/Image" 0x00080000
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/rootfs.cpio.gz.u-boot" 0x4000000
after 2000
targets -set -nocase -filter {name =~ "*Versal*"}
dow -data -force "$images_path/boot.scr" 0x20000000
after 2000
con
