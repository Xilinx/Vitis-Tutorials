#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz
#
# Generates a composite wideband signal by summing up to 16 sub-channels.
# Each sub-channel can be independently enabled, QAM-modulated, and
# frequency-swept. Maintains internal state for seamless sample continuity
# across successive calls to get_samp().
#

import numpy as np
from scipy.signal import lfilter

class wb_src:
    def __init__(self,Nsmp,upsmp,chan_en,qam_en,swp_en,swp_rate):
        self.Nsmp = Nsmp
        self.upsmp = upsmp
        self.chan_en = chan_en
        self.qam_en = qam_en
        self.swp_en = swp_en
        self.swp_rate = swp_rate
        coef = rcosdesign(0.22,4,self.upsmp)
        self.qam_coef = self.upsmp * coef / np.sum(coef)
        self.fir_state = np.zeros((len(self.qam_coef)-1,16),dtype=np.complex64)
        self.prv_carrier_ph = np.random.randint(1,self.Nsmp+1,size=16)
        self.prv_sweep_ph = np.zeros(16,dtype=int)
        self.frq_nrm = np.array([0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1])
        self.nvar = 0.05
        self.gain = 1/6

    def get_samp(self):
        # array for individual channel samples
        wbsmp = np.zeros((self.upsmp*self.Nsmp,16),dtype=complex)

        # generate baseband QAM samples
        for i in range(16):
            if self.chan_en[i] and self.qam_en[i]:
                iq = 2*np.random.randint(0,4,size=(self.Nsmp,2))-3
                qsym = (iq[:,0] + 1j*iq[:,1]) / np.sqrt(10)
                up_len = len(qsym) * self.upsmp
                qsym_us = np.zeros(up_len, dtype=np.complex64)
                qsym_us[::self.upsmp] = qsym
                qsamp, self.fir_state[:,i] = lfilter(self.qam_coef,1,qsym_us,zi=self.fir_state[:,i])
                wbsmp[:,i] = qsamp

        # Constant channels
        I = np.where((np.array(self.chan_en) == 1) & (np.array(self.qam_en) == 0))[0]
        if I.size > 0:
            wbsmp[:, I] = np.ones((self.upsmp*self.Nsmp, 1), dtype=np.complex64)

        # Carrier modulation
        I = np.where(np.array(self.chan_en) == 1)[0]
        J = np.where((np.array(self.chan_en) == 1) & (np.array(self.swp_en) == 1))[0]
        if I.size > 0:
            cr_cntr = (np.tile(np.arange(1, self.upsmp * self.Nsmp + 1).reshape(-1, 1), (1, len(I))) + self.prv_carrier_ph[I]) % (2**32)
            self.prv_carrier_ph[I] = cr_cntr[-1, :]
            cr_cntr = cr_cntr * self.frq_nrm[I] / 16

            if J.size > 0:
                swp_cntr = (np.tile(np.arange(1, self.upsmp * self.Nsmp + 1).reshape(-1, 1), (1, len(J))) + self.prv_sweep_ph[J]) % (2**32)
                self.prv_sweep_ph[J] = swp_cntr[-1, :]
                swp_cntr = (swp_cntr**2) * np.array(self.swp_rate)[J] * 1e-8
                ia = [np.where(I == j)[0][0] for j in J]
                cr_cntr[:, ia] += swp_cntr

            carr_smp = np.exp(1j * 2 * np.pi * cr_cntr)
            wbsmp[:, I] *= carr_smp
    
        # add noise and set signal level
        noise = self.nvar * (np.random.randn(self.upsmp*self.Nsmp) + 1j * np.random.randn(self.upsmp*self.Nsmp))
        smp = self.gain * (np.sum(wbsmp, axis=1) + noise)
 
        return smp
            
def rcosdesign(alpha, span, sps):
    # Raised cosine filter design
    N = span * sps + 1
    t = np.arange(-span/2, span/2 + 1/sps, 1/sps)
    h = np.zeros_like(t)

    for i in range(len(t)):
        if t[i] == 0.0:
            h[i] = 1.0
        elif abs(t[i]) == 1 / (2 * alpha):
            h[i] = (np.pi / 4) * np.sinc(1 / (2 * alpha))
        else:
            numerator = np.sin(np.pi * t[i]) * np.cos(np.pi * alpha * t[i])
            denominator = (np.pi * t[i]) * (1 - (2 * alpha * t[i]) ** 2)
            h[i] = numerator / denominator
    h /= np.sum(h)
    return h
