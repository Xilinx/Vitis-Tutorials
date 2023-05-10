
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
#import matplotlib.pyplot as plt

class IQData():
    def __init__(self, numpy_cplx_data,aietype='cint16',plio_width=32):
        """ Initialization """
        self.input_cplx_data = numpy_cplx_data
        self.parent_conn0, self.child_conn0 = mp.Pipe()
        self.aietype = aietype
        self.plio_width = plio_width
        self.rx_payload_len = -1
        self.tx_payload_len = -1
 
    def self_test(self):
        print("inside self_test")
        #iqdata =MakeInputStim(NSamps)
        self.input_cplx_data = MakeCountingPattern(16)
        #self.plot_results(self.input_cplx_data,iqdata)

        #this is a test plot
        t1 = self.convert_numpy_to_bytes()
        self.convert_bytes_to_numpy(t1)

        #self.tx_to_aie(self.convert_numpy_to_bytes(),True)


    
    def rx_from_aie(self):        
        payload = self.out0_util.sample_transaction()
        print("Rx Payload1(pasteable)= ")
 
        #this magic makes sure the first byte isnt interpreted 
        #as an ASCII encoding character and just prints the value
        #print(''.join(r'\x'+hex(letter)[2:] for letter in payload.data))
        print(payload.data)
        #print(payload.tkeep)

        print("Rx payload len=")
        print(len(payload.data))
        
        #formatString = "<"+str(len(payload.data)//2)+"h"
        #rxData = struct.unpack(formatString, payload.data)
        #print("Rx payload finished")
        
        cvec = self.convert_bytes_to_numpy(payload.data)
        self.child_conn0.send(cvec)
        print("Rx payload tx to parent")

        
    def tx_to_aie(self,iq_data_as_bytes,test):
                             
        NumBytesToSend = len(iq_data_as_bytes)
        print("xmit: len Bytes = %d" % NumBytesToSend)

        NumBytesPerBeat = self.plio_width//8 
        
        NumTransports = NumBytesToSend//NumBytesPerBeat
        
        print("NumBytesToSend=%d,NumBytesPerBeat=%d,NumTransports=%d" % (NumBytesToSend,NumBytesPerBeat,NumTransports))
        
        for i in range(NumTransports):
            
            data2send = iq_data_as_bytes[(i*NumBytesPerBeat):(i*NumBytesPerBeat)+NumBytesPerBeat]
            
            if(test == False):              
                    payload = xtlm_ipc.axi_stream_packet()
                    payload.data_length = NumBytesPerBeat

                    if(i == NumTransports-1):
                        payload.tlast = True
                        print("Tlast sent!")
                    else:
                        payload.tlast = False
            
                    payload.data  =data2send

                    if(self.plio_width == 32):
                        payload.tkeep = bytes(bytearray(struct.pack("B", 0xF)))
                    elif(self.plio_width == 64):
                        payload.tkeep = bytes(bytearray(struct.pack("H", 0xFF)))
                    elif(self.plio_width == 128):                        
                        payload.tkeep = bytes(bytearray(struct.pack("I", 0xFFFF)))

                    print("Payload Num: %d" % i)
                    print(data2send)
            
            #print(''.join(r'\x'+hex(letter)[2:] for letter in data2send))
                            
            if(test == False):
                    self.in0_util.b_transport(payload)               

        print("Finished sending")

    def run_test(self, ipc=False):
  
        if ipc:
            self.in0_util  = ipc_axis_master_util("sim_ipc_axis_master")        
            self.out0_util = ipc_axis_slave_util("sim_ipc_axis_slave")
            
        self.tx_to_aie(self.convert_numpy_to_bytes(),False)
        print("Data sent to AIE. Waiting for results...")  

        if ipc:
            p= mp.Process(target=self.rx_from_aie())
            p.start()
            aie_output = self.parent_conn0.recv()
            print("Data received from AIE ") 
            
            #print("z aie: ", z_aie)
            p.join()

        print(aie_output)
        self.plot_results(self.input_cplx_data,aie_output)
        
   
        input("Enter any key to end simulation")
        self.out0_util.disconnect()
        self.in0_util.end_of_simulation()
        print("Disconnected all IPC handles.. done!")  
        
    def convert_numpy_to_bytes(self):
        #print(self.input_cplx_data)
        L = len(self.input_cplx_data)
        data = self.input_cplx_data
        print("Cplx Samples Len = %d" % L)


        if(self.aietype == "cint16"):
            rVec = np.real(data).astype(np.int16)
            iVec = np.imag(data).astype(np.int16)
            
            out2column = np.zeros((L,2)).astype(np.int16)

        elif(self.aietype =='cfloat'):
            print("cfloat!")
            rVec = np.real(data)
            iVec = np.imag(data)
            out2column = np.zeros((L,2)).astype(np.single)

        # elif(self.aietype =='cint16_plio128'):
        #     print("cint16_PLIO128!")
        #     #its unclear to me if i have to pack this differently...
        #     rVec = np.real(data)
        #     iVec = np.imag(data)
        #     out2column = np.zeros((L,2)).astype(np.int16)

        else:
            print("Not supported type!")

        
        out2column[:,0] = rVec
        out2column[:,1] = iVec

        print("Byte array to send")    
        print(''.join(r'\x'+hex(letter)[2:] for letter in out2column.tobytes()))
        print("outlen=")
        #print(len(out2column.tobytes()  ) )
        
        return out2column.tobytes()

    def convert_bytes_to_numpy(self,byte_array):
        #print("convert back byte_array =")
        #print(byte_array)
        #print(''.join(r'\x'+hex(letter)[2:] for letter in byte_array))

        if(self.aietype == "cint16"):
            print("AIE TYPE = cint16")
            formatString = "<"+str(len(byte_array)//2)+"h"
            upack = struct.unpack(formatString, byte_array) 

            ivec = upack[0:len(upack):2]
            rvec = upack[1:len(upack):2]


        elif(self.aietype =='cfloat'):
            print("AIE TYPE = cfloat")
            formatString = "<"+str(len(byte_array)//4)+"f"
            upack = struct.unpack(formatString, byte_array)

            print("Len Rx Array=")
            print(len(byte_array))
            #allgoodvalues = upack[0:len(upack):2]
            ivec = upack[0:len(upack):2]
            rvec = upack[1:len(upack):2]

            #Shouldnt need this
            # if(len(ivec) != len(rvec)):
            #     shorterlen = np.min((len(ivec),len(rvec)))
            #     ivec = ivec[:shorterlen]
            #     rvec = rvec[:shorterlen]

        # elif(self.aietype =='cint16_plio128'):
        #     formatString = "<"+str(len(byte_array)//2)+"h"
        #     upack = struct.unpack(formatString, byte_array)

        #     #print(upack)
        #     rvec = upack[0:len(upack):2]
        #     ivec = upack[1:len(upack):2]


        else:
            print("Not supported type!")

        cVec = np.array(rvec) + 1j*np.array(ivec)
        print("back to numpy")
        #print(cVec)
                
        return cVec
    
    def plot_results(self,aie_in,aie_out,style='t'):
        print("In Plot")
        ##AIE IN
        #plt.plot( list(range(0,len(aie_in)))  ,np.real(aie_in),label ="aie_in R")
        #plt.plot( list(range(0,len(aie_in)))  ,np.imag(aie_in),label ="aie_in I")

        ### SKIPPING FFT FOR CFLOAT EXAMPLE 
        ##Perform Golden Operation on AIE IN to generate Golden/reference output
        #golden_iq_out = np.fft.fftshift(np.fft.fft(aie_in))
        #golden_iq_out = golden_iq_out/4 #DSPLIB FFT HAS OUTPUT = MATLAB*4. Compensate for this.
        #aie_out_shft  = np.fft.fftshift(aie_out)

        #plt.plot( list(range(0,len(golden_iq_out))),np.abs(golden_iq_out),label ="Golden FFT - MAG",marker="+")
        #plt.plot( list(range(0,len(aie_out)))      ,np.abs(aie_out_shft),label  ="AIE OUT - MAG")        
        

        #AIE OUT
        #plt.plot( list(range(0,len(aie_out))),np.real(aie_out),label ="aie_out R")
        #plt.plot( list(range(0,len(aie_out))),np.imag(aie_out),label  ="aie_out I ")        
        

        #plt.legend()
        #plt.show()    
    
        # TriplePlot(aie_in,"Aie IN",file_or_plot="plot",logplot=True)
        # TriplePlot(aie_out,"Aie out",file_or_plot="plot",logplot=True)
    

# def TriplePlot(iqdata,name,file_or_plot="file",FsMHz=245.76,logplot=False):
#     n = len(iqdata)
#     k = np.arange(n)
#     T = n/FsMHz
#     frq = (k-k.size/2)/T

#     fig, ax = plt.subplots(2, 1,figsize=(10, 5))
#     ax[0].plot(np.real(iqdata), 'r')
#     if(iqdata.dtype == 'complex'):
#         ax[0].plot(np.imag(iqdata),'b')

#     ax[0].set_xlabel('Time Domain - Sample count')

#     fdom = np.fft.fftshift(np.fft.fft(iqdata))
#     if(logplot == True):
#         logfdom = 20*np.log10(np.abs(fdom)+1e-9)
#         NBits = 10
#         auto_scale_pk = np.max(logfdom)
#         auto_scale_range = 20*np.log10(2**(NBits-1))
#         #print(auto_scale_range)
#         if(auto_scale_pk-auto_scale_range > 6):
#             #dont use this autoscale if the data doesnt have at least 6db range
#             ax[1].set_ylim([(auto_scale_pk*1.1)-auto_scale_range,auto_scale_pk*1.1])
    
#         ax[1].set_ylabel('db (20log)')
#     else:
#         logfdom= np.abs(fdom)
#         ax[1].set_ylabel('linear')

#     ax[1].plot(frq,logfdom,'r')
#     ax[1].grid(True)

#     ax[1].set_xlabel('Frequency Domain ')
#     ax[0].set_title(name)

#     if(file_or_plot == "file"):
#         figfilename = name + ".png"
#         plt.savefig(figfilename,bbox_inches='tight')
#     else:
#         plt.show()
               

def MakeInputStim(Nsamps=1024):
   
    n = np.arange(0,Nsamps)
    Fs = 245.76e6
    Ftone = 30.72e6/16
    cplxTone =  1.0* np.exp(-2*1j*np.pi*Ftone/Fs*n)
    #cplxTone += 1.0* np.exp(-2*1j*np.pi*15.36e6/Fs*n)
    
    Nbits =  16-4
    Nbits = Nbits -2 #removed additional bits to help with FFT processing gain
    mx = np.max(np.abs(cplxTone))
    cscaled = np.round(cplxTone/mx  * 2**(Nbits-1)-1)
    
    return cscaled
    
def MakeCountingPattern(Nsamps=16):
    #print("inside counting")
    #print(Nsamps)
    n = np.arange(0,Nsamps)
    nneg = -1*np.arange(0,Nsamps)

    cscale = n + 1j*nneg

    return cscale

if __name__ == "__main__":
    
    NSamps=32

    #iqdata =MakeInputStim(NSamps)
    iqdata = MakeCountingPattern(NSamps)
    obj = IQData(iqdata,aietype="cint16",plio_width=64)
    #obj.self_test()
    print("Starting Test...")
    obj.run_test(ipc=True)
    print("Test complete")
        
