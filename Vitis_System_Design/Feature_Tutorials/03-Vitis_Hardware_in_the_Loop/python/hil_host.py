#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz
#
# HIL host script for the M=16, SSR=8, P/Q=8/7 channelizer.
# Streams wideband signal samples to the hardware channelizer via HIL,
# collects the channelized outputs, and displays spectra.
#

import numpy as np
import matplotlib.pyplot as plt
from hil import hil
import varray as va
from wb_src import wb_src
import time

# Configure host code
n_iter = 1024           # specifies how many times HIL run function is called
N = 16384               # sets block size for HIL processing
tictocsum = 0.0         # total HIL processing time
totalsamp = 0           # total output samples
stats_en = False        # delays accumulating stats until output is available
fft_sz = 1024           # for spectrum display
overlap = fft_sz // 2   # for spectrum display
step = fft_sz - overlap  # for spectrum display

# Instantiate HIL object
hil_dir = '../channelizer_hil'
hil_server_ip = "10.23.196.92"   # modify for your IP
hil_chnlzr = hil(hil_dir=hil_dir)
hil_chnlzr.setConnection(ip_addr=hil_server_ip)

# Configure HIL object - adjust frame size and timeout to optimize HIL for your network
chan_frame_size = 4 * N
timeout = 0.01
for prt_idx in range(7):
    hil_chnlzr.setInputFrameSize(prt_idx, chan_frame_size)
for prt_idx in range(8):
    hil_chnlzr.setOutputFrameSize(prt_idx, chan_frame_size)
inputSpec = hil_chnlzr.getInputSpec()
outputSpec = hil_chnlzr.getOutputSpec()
print(inputSpec)
print(outputSpec)
hil_chnlzr.setTimeout(timeout)
hil_chnlzr.initialize()

# Create a FIFO to align HIL output
ssro = 8
fifo_max = 5 * chan_frame_size
fifo_buffer = np.zeros((fifo_max, ssro), dtype=complex)
fifo_ptr = np.zeros(ssro, dtype=int)
fifo_size = np.zeros(ssro, dtype=int)

# Configure wideband signal source
Fs = 8.75e9
upsmp = 28
chan_en = [0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0]       # enable individual sub-channels
qam_en = [0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0]        # modulate selected sub-channels
swp_en = [0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0]        # enable carrier sweep on sub-channels
swp_rate = [0,0.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0]   # set carrier sweep rate: range [-1.0, +1.0]
wbs = wb_src(N,upsmp,chan_en,qam_en,swp_en,swp_rate)

# Setup spectrum display for wideband input
fig_src, ax_src = plt.subplots()
fig_src.patch.set_facecolor('black')
ax_src.set_facecolor('black') 
fft_freqs = np.fft.fftshift(np.fft.fftfreq(fft_sz, 1/Fs))
line, = ax_src.plot(fft_freqs/1e9,np.zeros(fft_sz),color='cyan')
for spine in ax_src.spines.values():
    spine.set_color('white')
ax_src.set_ylim(-20, 50)
ax_src.set_xlim(-Fs/2e9, Fs/2e9)
ax_src.set_title("Wideband Signal Input", color='white')
ax_src.set_xlabel("Frequency (GHz)", color='white')
ax_src.set_ylabel("Power (dB)", color='white')
ax_src.tick_params(colors='white')
ax_src.grid(True, color='gray')

# Setup spectrum display for channelizer outputs
disp_slct = [1, 2, 5, 6]
fig, axes = plt.subplots(2, 2, figsize=(8,8))
fig.subplots_adjust(hspace=0.4,wspace=0.4)
for ax in axes.flat:
    ax.set_facecolor('black') 
    for spine in ax.spines.values():
        spine.set_color('white')
    ax.set_ylim(-40, 50)
    ax.set_xlim(-(8/7)*Fs/(16*2e6), (8/7)*Fs/(16*2e6))
    ax.set_xlabel("Frequency (MHz)", color='white')
    ax.set_ylabel("Power (dB)", color='white')
    ax.tick_params(colors='white')
    ax.grid(True, color='gray')
fig.patch.set_facecolor('black')
fft_freqo = np.fft.fftshift(np.fft.fftfreq(fft_sz, 16/((8/7)*Fs)))
line0, = axes[0,0].plot(fft_freqo/1e6,np.zeros(fft_sz),color='yellow')
line1, = axes[0,1].plot(fft_freqo/1e6,np.zeros(fft_sz),color='yellow')
line2, = axes[1,0].plot(fft_freqo/1e6,np.zeros(fft_sz),color='yellow')
line3, = axes[1,1].plot(fft_freqo/1e6,np.zeros(fft_sz),color='yellow')
axes[0,0].set_title(f"Channel {disp_slct[0]}", color='white')
axes[0,1].set_title(f"Channel {disp_slct[1]}", color='white')
axes[1,0].set_title(f"Channel {disp_slct[2]}", color='white')
axes[1,1].set_title(f"Channel {disp_slct[3]}", color='white')

plt.ion()
plt.show()

# Main processing loop
for _ in range(n_iter):
    # generate wideband samples
    smp = wbs.get_samp()

    # update input spectrum plot
    segments = [smp[i:i+fft_sz] for i in range(0, len(smp)-fft_sz+1, step)]
    pwrspec = []
    for seg in segments:
        seg_windowed = seg * np.hanning(fft_sz)
        fft_result = np.fft.fft(seg_windowed, n=fft_sz, axis=0)
        pwrspec.append(np.abs(fft_result)**2)
    avg_pwrspec = np.mean(pwrspec, axis=0)
    line.set_ydata(np.fft.fftshift(10*np.log10(avg_pwrspec)))
    fig_src.canvas.draw()
    fig_src.canvas.flush_events()

    # break the wideband signal data into SSR streams
    Nwbs = smp.size
    chnlzr_in = np.zeros((Nwbs//7,7), dtype = np.complex64)
    for col in range(7):
        src_idx = np.column_stack((np.arange(13-col,Nwbs,14), np.arange(6-col,Nwbs,14))).T.reshape(-1,order='F')
        chnlzr_in[:,col] = (2**15) * (smp[src_idx])

    # create a list of Vitis arrays to send to HIL
    send_data = [va.array(chnlzr_in[:, i], dtype=va.cint16) for i in range(7)]
  
    # process data with HIL
    tic = time.time()
    return_data = hil_chnlzr.run(send_data)
    if stats_en:
        tictocsum += time.time() - tic

    # Put new samples into FIFO
    for i in range(ssro):
        nsmp = return_data[i].shape[0]
        if nsmp > 0:
            mod_idx = (fifo_ptr[i] + fifo_size[i] + np.arange(nsmp)) % fifo_max
            fifo_buffer[mod_idx,i] = np.array(return_data[i], dtype=complex)
            fifo_size[i] += nsmp

    # Determine number of output samples to read from fifo.
    # More data than specified by chan_frame_size may be available for
    # reading, but a fixed amount is read so that output displays function
    # more like real spectrum analyzers. This also adds latency between
    # input and output displays.
    if np.min(fifo_size) == 0:
        nout = 0
    else:
        nout = chan_frame_size

    if stats_en:
        totalsamp += ssro * nout
        print(f"HIL processing throughput = {np.round(totalsamp / tictocsum)} samples per second.")
    elif nout != 0:
        stats_en = True

    # Extract data from FIFO for display
    if nout != 0:
        chnlzr_ssr_out = np.zeros((nout,ssro), dtype=complex)
        for i in range(ssro):
            mod_idx = (fifo_ptr[i] + np.arange(nout)) % fifo_max
            chnlzr_ssr_out[:,i] = fifo_buffer[mod_idx,i]
            fifo_ptr[i] = (fifo_ptr[i] + nout) % fifo_max
            fifo_size[i] -= nout
        
        # Extract individual channel streams from channelizer SSR output
        ssr_combined = (2**-15) * (chnlzr_ssr_out.flatten(order='F'))
        Nall = ssr_combined.size
        chnlzr_out = np.zeros((Nall // 16, 16), dtype = complex)
        for i in range(4):
            src_idx = np.column_stack((np.arange(i,Nall//8,4), np.arange(Nall//8+i,Nall//4,4))).T.reshape(-1,order='F')
            chnlzr_out[:,i] = ssr_combined[src_idx]
        for i in range(4):
            src_idx = np.column_stack((np.arange(Nall//4+i,3*Nall//8,4), np.arange(3*Nall//8+i,Nall//2,4))).T.reshape(-1,order='F')
            chnlzr_out[:,i+4] = ssr_combined[src_idx]
        for i in range(4):
            src_idx = np.column_stack((np.arange(Nall//2+i,5*Nall//8,4), np.arange(5*Nall//8+i,3*Nall//4,4))).T.reshape(-1,order='F')
            chnlzr_out[:,i+8] = ssr_combined[src_idx]
        for i in range(4):
            src_idx = np.column_stack((np.arange(3*Nall//4+i,7*Nall//8,4), np.arange(7*Nall//8+i,Nall,4))).T.reshape(-1,order='F')
            chnlzr_out[:,i+12] = ssr_combined[src_idx]
    else:
        chnlzr_out = np.array([], dtype=complex)

    # Display spectrum of output channels
    if chnlzr_out.size != 0:
        segments = [chnlzr_out[i:i+fft_sz,disp_slct[0]] for i in range(0, chnlzr_out.shape[0]-fft_sz+1, step)]
        pwrspec = []
        for seg in segments:
            seg_windowed = seg * np.hanning(fft_sz)
            fft_result = np.fft.fft(seg_windowed, n=fft_sz, axis=0)
            pwrspec.append(np.abs(fft_result)**2)
        avg_pwrspec = np.mean(pwrspec, axis=0)
        line0.set_ydata(np.fft.fftshift(10*np.log10(avg_pwrspec)))
        segments = [chnlzr_out[i:i+fft_sz,disp_slct[1]] for i in range(0, chnlzr_out.shape[0]-fft_sz+1, step)]
        pwrspec = []
        for seg in segments:
            seg_windowed = seg * np.hanning(fft_sz)
            fft_result = np.fft.fft(seg_windowed, n=fft_sz, axis=0)
            pwrspec.append(np.abs(fft_result)**2)
        avg_pwrspec = np.mean(pwrspec, axis=0)
        line1.set_ydata(np.fft.fftshift(10*np.log10(avg_pwrspec)))
        segments = [chnlzr_out[i:i+fft_sz,disp_slct[2]] for i in range(0, chnlzr_out.shape[0]-fft_sz+1, step)]
        pwrspec = []
        for seg in segments:
            seg_windowed = seg * np.hanning(fft_sz)
            fft_result = np.fft.fft(seg_windowed, n=fft_sz, axis=0)
            pwrspec.append(np.abs(fft_result)**2)
        avg_pwrspec = np.mean(pwrspec, axis=0)
        line2.set_ydata(np.fft.fftshift(10*np.log10(avg_pwrspec)))
        segments = [chnlzr_out[i:i+fft_sz,disp_slct[3]] for i in range(0, chnlzr_out.shape[0]-fft_sz+1, step)]
        pwrspec = []
        for seg in segments:
            seg_windowed = seg * np.hanning(fft_sz)
            fft_result = np.fft.fft(seg_windowed, n=fft_sz, axis=0)
            pwrspec.append(np.abs(fft_result)**2)
        avg_pwrspec = np.mean(pwrspec, axis=0)
        line3.set_ydata(np.fft.fftshift(10*np.log10(avg_pwrspec)))
        fig.canvas.draw()
        fig.canvas.flush_events()

# drain loop
drain_iter = 16
empty_inputs = [va.array([], va.cint16) for _ in range(7)]
for _ in range(drain_iter):
    hil_chnlzr.run(*empty_inputs)

print(hil_chnlzr.getStats())
