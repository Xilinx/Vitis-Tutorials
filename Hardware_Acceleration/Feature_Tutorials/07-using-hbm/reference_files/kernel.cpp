/*
#include "kernel.h"
#iopyright 2021 Xilinx, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include "kernel.h"
#include <ap_int.h>
#include <stdio.h>


// TRIPCOUNT identifier
const unsigned int c_total = 1024/VDATA_SIZE;


// 32 bits LFSR with taps at 32,22,2,1, as per table 3 of XAPP 052
// https://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
unsigned int next_lfsr32(ap_uint<32> lfsr_in) {

  bool b_32 = lfsr_in.get_bit(32 - 32);
  bool b_22 = lfsr_in.get_bit(32 - 22);
  bool b_2 = lfsr_in.get_bit(32 - 2);
  bool b_1 = lfsr_in.get_bit(32 - 1);
  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
  ap_uint<32> lfsr_out = lfsr_in >> 1;
  lfsr_out.set_bit(31, new_bit);

  return lfsr_out.to_uint();
}


/*
 * generates a "all 1's" mask that is strictly smaller than the input value,
 * e.g. :
 * gen_clip_mask(126) -> 63
 * gen_clip_mask(127) -> 63
 * gen_clip_mask(128) -> 127
 * gen_clip_mask(129) -> 127
 *
 * we will use this mask to clear the higher bits of the PRNG so that what
 * is left is of the same magnitude of the size, but smaller.
 * this clipped random number is then added to the previous index
*/
unsigned int gen_clip_mask(unsigned int x) {
    unsigned int mask=0xffffffff;
gmask:
    do {
        #pragma HLS LOOP_TRIPCOUNT min = 1 max = 32
        mask=mask>>1;
    } while(mask>=x);
    return mask;
}

unsigned int gen_align_address_mask(unsigned int bl) {
    if (     bl==1  ) return (0xffffffff); // no alignment enforced
    else if (bl==2  ) return (0xfffffffe); // align every 2 words
    else if (bl==4  ) return (0xfffffffc); // align every 4 words
    else if (bl==8  ) return (0xfffffff8); // ... 8 words
    else if (bl==16 ) return (0xfffffff0); // ... 16 words
    else if (bl==32 ) return (0xffffffe0); // ... 32 words
    else              return (0xffffff00); // 64 words
}

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        dsize  (input)     --> Size of Vector in Integer
*/
extern "C"
void vadd(
        const v_dt *in1,               // Read-Only Vector 1
        const v_dt *in2,               // Read-Only Vector 2
        v_dt *out,                     // Output Result for ADD
        const unsigned int dsize,       // Size in integer
        const unsigned int kernel_loop,  // Running the same kernel operations kernel_loop times
        bool addRandom                 // Address Pattern is random
        ) {
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 latency = 300 num_read_outstanding = 128 num_write_outstanding=1
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 latency = 300 num_read_outstanding = 128 num_write_outstanding=1
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 latency = 50  num_write_outstanding= 128 num_read_outstanding=1

    // dsize is a multiple of 16, even 1024, VDATA_SIZE is power of 2
    unsigned int vSize = dsize / VDATA_SIZE;
    unsigned int in_index=0;
    unsigned int bl = BLENGTH;
    v_dt tmpOutAdd;

    // we separate the code between random access and sequential access
    // because the compile is able to infer burst in the sequential case
    // but if we merge the 2 codes, the burst is not inferred 
    if(addRandom==false) {
        // sequential access
    SeqLoop:
        for (unsigned int count = 0; count < kernel_loop; count++) {
            // Running same kernel operation kernel_loop to keep the kernel busy for HBM
            // bandwidth testing
            #pragma HLS LOOP_TRIPCOUNT min = c_total max = c_total
            //Per iteration of this loop perform vSize vector addition
    _vSizeLoop1:
            for (unsigned int i = 0; i < vSize/bl; i++) {
		#pragma HLS PIPELINE 
                #pragma HLS LOOP_TRIPCOUNT min = VDATA_SIZE max = VDATA_SIZE
                in_index = i;

    _blLoop1:
                for (unsigned int ii = 0; ii < bl; ii++) {
		   //#pragma HLS UNROLL
                   v_dt tmpIn1 = in1[in_index*bl+ii];
                   v_dt tmpIn2 = in2[in_index*bl+ii];
   
    _vecLoop1: // will be unrolled because parent loop is pipelined
                   for (int k = 0; k < VDATA_SIZE; k++) {
                       tmpOutAdd.data[k] = tmpIn1.data[k] + tmpIn2.data[k];
                   }

                   out[in_index*bl+ii] = tmpOutAdd;
		}
            }
        }
    } else {
        // random access
        unsigned int randvalue=0x12345678; // initialize to non-zero value!
	unsigned int index_clip_mask = gen_clip_mask(vSize);
        unsigned int index_align_mask = gen_align_address_mask(BLENGTH);
    RandLoop:
        for (unsigned int count = 0; count < kernel_loop; count++) {
            #pragma HLS LOOP_TRIPCOUNT min = c_total max = c_total
            //Per iteration of this loop perform vSize vector addition
    _vSizeLoop2:
            for (unsigned int i = 0; i < vSize/bl; i++) {
		#pragma HLS PIPELINE 
                #pragma HLS LOOP_TRIPCOUNT min = VDATA_SIZE max = VDATA_SIZE
                randvalue = next_lfsr32(randvalue); // 32 bits
                // index_clip_mask is less than vSize by construct, 
                // so randvalueclipped will be less than vSize too
                unsigned int randvalueclipped = randvalue & index_clip_mask;
                unsigned int candidate = in_index + randvalueclipped; // less than 2x vSize
                // need to make sure it is strictly less than vSize
                // note: don't need a true modulo operator because of the above masking and derived constraints
                unsigned int in_index_random = candidate < vSize ? candidate : candidate-vSize;
                in_index = in_index_random & index_align_mask;

    _blLoop2:
                for (unsigned int ii = 0; ii < bl; ii++) {
		   //#pragma HLS UNROLL
                   v_dt tmpIn1 = in1[in_index+ii];
                   v_dt tmpIn2 = in2[in_index+ii];

    _vecLoop2: // will be unrolled because parent loop is pipelined
                   for (int k = 0; k < VDATA_SIZE; k++) {
                       tmpOutAdd.data[k] = tmpIn1.data[k] + tmpIn2.data[k];
                   }

                   out[in_index+ii] = tmpOutAdd;
		}
            }
        }

    }
}

