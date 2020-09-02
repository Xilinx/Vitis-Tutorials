#include <iostream>
#include <ctime>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hls_stream_utils.h"
#include "sizes.h"

#define TOTAL_SIZE 3500000

//TRIPCOUNT identifiers
const unsigned int t_size = TOTAL_SIZE;
const unsigned int pf = PARALLELISATION;


#ifndef PARALLELISATION
#define PARALLELISATION 8
#endif

typedef ap_uint<sizeof(int )*8*PARALLELISATION> parallel_words_t; 
typedef ap_uint<sizeof(char)*8*PARALLELISATION> parallel_flags_t; 

const unsigned int bloom_filter_size = 1<<bloom_size;

unsigned int MurmurHash2(unsigned int key, int len, unsigned int seed)
{
  const unsigned char* data = (const unsigned char *)&key;
  const unsigned int m = 0x5bd1e995;
  unsigned int h = seed ^ len;
  switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
            h *= m;
  };
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
} 

void compute_hash_flags (
        hls::stream<parallel_flags_t>& flag_stream,
        hls::stream<parallel_words_t>& word_stream,
        unsigned int                   bloom_filter_local[PARALLELISATION][bloom_filter_size],
        unsigned int                   total_size) 
{
  compute_flags: for(int i=0; i<total_size/PARALLELISATION; i++)
  {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=t_size/pf
    parallel_words_t parallel_entries = word_stream.read();
    parallel_flags_t inh_flags = 0;

    compute_hash_flags_loop: for (unsigned int j=0; j<PARALLELISATION; j++)
    {
#pragma HLS UNROLL

      unsigned int curr_entry = parallel_entries(31+j*32, j*32);
      unsigned int frequency = curr_entry & 0x00ff;
      unsigned int word_id = curr_entry >> 8;
      unsigned hash_pu = MurmurHash2(word_id, 3, 1);
      unsigned hash_lu = MurmurHash2(word_id, 3, 5);
      bool doc_end= (word_id==docTag); 
      unsigned hash1 = hash_pu&hash_bloom; 
      bool inh1 = (!doc_end) && (bloom_filter_local[j][ hash1 >> 5 ] & ( 1 << (hash1 & 0x1f)));
      unsigned hash2=(hash_pu+hash_lu)&hash_bloom;
      bool inh2 = (!doc_end) && (bloom_filter_local[j][ hash2 >> 5 ] & ( 1 << (hash2 & 0x1f)));

      inh_flags(7+j*8, j*8) = (inh1 && inh2) ? 1 : 0;
    }

    flag_stream.write(inh_flags); 
  } 
}

void compute_hash_flags_dataflow(
        ap_uint<512>*   output_flags,
        ap_uint<512>*   input_words,
        unsigned int    bloom_filter[PARALLELISATION][bloom_filter_size],
        unsigned int    total_size)
{
#pragma HLS DATAFLOW

    hls::stream<ap_uint<512> >    data_from_gmem;
    hls::stream<parallel_words_t> word_stream;
    hls::stream<parallel_flags_t> flag_stream;
    hls::stream<ap_uint<512> >    data_to_gmem;

  // Burst read 512-bit values from global memory over AXI interface
  hls_stream::buffer(data_from_gmem, input_words, total_size/(512/32));

  // Form a stream of parallel words from stream of 512-bit values
  // Going from Wi=512 to Wo= 256 
  hls_stream::resize(word_stream, data_from_gmem, total_size/(512/32));

  // Process stream of parallel word : word_stream is of 2k (32*64)
  compute_hash_flags(flag_stream, word_stream, bloom_filter, total_size);

  // Form a stream of 512-bit values from stream of parallel flags
  // Going from Wi=64 to Wo=512
  hls_stream::resize(data_to_gmem, flag_stream, total_size/(512/8));

  // Burst write 512-bit values to global memory over AXI interface
  hls_stream::buffer(output_flags, data_to_gmem, total_size/(512/8));
}

extern "C" 
{

  void runOnfpga (
          ap_uint<512>*  output_flags,
          ap_uint<512>*  input_words,
          unsigned int*  bloom_filter,
          unsigned int   total_size,
          bool           load_filter)
  {
  #pragma HLS INTERFACE ap_ctrl_chain port=return            bundle=control
  #pragma HLS INTERFACE m_axi         port=output_flags      bundle=maxiport0   offset=slave 
  #pragma HLS INTERFACE m_axi         port=input_words       bundle=maxiport0   offset=slave 
  #pragma HLS INTERFACE m_axi         port=bloom_filter      bundle=maxiport1   offset=slave 

    static unsigned int bloom_filter_local[PARALLELISATION][bloom_filter_size];
  #pragma HLS ARRAY_PARTITION variable=bloom_filter_local complete dim=1
printf("From runOnfpga : Total_size = %d\n", total_size);

    if(load_filter==true) 
    {
      read_bloom_filter: for(int index=0; index<bloom_filter_size; index++) {
  #pragma HLS PIPELINE II=1
        unsigned int tmp = bloom_filter[index];
        for (int j=0; j<PARALLELISATION; j++) {
          bloom_filter_local[j][index] = tmp;
        }
      }
    }

    compute_hash_flags_dataflow(
      output_flags,
      input_words,
      bloom_filter_local,
      total_size);
  }
}
