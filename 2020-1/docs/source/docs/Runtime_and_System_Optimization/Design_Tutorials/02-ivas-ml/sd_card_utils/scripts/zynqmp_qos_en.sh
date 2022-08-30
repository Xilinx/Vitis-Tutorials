#
# Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software
# is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
# EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
# not be used in advertising or otherwise to promote the sale, use or other
# dealings in this Software without prior written authorization from Xilinx.
#

#!/usr/bin/env bash

# MPSOC chip only

#debug
VERBOSE="yes"

PATH="/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bini:$PATH"

[ ! -x /sbin/devmem ] && {echo "could not find devmem, exit" && exit 0}

qos_msg() {
    test "$VERBOSE" != "no" && printf "[%s]  0x%x:    0x%x => " "$1" $2 $(/sbin/devmem $2 32)
}

qos_dbg() {
    test "$VERBOSE" != "no" && printf "0x%x\n" $(/sbin/devmem $1 32)
}

axi_port_classfication()
{
    ddr_qos_ctrl=0xfd090000
    test "$VERBOSE" != no && printf "\nDDR_PORT_TYPE\n=====================================\n"
    qos_msg "PORT_TYPE" "$ddr_qos_ctrl"
    devmem "$ddr_qos_ctrl" 32 0xa845
    qos_dbg "$ddr_qos_ctrl"
}

afi_hp_ddrc_qos()
{
    local hp0_rdctrl=0xfd380000
    local hp0_rdissue=0xfd380004
    local hp0_rdqos=0xfd380008
    local hp0_wrctrl=0xfd380014
    local hp0_wrissue=0xfd380018
    local hp0_wrqos=0xfd38001c

    for ((i=0;i<4;i++))
    do
        test "$VERBOSE" != no && printf "\nS_AXI_HP%d_FPD (0-low f-high)\n=====================================\n" $i
    
        # Check if FABRIC_QOS_EN-bit[2]=0 ?
        # The QoS bits are derived from APB register, AFIFM_RD/WRQoS.staticQoS
        ((hp_rdctrl=$hp0_rdctrl+$i*0x10000))
        qos_msg "RDCTRL " "$hp_rdctrl"
        qos_dbg "$hp_rdctrl"
        ((hp_wrctrl=$hp0_wrctrl+$i*0x10000))
        qos_msg "WRCTRL " "$hp_wrctrl"
        qos_dbg "$hp_wrctrl"
    
        ((hp_rdissue=$hp0_rdissue+$i*0x10000))
        qos_msg "RDISSUE" "$hp_rdissue"
        devmem "$hp_rdissue" 32 $1
        qos_dbg "$hp_rdissue"
    
        ((hp_rdqos=$hp0_rdqos+$i*0x10000))
        qos_msg "RDQoS  " "$hp_rdqos"
        devmem "$hp_rdqos" 32 $2
        qos_dbg "$hp_rdqos"
    
        ((hp_wrissue=$hp0_wrissue+$i*0x10000))
        qos_msg "WRISSUE" "$hp_wrissue"
        devmem "$hp_wrissue" 32 $3
        qos_dbg "$hp_wrissue"
    
        ((hp_wrqos=$hp0_wrqos+$i*0x10000))
        qos_msg "WRQoS  " "$hp_wrqos"
        devmem "$hp_wrqos" 32 $4
        qos_dbg "$hp_wrqos"
    done
}

afi_hpc0_ddrc_qos()
{
    local hpc0_rdissue=0xfd360004
    local hpc0_rdqos=0xfd360008
    local hpc0_wrissue=0xfd360018
    local hpc0_wrqos=0xfd36001c
    test "$VERBOSE" != no && printf "\nS_AXI_HPC0_FPD (0-low f-high)\n=====================================\n"

    qos_msg "RDISSUE" "$hpc0_rdissue"
    devmem "$hpc0_rdissue" 32 $1
    qos_dbg "$hpc0_rdissue"

    qos_msg "RDQoS  " "$hpc0_rdqos"
    devmem "$hpc0_rdqos" 32 $2
    qos_dbg "$hpc0_rdqos"

    qos_msg "WRISSUE" "$hpc0_wrissue"
    devmem "$hpc0_wrissue" 32 $3
    qos_dbg "$hpc0_wrissue"

    qos_msg "WRQoS  " "$hpc0_wrqos"
    devmem "$hpc0_wrqos" 32 $4
    qos_dbg "$hpc0_wrqos"
}

afi_hpc1_ddrc_qos()
{
    local hpc1_rdissue=0xfd370004
    local hpc1_rdqos=0xfd370008
    local hpc1_wrissue=0xfd370018
    local hpc1_wrqos=0xfd37001c
    test "$VERBOSE" != no && printf "\nS_AXI_HPC1_FPD (0-low f-high)\n=====================================\n"

    qos_msg "RDISSUE" "$hpc1_rdissue"
    devmem "$hpc1_rdissue" 32 $1
    qos_dbg "$hpc1_rdissue"

    qos_msg "RDQoS  " "$hpc1_rdqos"
    devmem "$hpc1_rdqos" 32 $2
    qos_dbg "$hpc1_rdqos"

    qos_msg "WRISSUE" "$hpc1_wrissue"
    devmem "$hpc1_wrissue" 32 $3
    qos_dbg "$hpc1_wrissue"

    qos_msg "WRQoS  " "$hpc1_wrqos"
    devmem "$hpc1_wrqos" 32 $4
    qos_dbg "$hpc1_wrqos"
}

qos_config()
{
axi_port_classfication

# increase the read issuing capability of the port connected to DPU.
# By default, it can take a maximum of eight requests at a time, and increasing
# the issueing capability may keep the ports busy with always some requests in the queue.
# Set RDISSUE-AFIFM register to allow 16 commands at a time, leave others as it is
#
# incoming ARQOS values 0-3 to LPR (best effort) traffic, and 4+ to Video Class.
# traffic class sets to "BE"

afi_hpc0_ddrc_qos 15 3 15 3
afi_hpc1_ddrc_qos 15 3 15 3
afi_hp_ddrc_qos   15 3 15 3

return 0
}
