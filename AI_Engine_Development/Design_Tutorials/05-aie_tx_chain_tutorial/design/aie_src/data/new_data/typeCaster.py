#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    (c) Copyright 2020 Xilinx, Inc. All rights reserved.
   
    This file contains confidential and proprietary information
    of Xilinx, Inc. and is protected under U.S. and
    international copyright and other intellectual property
    laws.
   
    DISCLAIMER
    This disclaimer is not a license and does not grant any
    rights to the materials distributed herewith. Except as
    otherwise provided in a valid license issued to you by
    Xilinx, and to the maximum extent permitted by applicable
    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
    (2) Xilinx shall not be liable (whether in contract or tort,
    including negligence, or under any other theory of
    liability) for any loss or damage of any kind or nature
    related to, arising under or in connection with these
    materials, including for any direct, or any indirect,
    special, incidental, or consequential loss or damage
    (including loss of data, profits, goodwill, or any type of
    loss or damage suffered as a result of any action brought
    by a third party) even if such damage or loss was
    reasonably foreseeable or Xilinx had been advised of the
    possibility of the same.
   
    CRITICAL APPLICATIONS
    Xilinx products are not designed or intended to be fail-
    safe, or for use in any application requiring fail-safe
    performance, such as life-support or safety devices or
    systems, Class III medical devices, nuclear facilities,
    applications related to the deployment of airbags, or any
    other applications that could lead to death, personal
    injury, or severe property or environmental damage
    (individually and collectively, "Critical
    Applications"). Customer assumes the sole risk and
    liability of any use of Xilinx products in Critical
    Applications, subject only to applicable laws and
    regulations governing limitations on product liability.
   
    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
    PART OF THIS FILE AT ALL TIMES.                       
	@rajeevp
	@vipulas


@python3

         
"""

import os
import sys
import re
import argparse
import random
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime

class TypeCaster(object):
    def __init__(self):
        """ initialize """
        self.description = "Functions to convert bit string to cint16"
        self.int16Max = np.iinfo(np.uint16(1)).max
             
    def __str__(self):
        """description """
        return self.description

    def hex2int16(self, inp):
        return np.int16(np.int(inp, 16))
    
    def hex2Cint16(self, data):
        """ convert the 32b hex to cint16 """
        if len(data)<5:
            imagT, realT = "0", data[0:4]    
        else:
            imagT, realT = data[0:len(data)-4], data[len(data)-4:]   
        imagT, realT = self.hex2int16(imagT), self.hex2int16(realT)        
        return(realT, imagT)
        
    def int16toHex(self, inp):
        if inp<0:
            """ abs(in16) does not work at 16b boundaries due to sign extensions on x86   
            hexVal = self.int16Max-abs(inp)+1    
            """
            hexVal = self.int16Max-abs(inp).astype(np.uint16).max()+1
        else:
            hexVal = inp
        hexValStr = "%x"%hexVal
        if len(hexValStr)<4:
            outStr = "0"*(4-len(hexValStr))+hexValStr
        else:
            outStr = hexValStr
        return outStr  

#----------------------------------------------------------------------------------------------------------------------
def main():
    os.system("clear")
    print("\n.. Executing.. : %s "%sys.argv[0])

    obj = typeCaster()
    r, i = obj.hex2Cint16("ffff0001")
    print("In:ffff0001  Out:Real:%d  Imag:%d"%(r, i))
    print("In:%d %d     Out:%s %s"%(i, r, obj.int16toHex(i),obj.int16toHex(r)))
    
    print("Exitting %s ... "%sys.argv[0])
    print(""+"-"*80)  
    
    
if __name__=="__main__":
    main()
