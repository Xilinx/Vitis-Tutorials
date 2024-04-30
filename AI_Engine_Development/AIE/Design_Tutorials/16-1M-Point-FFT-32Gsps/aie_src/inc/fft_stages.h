//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __FFT_STAGES_H__
#define __FFT_STAGES_H__

// -------------------------------------------------------------
// Stage 0: 1K Input from 2 input buffers
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage0_radix2_dit (cfloat * restrict px0,  cfloat * restrict px1, unsigned int n, cfloat * restrict py)
{
  // y0 = (x0 + x1)
  // y1 = (x0 - x1)
  //------------------------
  // output: 0/2/4/6/1/3/5/7
  //------------------------
  v4cfloat * restrict po1 = (v4cfloat * restrict) py;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (py + n/2);
  v4cfloat * restrict pi1 = (v4cfloat * restrict) px0;
  v4cfloat * restrict pi2 = (v4cfloat * restrict) (px0 + n/4);
  v4cfloat * restrict pi3 = (v4cfloat * restrict) px1;
  v4cfloat * restrict pi4 = (v4cfloat * restrict) (px1 + n/4);


  for (int j = 0; j < (n/2/8); ++j)
    chess_prepare_for_pipelining
      chess_loop_range(16,)
    {
      v4cfloat x0 = pi1[j];
      v4cfloat x1 = pi2[j];
      *po1++ = fpadd(x0, x1);
      *po2++ = fpsub(x0, x1);
      x0 = pi3[j];
      x1 = pi4[j];
      *po1++ = fpadd(x0, x1);
      *po2++ = fpsub(x0, x1);

    }
}


// ----------------------------------------------------------------------------
// Stage 1: 1K Input from 1 input buffer. twiddle is [1, -j]. No need for multiplication
// ----------------------------------------------------------------------------
inline __attribute__((always_inline)) void stage1_radix2_dit (cfloat * restrict px, unsigned int n, cfloat * restrict py0, cfloat * restrict py1)
{
  // x0 +   x1
  // x0 - j x1
  // x0 -   x1
  // x0 + j x1

  v8float * restrict x0 = (v8float * restrict) px;
  v8float * restrict x1 = (v8float * restrict) (px + n/4);
  v8float * restrict x2 = (v8float * restrict) (px + n/2);
  v8float * restrict x3 = (v8float * restrict) (px + n/4 + n/2);

  v8float * restrict y0 = (v8float * restrict) py0;
  v8float * restrict y1 = (v8float * restrict) py1;
  v8float * restrict y2 = (v8float * restrict) (py0 + n/4);
  v8float * restrict y3 = (v8float * restrict) (py1 + n/4);

  v8float coe = upd_elem(upd_elem(undef_v8float(), 0,  1), 1, -1);

  for(int i=0; i<(n/4/4); i++)
    chess_prepare_for_pipelining
      chess_loop_range(16,)
    {
      v8float xa = *x0++;
      v8float xb = *x1++;

      *y0++ = fpmac(xa, xb, 0, 0x76543210, coe, 0, 0);
      *y1++ = fpmac(xa, xb, 0, 0x76543210, coe, 1, 0);

    }


  for(int i=0; i<(n/4/4); i++)
    chess_prepare_for_pipelining
      chess_loop_range(16,)
    {
      v8float xc = *x2++;
      v8float xd = *x3++;

      *y2++ = fpmac(xc, xd, 0, 0x67452301, coe, 0, 0x10101010);
      *y3++ = fpmac(xc, xd, 0, 0x67452301, coe, 0, 0x01010101);
    }
}



// ---------------------------------------------------------------------------------
// Stage 2: 1K Input from 1 input buffer. twiddle is [1, (1-j)/sqrt(1/2), -j, (-1-j)/sqrt(2)]
// ---------------------------------------------------------------------------------
inline __attribute__((always_inline)) void stage2_radix2_dit (cfloat * restrict px0, cfloat * restrict px1, unsigned int n, cfloat * restrict py)
{
  // x0 +               x1
  // x0 + (1-j)/sqrt(2) x1
  // x0 -   j           x1
  // x0 - (1+j)/sqrt(2) x1
  // x0 -               x1
  // x0 - (1-j)/sqrt(2) x1
  // x0 +   j           x1
  // x0 + (1+j)/sqrt(2) x1

  v8float * restrict y0 = (v8float * restrict) py;
  v8float * restrict y1 = (v8float * restrict) (py + n/2);

  v8float coe = upd_elem( upd_elem(upd_elem(upd_elem(undef_v8float(), 0,  1), 1, -1), 2, 7.071068e-01), 3, -7.071068e-01);

  { 		// x0 + x1 | x0 - x1
    v8float * restrict x0 = (v8float * restrict) px0;
    v8float * restrict x1 = (v8float * restrict) (px0 + n/8);

    for(int i=0; i<(n/4/8); i++)
      chess_prepare_for_pipelining
        chess_loop_range(16,)
      {
        v8float xa = *x0++;
        v8float xb = *x1++;

        *y0++ = fpmac(xa, xb, 0, 0x76543210, coe, 0, 0);
        *y1++ = fpmac(xa, xb, 0, 0x76543210, coe, 1, 0);

      }

  }
  //------------------------------------


  { 		// x0 + (a - aj ) x1 = (x0_r + x1_r a + x1_i a)  + j (x0_i + x1_i a - x1_r a)
    // x0 - (a - aj ) x1 = (x0_r - x1_r a - x1_i a)  + j (x0_i - x1_i a + x1_r a)

    v8float * restrict x0 = (v8float * restrict) (px0 + n/4);
    v8float * restrict x1 = (v8float * restrict) (px0 + n/4 + n/8);

    for(int i=0; i<(n/4/8); i++)
      chess_prepare_for_pipelining
        chess_loop_range(16,)
      {
        v8float xa = *x0++;
        v8float xb = *x1++;

        v8float ya = fpmac(xa, xb, 0, 0x76543210, coe, 2, 0);
        v8float yb = fpmac(xa, xb, 0, 0x76543210, coe, 3, 0);

        *y0++ = fpmac(ya, xb, 0, 0x67452301, coe, 2, 0x10101010);
        *y1++ = fpmac(yb, xb, 0, 0x67452301, coe, 2, 0x01010101);

      }

  }


  { 		// x0 - j x1 | x0 + j x1
    v8float * restrict x0 = (v8float * restrict) px1;
    v8float * restrict x1 = (v8float * restrict) (px1 + n/8);


    for(int i=0; i<(n/4/8); i++)
      chess_prepare_for_pipelining
        chess_loop_range(16,)
      {
        v8float xa = *x0++;
        v8float xb = *x1++;

        *y0++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x10101010);
        *y1++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x01010101);
      }

  }


  { 		// x0 - (a + aj ) x1 = (x0_r - x1_r a + x1_i a)  + j (x0_i - x1_i a - x1_r a)
    // x0 + (a + aj ) x1 = (x0_r + x1_r a - x1_i a)  + j (x0_i + x1_i a + x1_r a)

    v8float * restrict x0 = (v8float * restrict) (px1 + n/4);
    v8float * restrict x1 = (v8float * restrict) (px1 + n/4 + n/8);

    for(int i=0; i<(n/4/8); i++)
      chess_prepare_for_pipelining
        chess_loop_range(16,)
      {
        v8float xa = *x0++;
        v8float xb = *x1++;

        v8float ya = fpmac(xa, xb, 0, 0x76543210, coe, 3, 0);
        v8float yb = fpmac(xa, xb, 0, 0x76543210, coe, 2, 0);

        *y0++ = fpmac(ya, xb, 0, 0x67452301, coe, 2, 0x10101010);
        *y1++ = fpmac(yb, xb, 0, 0x67452301, coe, 2, 0x01010101);

      }

  }
}


// -------------------------------------------------------------
// Stage 3: FFT FP Input, Interval<n/2, FP Output
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage3_radix2_dit ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  // y0 = (x0 + x1*tw)
  // y1 = (x0 - x1*tw)

  v8float * restrict po1 = (v8float * restrict) y;
  v8float * restrict po2 = (v8float * restrict) (y + n/2);

  v8float chess_storage(DM_bankA) * restrict px0 = (v8float chess_storage(DM_bankA) * restrict) x;
  v8float chess_storage(DM_bankA) * restrict px1 = (v8float chess_storage(DM_bankA) * restrict) (x+n/16);

  v8float * restrict pc =  ( v8float * restrict) tw;
  v8float coe = upd_elem(upd_elem(*pc++, 0, 1), 1, -1);

  { 		// x0 + x1 | x0 - x1
    for(int i=0; i<(n/4/16); i++) chess_flatten_loop
      //chess_prepare_for_pipelining
      //chess_loop_range(16,)
                                    {
                                      v8float xa = *px0++;
                                      v8float xb = *px1++;

                                      *po1++ = fpmac(xa, xb, 0, 0x76543210, coe, 0, 0);
                                      *po2++ = fpmac(xa, xb, 0, 0x76543210, coe, 1, 0);

                                    }

  }

  unsigned int coff = 0;

  for(int ite=1; ite<4; ite++)
    //chess_prepare_for_pipelining
    //chess_loop_range(3,)
    chess_flatten_loop
      {
        px0+=(n/4/16);
        px1+=(n/4/16);
        coff+=0x22222222;

        for(int i=0; i<(n/4/16); i++)  chess_flatten_loop
          //chess_prepare_for_pipelining
          //chess_loop_range(16,)
                                         {
                                           v4cfloat x0 = as_v4cfloat(*px0++);
                                           v4cfloat x1 = as_v4cfloat(*px1++);

                                           v4cfloat y1 = fpmac(x0, x1, 0,  0x76543210, coe, 0, coff);
                                           v4cfloat y2 = fpmsc(x0, x1, 0,  0x76543210, coe, 0, coff);

                                           *po1++ = as_v8float(fpmac_conf(y1, x1, 0, 0x67452301, coe, 1, coff, false, false, fpadd_mixadd, 0xff, fpcmp_nrm));
                                           *po2++ = as_v8float(fpmac_conf(y2, x1, 0, 0x67452301, coe, 1, coff, false, false, fpadd_mixsub, 0xff, fpcmp_nrm));

                                         }

      }

  px0+=(n/4/16);
  px1+=(n/4/16);

  { 		// x0 -j x1 | x0 + j x1
    for(int i=0; i<(n/4/16); i++) chess_flatten_loop
      //chess_prepare_for_pipelining
      //chess_loop_range(16,)
                                    {
                                      v8float xa = *px0++;
                                      v8float xb = *px1++;

                                      *po1++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x10101010);
                                      *po2++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x01010101);

                                    }

  }

  coe = *pc;
  coff = 0;

  for(int ite=1; ite<4; ite++) chess_flatten_loop
    //chess_prepare_for_pipelining
    //chess_loop_range(3,)
                                 {
                                   px0+=(n/4/16);
                                   px1+=(n/4/16);
                                   coff+=0x22222222;

                                   for(int i=0; i<(n/4/16); i++) chess_flatten_loop
                                     //chess_prepare_for_pipelining
                                     //chess_loop_range(16,)
                                                                   {
                                                                     v4cfloat x0 = as_v4cfloat(*px0++);
                                                                     v4cfloat x1 = as_v4cfloat(*px1++);

                                                                     v4cfloat y1 = fpmac(x0, x1, 0,  0x76543210, coe, 0, coff);
                                                                     v4cfloat y2 = fpmsc(x0, x1, 0,  0x76543210, coe, 0, coff);

                                                                     *po1++ = as_v8float(fpmac_conf(y1, x1, 0, 0x67452301, coe, 1, coff, false, false, fpadd_mixadd, 0xff, fpcmp_nrm));
                                                                     *po2++ = as_v8float(fpmac_conf(y2, x1, 0, 0x67452301, coe, 1, coff, false, false, fpadd_mixsub, 0xff, fpcmp_nrm));
                                                                   }

                                 }



}



// -------------------------------------------------------------
// Stage 4: FFT FP Input, Interval<n/2, FP Output
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage4_radix2_dit ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  // y0 = (x0 + x1*tw)
  // y1 = (x0 - x1*tw)

  v8float * restrict po1 = (v8float * restrict) y;
  v8float * restrict po2 = (v8float * restrict) (y + n/2);

  const int blksz  = (n/32);
  const int blkstp = (blksz/4);

  v8float chess_storage(DM_bankA) * restrict px0 = (v8float chess_storage(DM_bankA) * restrict) x;
  v8float chess_storage(DM_bankA) * restrict px1 = (v8float chess_storage(DM_bankA) * restrict) (x+blksz);

  cfloat chess_storage(DM_bankB) * restrict pc =  (cfloat chess_storage(DM_bankB) * restrict) (tw+1);



	{ 		// x0 + x1 | x0 - x1
		v8float coe = upd_elem(upd_elem(undef_v8float(), 0, 1), 1, -1);

		for(int i=0; i<blkstp; i++) chess_flatten_loop
			//chess_prepare_for_pipelining
			//chess_loop_range(16,)
		{
			v8float xa = *px0++;
			v8float xb = *px1++;

			*po1++ = fpmac(xa, xb, 0, 0x76543210, coe, 0, 0);
			*po2++ = fpmac(xa, xb, 0, 0x76543210, coe, 1, 0);

		}

	}


	for(int ite=1; ite<8; ite++)
		//chess_prepare_for_pipelining
	   //chess_loop_range(3,)
		chess_flatten_loop
	{
		px0+=blkstp;
		px1+=blkstp;

		v8float coe = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc++));

		for(int i=0; i<blkstp; i++)  chess_flatten_loop
			//chess_prepare_for_pipelining
			//chess_loop_range(4,)
		{
			v8float x0 = *px0++;
			v8float x1 = *px1++;

			v8float y1 = fpmac(x0, x1, 0,  0x76543210, coe, 0, 0);
			v8float y2 = fpmsc(x0, x1, 0,  0x76543210, coe, 0, 0);

			*po1++ = fpmac_conf(y1, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixadd, 0xff, fpcmp_nrm);
			*po2++ = fpmac_conf(y2, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixsub, 0xff, fpcmp_nrm);

		}

	}

	px0+=blkstp; //blksz;
	px1+=blkstp; //blksz;

 	{

		v8float coe = upd_elem(upd_elem(undef_v8float(), 0, 1), 1, -1);

 		// x0 -j x1 | x0 + j x1
		for(int i=0; i<blkstp; i++)
			chess_flatten_loop
		{
			v8float xa = *px0++;
			v8float xb = *px1++;

			*po1++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x10101010);
			*po2++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x01010101);

		}

	}


	pc++;

	for(int ite=1; ite<8; ite++) chess_flatten_loop
	{
		px0+=blkstp;
		px1+=blkstp;

		v8float coe = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc++));

		for(int i=0; i<blkstp; i++) chess_flatten_loop
			//chess_prepare_for_pipelining
			//chess_loop_range(4,)
		{
			v4cfloat x0 = as_v4cfloat(*px0++);
			v4cfloat x1 = as_v4cfloat(*px1++);

			v4cfloat y1 = fpmac(x0, x1, 0,  0x76543210, coe, 0, 0);
			v4cfloat y2 = fpmsc(x0, x1, 0,  0x76543210, coe, 0, 0);

			*po1++ = as_v8float(fpmac_conf(y1, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixadd, 0xff, fpcmp_nrm));
			*po2++ = as_v8float(fpmac_conf(y2, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixsub, 0xff, fpcmp_nrm));
		}

	}



}


#define ONEITE(idx,p0,p1,o1,o2)     {p0+=blkinc;p1+=blkinc;\
									for(int i=0; i<4; i++)  chess_flatten_loop \
									{	v8float x0 = *p0++;	v8float x1 = *p1++; \
										v8float y1 = fpmac(x0, x1, 0,  0x76543210, coe, idx*2+0, 0); \
										v8float y2 = fpmsc(x0, x1, 0,  0x76543210, coe, idx*2+0, 0); \
										*o1++ = fpmac_conf(y1, x1, 0, 0x67452301, coe, idx*2+1, 0, false, false, fpadd_mixadd, 0xff, fpcmp_nrm); \
										*o2++ = fpmac_conf(y2, x1, 0, 0x67452301, coe, idx*2+1, 0, false, false, fpadd_mixsub, 0xff, fpcmp_nrm); \
									}}

// -------------------------------------------------------------
// Stage 5: Radix 2 FFT FP Input, Interval<n/2, FP Output
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage5_radix2_dit ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  // y0 = (x0 + x1*tw)
  // y1 = (x0 - x1*tw)

  v8float * restrict po1 = (v8float * restrict) y;
  v8float * restrict po2 = (v8float * restrict) (y + n/2);
  v8float * restrict po3 = (v8float * restrict) (y + n/4);
  v8float * restrict po4 = (v8float * restrict) (y + n/2 + n/4);


  const int blksz  = (n/64);
  const int blkstp = (blksz/4);
  const int blkinc = blkstp;

  v8float chess_storage(DM_bankA) * restrict px0 = (v8float chess_storage(DM_bankA) * restrict) x;
  v8float chess_storage(DM_bankA) * restrict px1 = (v8float chess_storage(DM_bankA) * restrict) (x+blksz);
  v8float chess_storage(DM_bankA) * restrict px2 = (v8float chess_storage(DM_bankA) * restrict) (x+n/2);
  v8float chess_storage(DM_bankA) * restrict px3 = (v8float chess_storage(DM_bankA) * restrict) (x+n/2+blksz);

  v8float chess_storage(DM_bankB) * restrict pc0 =  (v8float chess_storage(DM_bankB) * restrict) tw;
  v8float chess_storage(DM_bankB) * restrict pc1 =  (v8float chess_storage(DM_bankB) * restrict) (tw+16);


	// x0 + x1 | x0 - x1
	{
		v8float coe = as_v8float(upd_elem(undef_v4cfloat(), 0, {1,-1}));

		for(int i=0; i<blkstp; i++) chess_flatten_loop
		{
			v8float xa = *px0++;
			v8float xb = *px1++;

			*po1++ = fpmac(xa, xb, 0, 0x76543210, coe, 0, 0);
			*po2++ = fpmac(xa, xb, 0, 0x76543210, coe, 1, 0);
		}

	}


	{

		v8float coe = *pc0++;
		ONEITE(1,px0,px1,po1,po2)
		ONEITE(2,px0,px1,po1,po2)
		ONEITE(3,px0,px1,po1,po2)

		coe = *pc0++;
		ONEITE(0,px0,px1,po1,po2)
		ONEITE(1,px0,px1,po1,po2)
		ONEITE(2,px0,px1,po1,po2)
		ONEITE(3,px0,px1,po1,po2)

		coe = *pc0++;
		ONEITE(0,px0,px1,po1,po2)
		ONEITE(1,px0,px1,po1,po2)
		ONEITE(2,px0,px1,po1,po2)
		ONEITE(3,px0,px1,po1,po2)

		coe = *pc0;
		ONEITE(0,px0,px1,po1,po2)
		ONEITE(1,px0,px1,po1,po2)
		ONEITE(2,px0,px1,po1,po2)
		ONEITE(3,px0,px1,po1,po2)
	}

	{
		v8float coe = as_v8float(upd_elem(undef_v4cfloat(), 0, {1,-1}));

		for(int i=0; i<blkstp; i++) chess_flatten_loop
		{
			v8float xa = *px2++;
			v8float xb = *px3++;

			*po3++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x10101010);
			*po4++ = fpmac(xa, xb, 0, 0x67452301, coe, 0, 0x01010101);
		}

	}


	{

		v8float coe = *pc1++;
		ONEITE(1,px2,px3,po3,po4)
		ONEITE(2,px2,px3,po3,po4)
		ONEITE(3,px2,px3,po3,po4)

		coe = *pc1++;
		ONEITE(0,px2,px3,po3,po4)
		ONEITE(1,px2,px3,po3,po4)
		ONEITE(2,px2,px3,po3,po4)
		ONEITE(3,px2,px3,po3,po4)

		coe = *pc1++;
		ONEITE(0,px2,px3,po3,po4)
		ONEITE(1,px2,px3,po3,po4)
		ONEITE(2,px2,px3,po3,po4)
		ONEITE(3,px2,px3,po3,po4)

		coe = *pc1;
		ONEITE(0,px2,px3,po3,po4)
		ONEITE(1,px2,px3,po3,po4)
		ONEITE(2,px2,px3,po3,po4)
		ONEITE(3,px2,px3,po3,po4)

	}


}

#undef ONEITE


#define ONEITE(coe,p0,p1)      coe = as_v8float(upd_elem(undef_v4cfloat(), 0, *pc++)); \
							  for(int i=0; i<blkstp; i++)  chess_flatten_loop \
							  {	v8float x0 = *p0++;	v8float x1 = *p1++; \
							  	v8float y1 = fpmac(x0, x1, 0,  0x76543210, coe, 0, 0); \
							  	v8float y2 = fpmsc(x0, x1, 0,  0x76543210, coe, 0, 0); \
							  	*po1++ = fpmac_conf(y1, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixadd, 0xff, fpcmp_nrm); \
							  	*po2++ = fpmac_conf(y2, x1, 0, 0x67452301, coe, 1, 0, false, false, fpadd_mixsub, 0xff, fpcmp_nrm);}



// -------------------------------------------------------------
// Stage 6: Radix 2 FFT FP Input, Interval<n/2, FP Output
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage6_radix2_dit ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  // y0 = (x0 + x1*tw)
  // y1 = (x0 - x1*tw)

  v8float * restrict po1 = (v8float * restrict) y;
  v8float * restrict po2 = (v8float * restrict) (y + n/2);

  const int blksz   = (n/128);
  const int blkstp  = (blksz/4);
  const int blkinc = (blkstp*3);
  const int loopcnt = 16;

  v8float chess_storage(DM_bankA) * restrict px0 = (v8float chess_storage(DM_bankA) * restrict) x;
  v8float chess_storage(DM_bankA) * restrict px1 = (v8float chess_storage(DM_bankA) * restrict) (x+blksz);
  v8float chess_storage(DM_bankA) * restrict px2 = (v8float chess_storage(DM_bankA) * restrict) (x+blksz*2);
  v8float chess_storage(DM_bankA) * restrict px3 = (v8float chess_storage(DM_bankA) * restrict) (x+blksz*3);


  cfloat chess_storage(DM_bankB) * restrict pc =  (cfloat chess_storage(DM_bankB) * restrict) (tw+1);

  v8float coe0;
  v8float x0, x1, y1, y2;


	coe0 = as_v8float(upd_elem(undef_v4cfloat(), 0, {1,-1}));

	for(int i=0; i<blkstp; i++) chess_flatten_loop
	{
		v8float xa = *px0++;
		v8float xb = *px1++;
		*po1++ = fpmac(xa, xb, 0, 0x76543210, coe0, 0, 0);
		*po2++ = fpmac(xa, xb, 0, 0x76543210, coe0, 1, 0);
	}


	ONEITE(coe0,px2,px3);



	for(int ite=1; ite<loopcnt; ite++)
		chess_flatten_loop
	{
		px0+=blkinc; px1+=blkinc;
		ONEITE(coe0,px0,px1);

		px2+=blkinc; px3+=blkinc;
		ONEITE(coe0,px2,px3);
	}


	px0+=blkinc; //blksz;
	px1+=blkinc; //blksz;


 	coe0 = as_v8float(upd_elem(undef_v4cfloat(), 0, {1,-1}));

 	// x0 -j x1 | x0 + j x1
	for(int i=0; i<blkstp; i++)	chess_flatten_loop
	{
		v8float xa = *px0++;
		v8float xb = *px1++;

		*po1++ = fpmac(xa, xb, 0, 0x67452301, coe0, 0, 0x10101010);
		*po2++ = fpmac(xa, xb, 0, 0x67452301, coe0, 0, 0x01010101);

	}

	pc++;
	px2+=blkinc; //blksz;
	px3+=blkinc; //blksz;
	ONEITE(coe0,px2,px3);


	for(int ite=1; ite<loopcnt; ite++) chess_flatten_loop
	{
		px0+=blkinc; px1+=blkinc;
		ONEITE(coe0,px0,px1);
		px2+=blkinc; px3+=blkinc;
		ONEITE(coe0,px2,px3);
	}



}


#undef ONEITE




// -------------------------------------------------------------
// Stage 7 Interleaving
// input:  0 2 4 6 1 3 5 7, 8 a c e 9 b d f
// output: 0 8 2 a 1 9 3 b <->  4 c 6 e 5 d 7 f
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage7_radix2_dit_a ( cfloat * restrict x,  const int n, cfloat * restrict y)
{
  v4cfloat * restrict po1 = (v4cfloat * restrict) y;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (y + n/2);

  v4cfloat * restrict px = (v4cfloat chess_storage(DM_bankA) * restrict) x;

  for (int i =0; i < (n/16); ++i)
	chess_prepare_for_pipelining
    chess_loop_range(16,)
	{

		v8cfloat x0 = xset_w(    0, *px);  px+=2;
				 x0 = upd_w(x0,  1, *px);  px-=1; // 0 2 4 6 8 a c e

		v8cfloat x1 = fpshuffle8(x0, 0, 0x73625140);

		*po1++ = ext_w(x1, 0);
		*po2++ = ext_w(x1, 1);

		x0 = xset_w(    0, *px);  px+=2;
		x0 = upd_w(x0,  1, *px);  px+=1; // 1 3 5 7 9 b d f

		x1 = fpshuffle8(x0, 0, 0x73625140);

		*po1++ = ext_w(x1, 0);
		*po2++ = ext_w(x1, 1);


	}

}

inline __attribute__((always_inline)) void stage7_radix2_dit_b ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  v4cfloat * restrict po1 = (v4cfloat * restrict) y;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (y + n/2);

  v4cfloat chess_storage(DM_bankA) * restrict px0 = (v4cfloat chess_storage(DM_bankA) * restrict) x;
  v4cfloat chess_storage(DM_bankA) * restrict px1 = (v4cfloat chess_storage(DM_bankA) * restrict) (x + n/2);

  v2cfloat chess_storage(DM_bankB) * restrict pc = (v2cfloat chess_storage(DM_bankB) * restrict) tw;

	v4cfloat x0 = *px0++;
	v8cfloat x1 = xset_w(0, *px1++);


  for (int i =0; i < (n/16); ++i)
	chess_prepare_for_pipelining
    chess_loop_range(16,)
	{

		v4cfloat coe = upd_v(undef_v4cfloat(), 0, *pc++);

		*po1++ = fpmac(x0, x1, 0, 0x3210, coe, 0, 0x1010); v4cfloat x2 = *px0++;
		*po2++ = fpmsc(x0, x1, 0, 0x3210, coe, 0, 0x1010); x1 = upd_w(x1, 1, *px1++);
		*po1++ = fpmac(x2, x1, 4, 0x3210, coe, 0, 0x1010); x0 = *px0++;
		*po2++ = fpmsc(x2, x1, 4, 0x3210, coe, 0, 0x1010); x1 = upd_w(x1, 0, *px1++);


	}

}
//-----------------------------------------------------------------------------------------------



// -------------------------------------------------------------
// Stage 8 Interleaving
// input:  0 8 2 a 1 9 3 b  0 8 2 a 1 9 3 b
// output: 0 8 0 8  1 9 1 9 <->  2 a 2 a 3 b 3 b
// -------------------------------------------------------------
inline __attribute__((always_inline)) void stage8_radix2_dit_a ( cfloat * restrict x,  const int n, cfloat * restrict y)
{
  v4cfloat * restrict po1 = (v4cfloat * restrict) y;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (y + n/2);

  v4cfloat * restrict px = (v4cfloat chess_storage(DM_bankA) * restrict) x;

  for (int i =0; i < (n/16); ++i)
	chess_prepare_for_pipelining
    chess_loop_range(16,)
	{

		v8cfloat x0 = xset_w(    0, *px);  px+=2;
				 x0 = upd_w(x0,  1, *px);  px-=1; //  0 8 2 a  0 8 2 a

		v8cfloat x1 = fpshuffle8(x0, 0, 0x76325410);

		*po1++ = ext_w(x1, 0);
		*po2++ = ext_w(x1, 1);

		x0 = xset_w(    0, *px);  px+=2;
		x0 = upd_w(x0,  1, *px);  px+=1; // 1 9 3 b 1 9 3 b

		x1 = fpshuffle8(x0, 0, 0x76325410);

		*po1++ = ext_w(x1, 0);
		*po2++ = ext_w(x1, 1);


	}

}


inline __attribute__((always_inline)) void stage8_radix2_dit_b ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  v4cfloat * restrict po1 = (v4cfloat * restrict) y;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (y + n/2);

  v4cfloat chess_storage(DM_bankA) * restrict px0 = (v4cfloat chess_storage(DM_bankA) * restrict) x;
  v4cfloat chess_storage(DM_bankA) * restrict px1 = (v4cfloat chess_storage(DM_bankA) * restrict) (x + n/2);

  v4cfloat chess_storage(DM_bankB) * restrict pc = (v4cfloat chess_storage(DM_bankB) * restrict) tw;

	v4cfloat x0 = *px0++;
	v8cfloat x1 = xset_w(0, *px1++);


  for (int i =0; i < (n/16); ++i)
	chess_prepare_for_pipelining
    chess_loop_range(16,)
	{

		v4cfloat coe = *pc++;

		*po1++ = fpmac(x0, x1, 0, 0x3210, coe, 0, 0x3210); v4cfloat x2 = *px0++;
		*po2++ = fpmsc(x0, x1, 0, 0x3210, coe, 0, 0x3210); x1 = upd_w(x1, 1, *px1++);
		*po1++ = fpmac(x2, x1, 4, 0x3210, coe, 0, 0x3210); x0 = *px0++;
		*po2++ = fpmsc(x2, x1, 4, 0x3210, coe, 0, 0x3210); x1 = upd_w(x1, 0, *px1++);

	}

}


inline __attribute__((always_inline)) void stage9_radix2_dit ( cfloat * restrict x,  cfloat * restrict tw, const int n, cfloat * restrict y)
{
  v4cfloat * restrict po1 = (v4cfloat * restrict) y;
  v4cfloat * restrict po2 = (v4cfloat * restrict) (y + n/2);

  v4cfloat chess_storage(DM_bankA) * restrict px0 = (v4cfloat chess_storage(DM_bankA) * restrict) x;
  v4cfloat chess_storage(DM_bankA) * restrict px1 = (v4cfloat chess_storage(DM_bankA) * restrict) (x+4);


  v4cfloat chess_storage(DM_bankB) * restrict pc = (v4cfloat chess_storage(DM_bankB) * restrict) tw;


  for (int i =0; i < (n/8); ++i)
	chess_prepare_for_pipelining
    chess_loop_range(16,)
	{

		v4cfloat x0 = *px0; px0+=4;
		v8cfloat x1 = xset_w(0, *px1); px1+=4;

		*po1++ = fpmac(x0, x1, 0, 0x3210, *pc, 0, 0x3210);
		*po2++ = fpmsc(x0, x1, 0, 0x3210, *pc, 0, 0x3210); pc+=2;

	}

}







#endif // __FFT_STAGES_H__

