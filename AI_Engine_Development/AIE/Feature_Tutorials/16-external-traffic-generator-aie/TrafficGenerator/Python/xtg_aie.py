
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


import os, sys

import numpy as np
import multiprocessing as mp
import struct
import logging
import matplotlib.pyplot as plt

from xilinx_xtlm import ipc_axis_master_util
from xilinx_xtlm import ipc_axis_slave_util
from xilinx_xtlm import xtlm_ipc

class ExternalTraffic():
    def __init__(self, inputf, inputfpc):
        """Initialization"""
        # Pipes for communication
        self.inputf = inputf
        self.inputfpc = inputfpc
        self.parent0, self.child0 = mp.Pipe() # Polar_clip_in --> Top
        self.parent1, self.child1 = mp.Pipe() # S2MM --> Top

    def mm2s(self):
        """Sending data to AIE from memory"""

        with open(self.inputf) as f:
            L = f.readlines()
            logging.info("MM2S Sending " + str(len(L)) + " complex int16 data...")
            # Packets of 128 cint16
            NLines = 0
            PacketData = []
            for line in L:
                NLines = NLines + 1
                real,imag = line.split(' ') # Read in data as real and imag
                # 2's Complement Data
                # Describe the transformation of value
                # Example: -4751, -2855
                # Why this is needed (2 values to 1 value)
                a = ((int('0xFFFF',16) + int('0x1',16)) + int(real)) & 0xFFFF
                b = ((int('0xFFFF',16) + int('0x1',16)) + int(imag)) & 0xFFFF
                b = b << 16
                c = a | b
                PacketData.append(c)

                # logging.info("[MM2S]: " + line)
                if NLines%128 == 0:
                    logging.info("mm2s: Sending Packet " + str(int(NLines/128)))
                    payload = xtlm_ipc.axi_stream_packet()
                    payload.data_length = 4*128
                    payload.tlast = True
                    format_string = "<"+str(int(payload.data_length/4))+"I"
                    payload.data = bytes(bytearray(struct.pack(format_string, *tuple(PacketData))))
                    self.mm2s_util.b_transport(payload)
                    PacketData = []


    def s2mm(self):
        """Receiving data from AIE to memory"""
        NData = 0
        logging.info("s2mm reading...")
        rxData = list()

        while NData<1024:
            payload = self.s2mm_util.sample_transaction()
            NData = NData+len(payload.data)/4
            formatString = "<"+str(len(payload.data)//4)+"i" # Data is still in cint16
            tmp = struct.unpack(formatString, payload.data)
            for d in tmp:
                rxData.append(d)
            if NData%256 == 0:
                logging.info("s2mm getting Packet " + str(int(NData/256)))

        self.child1.send(rxData)



    def polar_clip_in(self):
        """Polar Clip Kernel Receiving"""
        NData = 0
        pc_output = open("polar_clip_in.txt", "w")
        logging.info("polar_clip_in reading...")
        rxData = list()

        while NData<1024:
            payload = self.polar_clip_in_util.sample_transaction()
            NData = NData+len(payload.data)/4
            formatString = "<"+str(len(payload.data)//4)+"i" # Data is still in cint16
            tmp = struct.unpack(formatString, payload.data)
            for d in tmp:
                rxData.append(d)
            if NData%256 == 0:
                logging.info("polar_clip_in getting Packet " + str(int(NData/256)))

        self.child0.send(rxData) # Data comes in as tuple, only 1 value is needed


    def polar_clip_out(self):
        """Polar Clip Kernel Sending"""
                # Getting the last line so Tlast can be sent
        with open(self.inputfpc) as f:
            # logging.info("Getting last line for polar_clip")
            last_line = f.readlines()[-1]

        with open(self.inputfpc) as f:
            L = f.readlines()
            logging.info("PolarClip Sending " + str(len(L)) + " complex int16 data...")
            # Packets of 256 cint16
            NLines = 0
            PacketData = []
            for line in L:
                NLines = NLines + 1
                real,imag = line.split(' ') # Read in data as real and imag
                # 2's Complement Data
                # Describe the transformation of value
                # Example: -4751, -2855
                # Why this is needed (2 values to 1 value)
                a = ((int('0xFFFF',16) + int('0x1',16)) + int(real)) & 0xFFFF
                b = ((int('0xFFFF',16) + int('0x1',16)) + int(imag)) & 0xFFFF
                b = b << 16
                c = a | b
                PacketData.append(c)

                # logging.info("[MM2S]: " + line)
                if NLines%256 == 0:
                    logging.info("polar_clip: Sending Packet " + str(int(NLines/256)))
                    payload = xtlm_ipc.axi_stream_packet()
                    payload.data_length = 4*256
                    payload.tlast = True
                    format_string = "<"+str(int(payload.data_length/4))+"I"
                    payload.data = bytes(bytearray(struct.pack(format_string, *tuple(PacketData))))
                    self.polar_clip_out_util.b_transport(payload)
                    PacketData = []


    def run(self):
        """Run the design"""
        logging.info("Creating IPC Util Objects...")
        self.mm2s_util = ipc_axis_master_util("DataIn1")
        self.polar_clip_in_util = ipc_axis_slave_util("clip_in")
        self.polar_clip_out_util = ipc_axis_master_util("clip_out")
        self.s2mm_util = ipc_axis_slave_util("DataOut1")

        logging.info("Running mm2s...")
        tx = mp.Process(target=self.mm2s)
        tx.start()
        logging.info("Running polar_clip_in...")
        pc = mp.Process(target=self.polar_clip_in)
        pc.start()

        logging.info("Running polar_clip_out...")
        pc_tx = mp.Process(target=self.polar_clip_out)
        pc_tx.start()
        logging.info("Running s2mm...")
        p = mp.Process(target=self.s2mm)
        p.start()

        pc_data = self.parent0.recv() # Receive data from polar_clip_in
        pc.join()
        logging.info("polar_clip_in completed!")

        data = self.parent1.recv()
        p.join()
        logging.info("s2mm completed!")

        pc_tx.join()
        logging.info("polar_clip_out completed!")

        tx.join()
        logging.info("mm2s completed!")

        with open('DataOut1.txt','w') as f:
            for d in data:
                f.write(str(d)+' \n')


        # plot(self.convert_input_data(), "input")
        # plot(data, "output")


    def convert_input_data(self):
        """Convert the input data to be used in plot"""
        self.inputData = np.genfromtxt(self.inputf)
        formattedData = np.array(self.inputData).astype(np.int16)
        data = np.hstack(formattedData)
        return data

def plot(data,data_type):
    """Plot Data from Numpy Data"""
    data = np.array(data).astype(np.int16)
    data = np.hstack(data)
    plt.scatter(data[0::2],data[1::2])

    if(data_type == "output"):
        plt.axis([-1,4,-1,4])
        plt.axhline(1.5, color='black')
        plt.axvline(1.5, color='black')
    else:
        plt.axhline(1.5, color='black')
        plt.axvline(1.5, color='black')
    plt.show()
    # input("Enter any key to dismiss plot.")

if __name__ == "__main__":
    logging.basicConfig(format="%(asctime)s: %(message)s", level=logging.INFO, datefmt="%H:%M:%S")

    inputFile = "./data/mm2s.txt"
    inputPCFile = "./data/polar_clip_out.txt"
    design = ExternalTraffic(inputFile,inputPCFile)

    design.run()
