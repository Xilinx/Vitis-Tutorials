
#include<chrono>
#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<vector>
#include<utility>
#include<random>
#include"sizes.h"
#include"xcl2.hpp"
#include"common.h"
using namespace std;
using namespace std::chrono;


extern unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );
//extern void run(unsigned int* starting_doc_id,unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* fpga_profileScore,unsigned int total_num_docs,unsigned int size) ;

// Host Buffers

vector<unsigned int,aligned_allocator<unsigned int>> input_doc_words;
vector<unsigned long,aligned_allocator<unsigned long>> profile_weights;
vector<unsigned int,aligned_allocator<unsigned int>> bloom_filter;
vector<unsigned int,aligned_allocator<unsigned int>> starting_doc_id;
vector<unsigned long,aligned_allocator<unsigned long>> fpga_profileScore;
vector<unsigned int,aligned_allocator<unsigned int>> doc_sizes;
vector<unsigned long,aligned_allocator<unsigned long>> cpu_profileScore;

default_random_engine generator;
normal_distribution<double> distribution(3500,500);

unsigned int total_num_docs;
unsigned size=0;


unsigned doc_len()
{
   unsigned int len = distribution(generator);
   if (len < 100) { len = 100; }
   if (len > 4000 ) { len = 3500 ; }
   return len;
}


 void setupData()

{
   starting_doc_id.reserve( total_num_docs );
   fpga_profileScore.reserve( total_num_docs );
   cpu_profileScore.reserve(total_num_docs);

 //  h_docInfo.reserve( total_num_docs );

   doc_sizes.reserve( total_num_docs );
   unsigned block_size = 1024;
   for (unsigned i=0; i<total_num_docs; i++) {
      unsigned unpadded_size = doc_len();
      unsigned int padded_size = unpadded_size&(~(block_size-1));
      if(unpadded_size & (block_size-1)) padded_size+=block_size;
      starting_doc_id[i] = size;
      size+=padded_size;
      doc_sizes[i] = padded_size;
   }
    
   bloom_filter.reserve( (1L << bloom_size) );
   input_doc_words.reserve( size );

   std::cout << "Creating Documents total_terms = "<< size  << endl;

   for (unsigned i=0; i<size; i++) {
      input_doc_words[i] = docTag;
   }
   for (unsigned doci=0; doci < total_num_docs; doci++)
   {
      unsigned start_dimm1 = starting_doc_id[doci];
      unsigned size_1 = doc_sizes[doci];
      unsigned term;
      unsigned freq;

      for (unsigned i = 0; i < size_1; i++)
      {
         term = (rand()%((1L << 24)-1));
         freq = (rand()%254)+1;
         input_doc_words[start_dimm1 + i] = (term << 8) | freq;
      }
   }

   profile_weights.reserve( (1L << 24) );
   for (unsigned i=0; i<(1L << bloom_size); i++) {
      bloom_filter[i] = 0x0;
   }
   std::cout << "Creating Profile" << endl;
   for (unsigned i=0; i<(1L << 24); i++) {
      profile_weights[i] = 0;
   }

   for (unsigned i=0; i<16384; i++) {
      unsigned entry = (rand()%(1<<24));	

      profile_weights[entry] = 10;
      unsigned hash_pu = MurmurHash2(&entry,3,1);
      unsigned hash_lu = MurmurHash2(&entry,3,5);

      unsigned hash1 = hash_pu&hash_bloom;  //this gives me the top 16 bits of the 24bit word id
      unsigned hash2 = (hash_pu+hash_lu)&hash_bloom;
     
      bloom_filter[ hash1 >> 5 ] |= 1 << (hash1 & 0x1f);
      bloom_filter[ hash2 >> 5 ] |= 1 << (hash2 & 0x1f);
   }

}



int main(int argc, char** argv)
   
{

   if(argc==3){
    total_num_docs=atoi(argv[2]);
   } else {
   cout << "Incorrect number of arguments"<<endl;
   return 0;
   } 
    
   std::cout << "Initializing data"<< endl;
  
   setupData();
   chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

   runOnCPU(doc_sizes.data(),input_doc_words.data(),bloom_filter.data(),profile_weights.data(),cpu_profileScore.data(),total_num_docs) ;

   chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
   chrono::duration<double> time_span_cpu  = chrono::duration_cast<duration<double>>(t2-t1);

   return 0;
}

