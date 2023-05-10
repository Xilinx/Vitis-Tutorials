/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

/* Matrix Multiply C = A x B

Floating-Point case
  Dimensions
    A: Ra x Ca    With Ra = 2.ra   and Ca = 8.ca
    B: Rb x Cb    With  Rb = Ca (= 8.ca)  and Cb = 8.cb

    C: Ra x Cb

Complex Floating-Point case
  Dimensions
    A: Ra x Ca    With Ra = 2.ra  and Ca = 4.ca
    B: Rb x Cb    With Rb = Ca (=8.ra) and Cb = 4.cb

    C: Ra x Cb
*/

// Floating point example
#define F_Ra 16
#define F_Ca 8
#define F_Rb (F_Ca)
#define F_Cb 8

#define F_Rc (F_Ra)
#define F_Cc (F_Cb)

// Complex Floating-Point Example
#define CF_Ra 16
#define CF_Ca 4
#define CF_Rb (CF_Ca)
#define CF_Cb 4

#define CF_Rc (CF_Ra)
#define CF_Cc (CF_Cb)

// Window size
#define NSAMPLES_WINDOW_F_A (F_Ra*F_Ca)
#define NSAMPLES_WINDOW_F_B (F_Rb*F_Cb)
#define NSAMPLES_WINDOW_F_C (F_Rc*F_Cc)

#define NSAMPLES_WINDOW_CF_A (CF_Ra*CF_Ca)
#define NSAMPLES_WINDOW_CF_B (CF_Rb*CF_Cb)
#define NSAMPLES_WINDOW_CF_C (CF_Rc*CF_Cc)

#define NITERATIONS 2
