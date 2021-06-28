#    (c) Copyright 2020 Xilinx, Inc. All rights reserved.
#   
#    This file contains confidential and proprietary information
#    of Xilinx, Inc. and is protected under U.S. and
#    international copyright and other intellectual property
#    laws.
#   
#    DISCLAIMER
#    This disclaimer is not a license and does not grant any
#    rights to the materials distributed herewith. Except as
#    otherwise provided in a valid license issued to you by
#    Xilinx, and to the maximum extent permitted by applicable
#    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#    (2) Xilinx shall not be liable (whether in contract or tort,
#    including negligence, or under any other theory of
#    liability) for any loss or damage of any kind or nature
#    related to, arising under or in connection with these
#    materials, including for any direct, or any indirect,
#    special, incidental, or consequential loss or damage
#    (including loss of data, profits, goodwill, or any type of
#    loss or damage suffered as a result of any action brought
#    by a third party) even if such damage or loss was
#    reasonably foreseeable or Xilinx had been advised of the
#    possibility of the same.
#   
#    CRITICAL APPLICATIONS
#    Xilinx products are not designed or intended to be fail-
#    safe, or for use in any application requiring fail-safe
#    performance, such as life-support or safety devices or
#    systems, Class III medical devices, nuclear facilities,
#    applications related to the deployment of airbags, or any
#    other applications that could lead to death, personal
#    injury, or severe property or environmental damage
#    (individually and collectively, "Critical
#    Applications"). Customer assumes the sole risk and
#    liability of any use of Xilinx products in Critical
#    Applications, subject only to applicable laws and
#    regulations governing limitations on product liability.
#   
#    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#    PART OF THIS FILE AT ALL TIMES.                       

setws ../xpfm/plat_ws

getprojects

#importprojects ${proj_ws}/vck190_aie_tx_chain

platform active vck190_aie_tx_chain

set address_map [getaddrmap vck190_aie_tx_chain psv_cortexa72_0]

set plio_addrmap ""

lappend plio_addrmap [regexp -line -inline {mm2s_0_.+} $address_map]
lappend plio_addrmap [regexp -line -inline {mm2s_1_.+} $address_map]
lappend plio_addrmap [regexp -line -inline {s2mm_0_.+} $address_map]

# Open file
set fid [open ../aie_src/address_map.txt w+]

for {set i 0} {$i < [llength $plio_addrmap]} {incr i} {
    puts $fid "[concat {*}[lindex $plio_addrmap $i]]"
}

close $fid
