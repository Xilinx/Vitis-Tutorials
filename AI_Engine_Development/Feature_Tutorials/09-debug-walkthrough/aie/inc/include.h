/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

// Downlink Beamforming parameters
#define NUM_PRBS     8

// PDSCH Parameters
#define PDSCH_SHFT              17
#define PDSCH_IN_DATA_BLKSZ     (NUM_PRBS*8*12)
#define PDSCH_IN_COEF_BLKSZ     (NUM_PRBS*8*8)
#define PDSCH_OUT_DATA_BLKSZ    (NUM_PRBS*8*12)
#define PDSCH_IN_DATA_WINSZ     (4*PDSCH_IN_DATA_BLKSZ)
#define PDSCH_IN_COEF_WINSZ     (4*PDSCH_IN_COEF_BLKSZ)
#define PDSCH_OUT_DATA_WINSZ    (4*PDSCH_OUT_DATA_BLKSZ)

#define READSCD (getc_scd())
#define READSS0 (getc_wss(0))


#endif /* __INCLUDE_H__ */
