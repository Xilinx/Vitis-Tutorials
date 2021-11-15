/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <adf.h>

// needs chess storage on coeffs in testbench
void fir24_sym(input_window_cint16 *iwin, output_window_cint16 *owin,  const  int32  (&coeffs)[12], int32 (&coeffs_readback)[12]) { 

  v16cint16 xbuff;
  v16cint16 ybuff;

  v8cint16 wtmp;
  v4cint16 vtmp;
  window_readincr(iwin, wtmp);
  xbuff = upd_w(xbuff, 0, wtmp); // D0  D1  D2  D3  | D4  D5  D6  D7 |  x x x x | x x x x
  window_readincr(iwin, wtmp);
  xbuff = upd_w(xbuff, 1, wtmp); // D0  D1  D2  D3  | D4  D5  D6  D7 |  D8 D9 D10 D11 | D12 D13 D14 D15
  ybuff = upd_v(ybuff, 0, ext_v(wtmp,1));
  window_readincr(iwin, vtmp);
  ybuff = upd_v(ybuff, 1, vtmp);
  window_readincr(iwin, wtmp);
  ybuff = upd_w(ybuff, 1, wtmp );

  /*
     D0  D1  D2  D3  | D4  D5  D6  D7 |  D8  D9  D10 D11 | D12 D13 D14 D15
     D12 D13 D14 D15 | D16 D17 D18 D19 | D20 D21 D22 D23 | D24 D25 D26 D27
     */



  v16int16 zbuff ; 

  for (unsigned i = 0; i < 12 ; i++) { 
    zbuff = shft_elem(zbuff, (int16) coeffs[11 - i]);
    coeffs_readback[11 - i]=coeffs[11 - i];
  }

  const unsigned BLOCKSIZE = 64;

  // every loop consumes 16 input samples and produces 16 output samples
  for (unsigned i = 0 ; i < BLOCKSIZE >> 4 ; i++) { 

    v4cacc48 acc;
    v4cint16 isamples;
    v4cint16 osamples;
    acc = mul4_sym(xbuff, 12, 0x7654, 1, ybuff, 7, zbuff, 0, 0x0000, 1 ) ;
    acc = mac4_sym(acc, xbuff, 0, 0x7654, 1, ybuff, 3, zbuff, 4, 0x0000, 1) ;
    acc = mac4_sym(acc,  xbuff, 4, 0x7654, 1, ybuff, 15, zbuff, 8, 0x0000, 1 ) ;

    osamples = srs(acc, 9);

    window_readincr(iwin, isamples);
    window_writeincr(owin, osamples);

    // copy values 16,17,18,19 from b to a             // D16 D17 D18 D19  | D4  D5  D6  D7 |  D8 D9 D10 D11 | D12 D13 D14 D15
    xbuff = upd_v(xbuff, 0, ext_v(ybuff,1));
    // update 4 new values (28,29,30,31) into ybuff    // D28 D29 D30 D31  | D16 D17 D18 D19 | D20 D21 D22 D23 | D24 D25 D26 D27
    ybuff = upd_v(ybuff, 0, isamples);


    acc = mul4_sym(xbuff, 0, 0x7654, 1, ybuff, 11, zbuff, 0, 0x0000, 1 ) ;
    acc = mac4_sym(acc, xbuff, 4, 0x7654, 1, ybuff, 7, zbuff, 4, 0x0000, 1) ;
    acc = mac4_sym(acc,  xbuff, 8, 0x7654, 1, ybuff, 3, zbuff, 8, 0x0000, 1 ) ;

    osamples = srs(acc,9);

    window_readincr(iwin, isamples);
    window_writeincr(owin,osamples);
    // copy values 20,21,22,23 from b to a            // D16 D17 D18 D19  | D20 D21 D22 D23 |  D8 D9 D10 D11 | D12 D13 D14 D15

    xbuff = upd_v(xbuff, 1, ext_v(ybuff,2));
    // update 4 new values (28,29,30,31) into ybuff   // D28 D29 D30 D31  | D32 D33 D34 D35 | D20 D21 D22 D23 | D24 D25 D26 D27
    ybuff = upd_v(ybuff, 1, isamples);

    acc = mul4_sym(xbuff, 4, 0x7654, 1, ybuff, 15, zbuff, 0, 0x0000, 1 ) ;
    acc = mac4_sym(acc, xbuff, 8, 0x7654, 1, ybuff, 11, zbuff, 4, 0x0000, 1) ;
    acc = mac4_sym(acc,  xbuff, 12, 0x7654, 1, ybuff, 7, zbuff, 8, 0x0000, 1 ) ;

    osamples = srs(acc,9);

    window_readincr(iwin, isamples);
    window_writeincr(owin, osamples);                

    // copy values 24,25,26,27 from b to a           // D16 D17 D18 D19  | D20 D21 D22 D23 |  D24 D25 D26 D27 | D12 D13 D14 D15

    xbuff = upd_v(xbuff, 2, ext_v(ybuff,3));
    // update 4 new values (40,41,42,32)             // D28 D29 D30 D31  | D32 D33 D34 D35 |  D36 D27 D38 D39 | D24 D25 D26 D27
    ybuff = upd_v(ybuff, 2, isamples);

    acc = mul4_sym(xbuff, 8, 0x7654, 1, ybuff, 3, zbuff, 0, 0x0000, 1 ) ;
    acc = mac4_sym(acc, xbuff, 12, 0x7654, 1, ybuff, 15, zbuff, 4, 0x0000, 1) ;
    acc = mac4_sym(acc,  xbuff, 0, 0x7654, 1, ybuff, 11, zbuff, 8, 0x0000, 1 ) ;

    osamples = srs(acc, 9);

    window_readincr(iwin, isamples);
    window_writeincr(owin, osamples);  

    // copy values 28, 29, 30, 31 from b to a           // D16 D17 D18 D19  | D20 D21 D22 D23 |  D24 D25 D26 D27 | D28 D29 D30 D31

    xbuff = upd_v(xbuff, 3, ext_v(ybuff,0));
    // update 4 new values (40,41,42,32)             // D28 D29 D30 D31  | D32 D33 D34 D35 |  D36 D27 D38 D39 | D40 D41 D42 D43
    ybuff = upd_v(ybuff, 3, isamples);
  };

};



