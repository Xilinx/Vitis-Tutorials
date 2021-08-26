#include<iostream>
#include<ctime>
#include<chrono>
#include<vector>
#include<utility>
#include<cstdio>
#include<cstdlib>

#include"sizes.h"
#include "common.h"

using namespace std;
using namespace std::chrono;

void runOnCPU (
    unsigned int*  doc_sizes,
    unsigned int*  input_doc_words,
    unsigned int*  bloom_filter,
    unsigned long* profile_weights,
    unsigned long* profile_score,
    unsigned int   total_num_docs,
    unsigned int   total_size) 
{

    unsigned int size_offset=0;

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

    //unsigned char* inh_flags = (unsigned char*)aligned_alloc(4096, total_size*sizeof(char));
    bool* inh_flags = (bool*)aligned_alloc(4096, total_size*sizeof(bool));


    for(unsigned int doc=0;doc<total_num_docs;doc++) 
    {
        profile_score[doc] = 0.0;
        unsigned int size = doc_sizes[doc];

        for (unsigned i = 0; i < size ; i++)
        { 
            unsigned curr_entry = input_doc_words[size_offset+i];
            unsigned word_id = curr_entry >> 8;
            unsigned hash_pu =  MurmurHash2( &word_id , 3,1);
            unsigned hash_lu =  MurmurHash2( &word_id , 3,5);
            bool doc_end = (word_id==docTag);
            unsigned hash1 = hash_pu&hash_bloom;
            bool inh1 = (!doc_end) && (bloom_filter[ hash1 >> 5 ] & ( 1 << (hash1 & 0x1f)));
            unsigned hash2 = (hash_pu+hash_lu)&hash_bloom;
            bool inh2 = (!doc_end) && (bloom_filter[ hash2 >> 5 ] & ( 1 << (hash2 & 0x1f)));
            
           
            if (inh1 && inh2) {
                inh_flags[size_offset+i]=1;
            } else {
                inh_flags[size_offset+i]=0;
            }
        }
      
        size_offset+=size;
    }


     for(unsigned int doc=0, n=0; doc<total_num_docs;doc++) 
    {
        profile_score[doc] = 0.0;
        unsigned int size = doc_sizes[doc];
        
        for (unsigned i = 0; i < size ; i++,n++)
        { 
            if(inh_flags[n]) 
          {            
            unsigned curr_entry = input_doc_words[n];
            unsigned frequency = curr_entry & 0x00ff;
            unsigned word_id = curr_entry >> 8;      
            profile_score[doc]+= profile_weights[word_id] * (unsigned long)frequency;
          }
        }
    }
   
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> time_span_cpu   = (t2-t1);

    printf(" Executed Software-Only version     |  %10.4f ms\n", 1000*time_span_cpu.count());
}
