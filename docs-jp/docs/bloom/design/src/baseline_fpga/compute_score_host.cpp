#include<iostream>
#include<ctime>
#include<chrono>
#include<utility>
#include<cstdio>
#include<cstdlib>
#include"sizes.h"
#include"kernels.h"

using namespace std;
using namespace std::chrono;

typedef unsigned int uint;
typedef unsigned long ulong;


extern unsigned int MurmurHash2(const void* key ,int len,unsigned int seed);

extern "C"

{
void runOnCPU (unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* cpu_profileScore,unsigned int total_num_docs) {



      unsigned int size_offset=0;
      unsigned int false_access=0;
 
   chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
      for(unsigned int doc=0;doc<total_num_docs;doc++) {
         
         cpu_profileScore[doc] = 0.0;
         unsigned int size = doc_sizes[doc];
      for (unsigned i = 0; i < size ; i++)
      { 
         unsigned curr_entry = input_doc_words[size_offset+i];
         unsigned frequency = curr_entry & 0x00ff;
         unsigned word_id = curr_entry >> 8;
         unsigned hash_pu =  MurmurHash2( &word_id , 3,1);
         unsigned hash_lu =  MurmurHash2( &word_id , 3,5);
         bool doc_end = (word_id==docTag);
          unsigned hash1 = hash_pu&hash_bloom;
         bool inh1 = (!doc_end) && (bloom_filter[ hash1 >> 5 ] & ( 1 << (hash1 & 0x1f)));
         unsigned hash2 = (hash_pu+hash_lu)&hash_bloom;
         bool inh2 = (!doc_end) && (bloom_filter[ hash2 >> 5 ] & ( 1 << (hash2 & 0x1f)));

         if (inh1 && inh2)
         {  
            if(profile_weights[word_id]==0) false_access++;
            cpu_profileScore[doc] += profile_weights[word_id] * (unsigned long)frequency;
         }
      }
      size_offset+=size;
    }
    
   chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
   chrono::duration<double> time_span_cpu  = chrono::duration_cast<duration<double>>(t2-t1);
   cout << " False positive rate is " << ((double)false_access/size_offset)*100 << endl;
   cout << " Execution Time of CPU is " << time_span_cpu.count() << endl;
}
}
