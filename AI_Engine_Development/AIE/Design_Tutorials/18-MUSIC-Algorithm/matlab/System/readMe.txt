% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

To run the Music models:
 1. Configure the signal model and Music algorithm in testCfg.m under 
    "Configuration" folder. Models can run with the default settings.
 2. Simply type testMusic. Model will:
          a. Generate a snapshot matrix, and apply Music.
          b. Data will be be logged under ./data 

%% Music algorithm
-------------------
The music algorithm is contained in music.m file, @ the root directory, performs: 
 1. QRD according to GSSR (files under "GSSR" Folder)
 2. Noise sub-space estimation using Jacobi (1-sided) SVD (files under "NoiseSS"
    folder), and finally
 3. DOA estimation through null search (files under "DOA" Folder)
 
 Inputs: a snapshot matrix and a struct carrying the configuration parameters
 Output: a struct carrying MUSIC results


%% Signal Model 
---------------
The system is contained in systemModel.m uder "SystemModel" folder.
 1.  Generates n random (QPSK or BPSK) modulated  signals with random or
     fixed directional of arrivals.
 
 2.  Builds the Vandermonde matrix for steering vectors of a ULA. The 
     aperture of the ULA is (nUla-1) * dUla ; with  nULa and dUla the  
     configurable nbr of antenna elements and the element-spacing respectively. 
 
 3. Linear-Combines the signal m symbols and Vandermonde steering matrix, and adds 
    white Gaussian-distributed noise with a configured variance through the
    spec'd SNR.
   
   
%% Configuration parameters 
---------------------------
Parameters are carried by a struct defined in the testCfg.m file that can
be found under the "Configuration" folder.

>> Signal Model related parameters
    cfg.sigMod           := Modulation Scheme; 1:= BPSK, 2:= QPSk  
    cfg.mSamp            := Number of snapshots
    cfg.nSource          := Number of signal sources of signal
    cfg.sourceMinSepar   := Minimum separation between sources 
    cfg.sourceSpan       := The sapn of DOAs 

    cfg.nUla             := Apperture of ULA
    cfg.dUla             := Antenna Spacing in signal wavelength 
    cfg.Snr              := Value in dB @ ULA element

    % optional ... Set only if doa vector is explictly configured 
    %              In that case, nbr of sources is set to the specified vector length
 
    cfg.doaDeg           := Directions of arrival of source signals

 >> Music related parameters
    cfg.gssrModel        := 'row' or 'column' QRD GSSR 
    cfg.svdRotModel      := AMD-derivated Jacobi rotations; V1 or V2.
    cfg.svdNumIter       := Nbr of iterations of the Jacobi Algorithm
    cfg.noiseFloor       := in ]0, 1[; threshold for the normalized singular 
                            values  to decide the cut between the signal and 
                            noise sub-spaces (ss).
    cfg.noiseSsCutmeth   := 1 use noise floor; 2 := use number of sources
                            as a draw line between the signal and noise ss.
    cfg.musicSweepNPts   := Number of points to sweep looking for DOAs
    cfg.nullThreshold    := Null Search threshold in the inverse spatial 
                            spectrum
    cfg.musicSweepSpan   := Sweep Span in [-SweepSpan/2 SweepSpan/2]
    
 >> Log and Plot related parameters
    cfg.log              := 'on' or 'off'
    cfg.visiblePlot      := 'on' or 'off'

  