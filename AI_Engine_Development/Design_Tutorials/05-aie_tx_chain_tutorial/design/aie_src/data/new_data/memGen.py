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
from typeCaster import *

class MemGen(TypeCaster):
    def __init__(self):
        """ initialize """
        TypeCaster.__init__(self)
        self.description = "Generate mem file from cint16 text"
        self.inputDir = None
        self.outputDir = None
        self.fileDict = None
             
    def __str__(self):
        """description """
        return self.description

    def setFileDict(self):
        fileDict = {
        #"IFFT": {"path": "../../../aie/64_TRx/DLBF_IFFT/data/", "fileList":["ifft_din000.txt", "ifft_din001.txt", "ifft_din002.txt", "ifft_din003.txt", "ifft_din010.txt", "ifft_din011.txt", "ifft_din012.txt", "ifft_din013.txt", "ifft_din020.txt", "ifft_din021.txt", "ifft_din022.txt", "ifft_din023.txt", "ifft_din030.txt", "ifft_din031.txt", "ifft_din032.txt", "ifft_din033.txt", "ifft_din040.txt", "ifft_din041.txt", "ifft_din042.txt", "ifft_din043.txt", "ifft_din050.txt", "ifft_din051.txt", "ifft_din052.txt", "ifft_din053.txt", "ifft_din060.txt", "ifft_din061.txt", "ifft_din062.txt", "ifft_din063.txt", "ifft_din070.txt", "ifft_din071.txt", "ifft_din072.txt", "ifft_din073.txt"]}
	#"IFFT": {"path": "../../../aie/64_TRx/DLBF_IFFT/data/", "fileList":["ifft_gold000.txt", "ifft_gold001.txt", "ifft_gold010.txt", "ifft_gold011.txt", "ifft_gold020.txt", "ifft_gold021.txt", "ifft_gold030.txt", "ifft_gold031.txt", "ifft_gold040.txt", "ifft_gold041.txt", "ifft_gold050.txt", "ifft_gold051.txt", "ifft_gold060.txt", "ifft_gold061.txt", "ifft_gold070.txt", "ifft_gold071.txt" ]}
	#"DLBF": {"path": "../../../aie/64_TRx/DLBF_IFFT/data/", "fileList":["dlbf_cin00.txt", "dlbf_cin01.txt", "dlbf_cin10.txt", "dlbf_cin11.txt", "dlbf_cin20.txt", "dlbf_cin21.txt", "dlbf_cin30.txt", "dlbf_cin31.txt", "dlbf_cin40.txt", "dlbf_cin41.txt", "dlbf_cin50.txt", "dlbf_cin51.txt", "dlbf_cin60.txt", "dlbf_cin61.txt", "dlbf_cin70.txt", "dlbf_cin71.txt", "dlbf_din0.txt", "dlbf_din1.txt", "dlbf_rin0.txt", "dlbf_rin1.txt", "dlbf_rin2.txt", "dlbf_rin3.txt", "dlbf_rin4.txt", "dlbf_rin5.txt", "dlbf_rin6.txt", "dlbf_rin7.txt", "dlbf_gold0.txt", "dlbf_gold1.txt", "dlbf_gold2.txt", "dlbf_gold3.txt", "dlbf_gold4.txt", "dlbf_gold5.txt", "dlbf_gold6.txt", "dlbf_gold7.txt"]},
	"tx_chain": {"path": "./", "fileList":["golden_temp.txt"]},		
	}
        self.fileDict = fileDict 


    def genMemFile(self):
        """ function """
        for dirName in self.fileDict.keys():
            self.inputDir = self.fileDict[dirName]["path"]
            self.outputDir = self.fileDict[dirName]["path"]
            for fileName in self.fileDict[dirName]["fileList"]:
                try:
                    fr = open(self.inputDir+fileName, "r")  
                except PermissionError:
                    print("Please close the file and run again..")
                    sys.exit(1)
                except IOError:
                    print("*** FATAL *** Cannot open the file: ", self.inputDir +fileName)
                    sys.exit(1)
                else:
                    inTxt = fr.readlines()
                finally:
                    fr.close()


                try:
                    fw = open(self.outputDir+ fileName.replace(".txt","_hex.mem"), "w" )
                except PermissionError:
                    print("Please close the file and run again..")
                    sys.exit(1)
                except IOError:
                    print("*** FATAL *** Cannot create the file, ")
                    sys.exit(1)
                else:
                    for idx in range(0,len(inTxt)):
                        arr = inTxt[idx].split()
                        realD0 = np.int16(int(arr[0]))
                        realH0 = self.int16toHex(realD0)
                        imagD0 = np.int16(int(arr[1]))
                        imagH0 = self.int16toHex(imagD0)
                        arr = inTxt[idx+1].split()
                        realD1 = np.int16(int(arr[0]))
                        realH1 = self.int16toHex(realD1)
                        imagD1 = np.int16(int(arr[1]))
                        imagH1 = self.int16toHex(imagD1)
                        fw.write("%s\n"%(imagH1+realH1+imagH0+realH0))
                finally:
                    fw.close()        
    
    def genMemFileRandom(self):
        """ function """
        for fileName in ["random_in00.txt", "random_in01.txt"]:
            print(fileName)
            try:
                fw0 = open(self.outputDir + fileName, "w" )
                fw1 = open(self.outputDir + fileName.replace(".txt", "_hex.mem") , "w" )
            except PermissionError:
                print("Please close the file and run again..")
                sys.exit(1)
            except IOError:
                print("*** FATAL *** Cannot create the file, ")
                sys.exit(1)
            else:
                for i in range(0,4096):
                    x = random.randint(0, 0xffff)
                    fw0.write("%d\n"%(x))
                    fw1.write("%x\n"%(x))
            finally:
                fw0.close()
                fw1.close()				

        try:
            fr0 = open(self.outputDir + "random_in00.txt", "r" )
            fr1 = open(self.outputDir + "random_in01.txt", "r" )
            fw0 = open(self.outputDir + "golden.txt", "w" )
        except PermissionError:
            print("Please close the file and run again..")
            sys.exit(1)
        except IOError:
            print("*** FATAL *** Cannot create the file, ")
            sys.exit(1)
        else:
            f0 = fr0.readlines()
            f1 = fr1.readlines()
            fr0.close()
            fr1.close()
            for i in range(0,int(4096/2) ):
                x = int(f0[i*2].strip())
                y = int(f1[i*2].strip())
                fw0.write("%d "%(x+y))
                x = int(f0[(i*2)+1].strip())
                y = int(f1[(i*2)+1].strip())
                fw0.write("%d \n"%(x+y))
        finally:
            fw0.close()

    def memCheck(self):
        for dirName in self.fileDict.keys():
            self.inputDir = self.fileDict[dirName]["path"]
            self.outputDir = self.fileDict[dirName]["path"]
            for fileName in self.fileDict[dirName]["fileList"]:
                try:
                    fw = open(self.outputDir + fileName.replace(".txt","_check.txt"), "w" )
                    fr = open(self.outputDir + fileName.replace(".txt", "_hex.mem"), "r" )
                except PermissionError:
                    print("Please close the file and run again..")
                    sys.exit(1)
                except IOError:
                    print("*** FATAL *** Cannot create the file, ")
                    sys.exit(1)
                else:
                    inTxt = fr.readlines()
                    for idx in range(0, len(inTxt)):
                        data = inTxt[idx].strip()
                        realT0,imagT0= self.hex2Cint16(data[8:])
                        realT1,imagT1= self.hex2Cint16(data[:8])
                        fw.write("%d %d\n"%(realT0,imagT0))
                        fw.write("%d %d\n"%(realT1,imagT1))
                finally:
                    fw.close()
	
                
#----------------------------------------------------------------------------------------------------------------------
def main():
    os.system("clear")
    print("\n.. Executing.. : %s "%sys.argv[0])

    obj = MemGen()
    obj.setFileDict()
    obj.genMemFile()
    #obj.memCheck()
    #obj.genMemFileRandom()

    print("*_check.txt have been generated and the user can compare the files")
    print("Exitting %s ... "%sys.argv[0])
    print(""+"-"*80)  
    
    
if __name__=="__main__":
    main()
