//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#pragma once
#include <hls_stream.h>
#include <ap_fixed.h>

template <typename TT_DATA, unsigned DEPTH>
class image_buffer_template {
public:
  typedef hls::stream<TT_DATA> TT_STREAM;

  image_buffer_template(void) { }

  void run( TT_DATA mem[DEPTH], unsigned NPULSE_USE, TT_STREAM& buff_to_aie, TT_STREAM& aie_to_buff )
  {
    bool done_rd=0, done_wr=0;
    unsigned pp_r=0, pp_w=0;
    unsigned dd_r=0,dd_w=0;
    static bool done=0;
    static TT_DATA  buff[DEPTH];
#pragma HLS bind_storage variable=buff type=RAM_T2P latency=1 impl=uram
#pragma HLS dependence variable=buff type=intra false

    // Process all radar pulses, sending image to AIE and capturing returned image:
    done = 0;                   // Support re-entrant for multiple frames
  SEND_PULSES: while (!done) {
#pragma HLS PIPELINE II=1
      // Write side of buffer:
      TT_DATA val_o = ( pp_w == 0 ) ? TT_DATA(0) : buff[dd_w]; // Sends zeros on initial pulse
      if ( done_wr == 0 && buff_to_aie.full() == 0 ) {
        buff_to_aie.write( val_o );
        if ( dd_w == DEPTH-1 ) {
          done_wr = ( pp_w == NPULSE_USE-1 );
          pp_w++;
          dd_w = 0;
        }
        else dd_w++;
      }
      // Read side of buffer:
      if ( done_rd == 0 && aie_to_buff.empty() == 0 ) { // Avoid blocking if empty
        buff[dd_r] = aie_to_buff.read();
        if ( dd_r == DEPTH-1 ) {
          done_rd = ( pp_r == NPULSE_USE-1 );
          pp_r++;
          dd_r = 0;
        }
        else dd_r++;
      }
      done = (done_rd==1) && (done_wr==1);
    }
    // Upload final image to LPDDR:
  UPLOAD_IMAGE: for (unsigned dd=0; dd < DEPTH; dd++) {
#pragma HLS PIPELINE II=1
      mem[dd] = buff[dd];
    } // dd
  }
};
