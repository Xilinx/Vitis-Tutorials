//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <adf.h>
#include <aie_api/aie.hpp>

#include "ssr_fft_fp_rotate.h"


// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

ssr_fft_fp_rotate::ssr_fft_fp_rotate( int (&twrom_i)[1536], int (&fft_phrot_buf_i)[2048] )
  : twrom( twrom_i ), fft_phrot_buf(fft_phrot_buf_i)
{
  idx = 0;
  aie::set_rounding(aie::rounding_mode::symmetric_inf);
  aie::set_saturation(aie::saturation_mode::saturate);
}

// ------------------------------------------------------------
// fp_elem_mul
// ------------------------------------------------------------

inline __attribute__((always_inline))
void ssr_fft_fp_rotate::fp_elem_mul (cfloat * restrict x1, cfloat * restrict x2,
                                     cfloat * restrict y1, cfloat * restrict y2)
{
  v4cfloat chess_storage(DM_bankB) * restrict pc1 = (v4cfloat chess_storage(DM_bankB) * restrict)  fft_phrot_buf;
  v4cfloat chess_storage(DM_bankB) * restrict pc2 = (v4cfloat chess_storage(DM_bankB) * restrict) (fft_phrot_buf+8);

  v4cfloat chess_storage(DM_bankA) * restrict px1 = (v4cfloat chess_storage(DM_bankA) * restrict) x1;
  v4cfloat chess_storage(DM_bankA) * restrict px2 = (v4cfloat chess_storage(DM_bankA) * restrict) x2;

  v4cfloat chess_storage(DM_bankC) * restrict py1 = (v4cfloat chess_storage(DM_bankC) * restrict) y1;
  v4cfloat chess_storage(DM_bankC) * restrict py2 = (v4cfloat chess_storage(DM_bankC) * restrict) y2;

  for (int j = 0; j < 512/4; ++j)
    chess_flatten_loop
      {
        *py1++ = fpmul(*px1++, *pc1); pc1+=2;
        *py2++ = fpmul(*px2++, *pc2); pc2+=2;
      }
}

// ------------------------------------------------------------
// fp_dds
// ------------------------------------------------------------

inline __attribute__((always_inline))
void ssr_fft_fp_rotate::fp_dds(int * rom)
{
  v4cfloat * restrict pc = (v4cfloat * restrict) rom;
  v4cfloat * restrict tw = (v4cfloat * restrict) fft_phrot_buf;
  v4cfloat coe;
  // fill up 0-63
  //{	// 0:7
  v4cfloat vx1 = *pc++;
  v4cfloat vx2 = *pc++;

  // x8, x16, x24, x32
  coe = *pc++;

  // output pointer
  v4cfloat * restrict py1 = tw;
  v4cfloat * restrict py2 = tw+1;

  // 0-7
  *py1 = vx1;
  *py2 = vx2;

  // 8-15
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 0, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 0, 0);

  // 16-23
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 1, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 1, 0);

  // 24-31
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 2, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 2, 0);

  // 32-39
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 3, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 3, 0);

  // x40, x48, x56, x64
  coe = *pc++;
  // 40-47
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 0, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 0, 0);

  // 48-55
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 1, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 1, 0);

  // 56-63
  py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 2, 0);
  py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 2, 0);

  // first 4 samples
  //py1+=2; *py1 = fpmul(vx1, 0, 0x3210, coe, 3, 0);  //py1 = 56
  //py2+=2; *py2 = fpmul(vx2, 0, 0x3210, coe, 3, 0);  //py2 = 60

  //}

  py1 += (64 -56)/4;
  py2 += (128-60)/4;
  v4cfloat * restrict py3 = tw+(192/4);


  //{
  //----------------------------------------------
  // 64,128,192 + (0:63)
  //----------------------------------------------

  // x64, x128, x192, x256
  //v4cfloat coe = *pc++;
  coe = *pc++;

  v4cfloat * restrict px =   tw;

  //v4cfloat * restrict py1 =  tw+(64/4);
  //v4cfloat * restrict py2 =  tw+(128/4);

  //64-255
  for(int k=0; k<(64/4); k++) chess_flatten_loop
                                {
                                  v4cfloat xx = *px++;
                                  *py1++ = fpmul(xx, 0, 0x3210, coe, 0, 0);   //64-128
                                  *py2++ = fpmul(xx, 0, 0x3210, coe, 1, 0);   //128-192
                                  *py3++ = fpmul(xx, 0, 0x3210, coe, 2, 0);   //192-256
                                }

  px -= 64/4;
  py1+= (256-128)/4;
  py2+= (512-192)/4;
  py3+= (768-256)/4;

  //----------------------------------------------
  // 256,512,768 + (1:256)
  //----------------------------------------------

  // x256, x512, x768
  coe = *pc;

  //v4cfloat * restrict px  = tw;
  //v4cfloat * restrict py1 = tw+(256/4);
  //v4cfloat * restrict py2 = tw+(512/4);
  //v4cfloat * restrict py3 = tw+(768/4);

  // 256-1023
  for(int k=0; k<(256/4); k++) chess_flatten_loop
                                 {
                                   v4cfloat xx = *px++;

                                   *py1++ = fpmul(xx, 0, 0x3210, coe, 0, 0);
                                   *py2++ = fpmul(xx, 0, 0x3210, coe, 1, 0);
                                   *py3++ = fpmul(xx, 0, 0x3210, coe, 2, 0);
                                 }

  //}
}

// ------------------------------------------------------------
// Run
// ------------------------------------------------------------

void ssr_fft_fp_rotate::run( input_buffer <int,extents<1024> >& __restrict x0,
                             input_buffer <int,extents<1024> >& __restrict x1,
                             output_buffer<int,extents<1024> >& __restrict y0,
                             output_buffer<int,extents<1024> >& __restrict y1 )
{
  fp_dds(twrom+idx);
  idx = (idx== (48*31))? 0 : (idx+48);
  fp_elem_mul((cfloat * restrict) x0.data(), (cfloat * restrict) x1.data(),
              (cfloat * restrict) y0.data(), (cfloat * restrict) y1.data());
}


