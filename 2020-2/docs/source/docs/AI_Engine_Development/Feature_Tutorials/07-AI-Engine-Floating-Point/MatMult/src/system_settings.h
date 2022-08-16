/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
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

#define NBYTES_FLOAT 4
#define NBYTES_CFLOAT 8
