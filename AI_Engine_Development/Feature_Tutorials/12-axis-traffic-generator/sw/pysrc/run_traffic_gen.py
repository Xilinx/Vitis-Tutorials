
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

import os
os.environ['OPENBLAS_NUM_THREADS'] = '1'

from xilinx_xtlm import ipc_axis_master_util
from xilinx_xtlm import ipc_axis_slave_util
from xilinx_xtlm import xtlm_ipc
import struct
import multiprocessing as mp 
import numpy as np
import copy as copy
import matplotlib.pyplot as plt
import sys

class IQData():
    def __init__(self, numpy_cplx_data,aietype='cint16',plio_width=32,supressplots=0):
        """ Initialization """
        self.input_cplx_data = numpy_cplx_data
        self.parent_conn0, self.child_conn0 = mp.Pipe()
        self.aietype = aietype
        self.plio_width = plio_width
        self.rx_payload_len = -1
        self.tx_payload_len = -1
        self.supressplots = supressplots
 
    def self_test(self):
        print("inside self_test")        
        self.input_cplx_data = MakeCountingPattern(32)
        #self.plot_results(self.input_cplx_data,iqdata)

        t1 = self.convert_numpy_to_bytes()
        self.convert_bytes_to_numpy(t1)
    
    def rx_from_aie(self):        
        payload = self.rx_axis.sample_transaction()
        #This call blocks until the AIE passes some data to the AXI SIM IPC SLAVE
           
        cvec = self.convert_bytes_to_numpy(payload.data)
        self.child_conn0.send(cvec)
        print("Received AIE Output. Sending to parent thread for processing")

        
    def tx_to_aie(self,iq_data_as_bytes,test):
                             
        NumBytesToSend = len(iq_data_as_bytes)
        print("xmit: len Bytes = %d" % NumBytesToSend)

        NumBytesPerBeat = self.plio_width//8 
        
        NumTrans = NumBytesToSend//NumBytesPerBeat
        
        print("NumBytesToSend=%d,NumBytesPerTransaction=%d,TotalTransactions=%d" % (NumBytesToSend,NumBytesPerBeat,NumTrans))
        
        for i in range(NumTrans):
            
            data2send = iq_data_as_bytes[(i*NumBytesPerBeat):(i*NumBytesPerBeat)+NumBytesPerBeat]
            #Stride through byte array in steps of BytesPerBeat
                                     
            payload = xtlm_ipc.axi_stream_packet()
            #Create a axi stream packet object

            payload.data_length = NumBytesPerBeat
            #Tell the object how much data will be sent in bytes

            if(i == NumTrans-1):
                payload.tlast = True
                print("Tlast sent!")
            else:
                payload.tlast = False
    
            payload.data  =data2send
    
            self.tx_axis.b_transport(payload)
            #Send the data to the ipc master        

        print("Finished sending")

    def run_test(self, ipc=False):
  
        if ipc:
            self.tx_axis  = ipc_axis_master_util("tx_iqdata")        
            self.rx_axis = ipc_axis_slave_util("rx_iqdata")
            #Create both Master and Slave ipc utils. 
            #The argument strings must match the names in system.cfg
            
        self.tx_to_aie(self.convert_numpy_to_bytes(),False)
        print("Data sent to AIE. Waiting for results...this may take a few minutes")  

        if ipc:
            p= mp.Process(target=self.rx_from_aie())
            p.start()
            aie_output = self.parent_conn0.recv()
            print("Data received from AIE ") 
    
            p.join()
        
        if (not self.supressplots):
            self.plot_results(self.input_cplx_data,aie_output)
            #input("Enter any key to end simulation")   

        self.rx_axis.disconnect()
        self.tx_axis.end_of_simulation()
        print("Disconnected all IPC handles.. done!")  
        
    def convert_numpy_to_bytes(self):            
        L = len(self.input_cplx_data)
        data = self.input_cplx_data        


        if(self.aietype == "cint16"):
            rVec = np.real(data).astype(np.int16)
            iVec = np.imag(data).astype(np.int16)
            
            out2column = np.zeros((L,2)).astype(np.int16)

        elif(self.aietype =='cfloat'):
            print("cfloat!")
            rVec = np.real(data)
            iVec = np.imag(data)
            out2column = np.zeros((L,2)).astype(np.single)

        else:
            print("Not supported type!")

        
        out2column[:,0] = rVec
        out2column[:,1] = iVec

        #print("Byte array to send")    
        #print(''.join(r'\x'+hex(letter)[2:] for letter in out2column.tobytes()))
        #print("outlen=")

        
        return out2column.tobytes()

    def convert_bytes_to_numpy(self,byte_arry):
 
        if(self.aietype == "cint16"):
            formatString = "<"+str(len(byte_arry)//2)+"h"
            upack = struct.unpack(formatString, byte_arry) 

            ivec = upack[0:len(upack):2]
            rvec = upack[1:len(upack):2]


        elif(self.aietype =='cfloat'):
            formatString = "<"+str(len(byte_arry)//4)+"f"
            upack = struct.unpack(formatString, byte_arry)

            print("Len Rx Array=")
            print(len(byte_arry))
          
            ivec = upack[0:len(upack):2]
            rvec = upack[1:len(upack):2]

        else:
            print("Not supported type!")

        cVec = np.array(rvec) + 1j*np.array(ivec)        
                
        return cVec
    
    def plot_results(self,aie_in,aie_out,style='t'):
    
        ##AIE IN
        # plt.plot( list(range(0,len(aie_in)))  ,np.real(aie_in),label ="aie_in R")
        # plt.plot( list(range(0,len(aie_in)))  ,np.imag(aie_in),label ="aie_in I")
        
        #Perform Golden Operation on AIE IN to generate Golden/reference output
        golden_iq_out = np.fft.fftshift(np.fft.fft(aie_in))
        golden_iq_out = golden_iq_out/4 #DSPLIB FFT HAS OUTPUT = MATLAB*4. Compensate for this.
        aie_out_shft  = np.fft.fftshift(aie_out)

        plt.plot( list(range(0,len(golden_iq_out))),np.abs(golden_iq_out),label ="Golden FFT - MAG",marker="+")
        plt.plot( list(range(0,len(aie_out)))      ,np.abs(aie_out_shft),label  ="AIE OUT - MAG")        
        
        ###will show plot only when SHOW_PLOT is True
        plt.legend()
        if (len(sys.argv) == 4 and sys.argv[3] == "plot"): plt.show()	
    


def MakeInputStim(Nsamps=1024):
   
    n = np.arange(0,Nsamps)
    Fs = 245.76e6
    Ftone = 30.72e6/16
    cplxTone =  1.0* np.exp(-2*1j*np.pi*Ftone/Fs*n)
    
    Nbits =  16-4
    Nbits = Nbits -2 #removed additional bits to help with FFT processing gain
    mx = np.max(np.abs(cplxTone))
    cscaled = np.round(cplxTone/mx  * 2**(Nbits-1)-1)
    
    return cscaled
    
def MakeCountingPattern(Nsamps=16):
    n = np.arange(0,Nsamps)
    nneg = -1*np.arange(0,Nsamps)

    cscale = n + 1j*nneg

    return cscale

if __name__ == "__main__":
    for i, arg in enumerate(sys.argv):
        if( i == 1):
            cmd_line_pliowidth = int(arg)
        if( i == 2):
            skipplots=int(arg)
            print(skipplots)

    
    NSamps=1024
    iqdata =MakeInputStim(NSamps)
    #iqdata = MakeCountingPattern(NSamps)
    obj = IQData(iqdata,aietype="cint16",plio_width=cmd_line_pliowidth,supressplots=skipplots)
    obj.run_test(ipc=True)
    print("TEST PASSED")
