
#include<chrono>
#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<vector>
#include<utility>
#include<random>
#include"xcl2.hpp"
#include"sizes.h"
#include"common.h"

//#define DEBUGX 
using namespace std;
using namespace std::chrono;


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
unsigned block_size;

unsigned doc_len()
{
    unsigned int len = distribution(generator);
    if (len < 100) { len = 100; }
//    if(len > 4000) { len = 3500;}
    return len;
}


void setupData()
{
    starting_doc_id.reserve( total_num_docs );
    fpga_profileScore.reserve( total_num_docs );
    cpu_profileScore.reserve(total_num_docs);

    //  h_docInfo.reserve( total_num_docs );

    doc_sizes.reserve( total_num_docs );
    unsigned unpadded_size=0;

    for (unsigned i=0; i<total_num_docs; i++) {
        unsigned len_doc = doc_len();
        starting_doc_id[i] = unpadded_size;
        unpadded_size+=len_doc;
        doc_sizes[i] = len_doc;
   #ifdef DEBUGX 
    	printf("total_num_docs = %d, iter = %d, len_doc = %d, unpadded_size = %d doc_sizes[i] = %d\n",  \
		total_num_docs, i, len_doc, starting_doc_id[i], doc_sizes[i]);
   #endif
    }
		
		
   #ifdef DEBUGX 
    printf("Block Size %x\n", block_size);
    printf("~(Block Size-1) %x\n", ~(block_size-1));
    printf("Unpadded Size %x\n", (unpadded_size));
   #endif
    size = unpadded_size&(~(block_size-1));
   #ifdef DEBUGX 
    printf("Size before unpadded check %x\n", size);
   #endif
    if(unpadded_size & (block_size-1)) size+=block_size;
   #ifdef DEBUGX 
    printf("Size after unpadded check %x\n", size);
   #endif
    bloom_filter.reserve( (1L << bloom_size) );
    input_doc_words.reserve( size );

    // double mbytes = size*sizeof(int)/(1000000.0);

    printf("Creating documents - total size : %.3f MBytes (%d words)\n", size*sizeof(int)/1000000.0, size);
    // std::cout << "Creating documents of total size = "<< size  << " words" << endl;

   #ifdef DEBUGX 
    printf ("Size with docTag %x\n", size);
   #endif
    for (unsigned i=0; i<size; i++) {
        input_doc_words[i] = docTag;
    }
    for (unsigned doci=0; doci < total_num_docs; doci++)
    {
        unsigned start_dimm1 = starting_doc_id[doci];
   #ifdef DEBUGX 
        printf("Start_dimm1 starting_doc_id[doci] = %u\n",  start_dimm1);
   #endif
        unsigned size_1 = doc_sizes[doci];
        unsigned term;
        unsigned freq;

        //printf ("size_1 %d\n", size_1);
        for (unsigned i = 0; i < size_1; i++)
        {
            term = (rand()%((1L << 24)-1));   // 24 Bit
            freq = (rand()%254)+1;            //  8 Bit
            //printf("TERM = %x, Freq=%x\n", term , freq);
            input_doc_words[start_dimm1 + i] = (term << 8) | freq;
        }
    }

    profile_weights.reserve( (1L << 24) );
    for (unsigned i=0; i<(1L << bloom_size); i++) {
        bloom_filter[i] = 0x0;
    }
    std::cout << "Creating profile weights" << endl;
    std::cout << endl;
 
    for (unsigned i=0; i<(1L << 24); i++) {
        profile_weights[i] = 0;
    }

    for (unsigned i=0; i<16384; i++) {    // Bloom filter size 1L<<14 =16384
        unsigned entry = (rand()%(1<<24));	

        profile_weights[entry] = 10;
        unsigned hash_pu = MurmurHash2(&entry,3,1);
        unsigned hash_lu = MurmurHash2(&entry,3,5);

        unsigned hash1 = hash_pu&hash_bloom; 
        unsigned hash2 = (hash_pu+hash_lu)&hash_bloom;

        bloom_filter[ hash1 >> 5 ] |= 1 << (hash1 & 0x1f);
        bloom_filter[ hash2 >> 5 ] |= 1 << (hash2 & 0x1f);
    }

}

int main(int argc, char** argv)
{
    int num_iter;

    switch(argc) {
      case 2: 
         total_num_docs=atoi(argv[1]);
         num_iter = 2;
         break;
      case 3:
         total_num_docs=atoi(argv[1]);
         num_iter = atoi(argv[2]);
         break;      
      default:
         cout << "Incorrect number of arguments"<<endl;
         return 0;
    } 

    std::cout << "Initializing data"<< endl;
    block_size = num_iter*64;
    setupData();

    //printf ("Sending data on FPGA  Doc_sizes = %lu\n ", doc_sizes.size());
    //std::cout << "Sending data on FPGA  Doc_sizes " <<  doc_sizes.size() << endl;
    //std::cout << "Input Doc Words " <<  input_doc_words.size() << endl;
    runOnFPGA(
        doc_sizes.data(),
        input_doc_words.data(),
        bloom_filter.data(),
        profile_weights.data(),
        fpga_profileScore.data(),
        total_num_docs,
        size,
        num_iter) ;
  
     runOnCPU(
        doc_sizes.data(),
        input_doc_words.data(),
        bloom_filter.data(),
        profile_weights.data(),
        cpu_profileScore.data(),
        total_num_docs,
        size) ;
  
    printf("--------------------------------------------------------------------\n");
    
    for (unsigned doci = 0; doci < total_num_docs; doci++)
    {
        if (cpu_profileScore[doci] != fpga_profileScore[doci]) {
            std::cout << " Verification: FAILED "<< endl  << " : doc[" << doci << "]" << " score: CPU = " << cpu_profileScore[doci]<< ", FPGA = "<< fpga_profileScore[doci] <<  endl;
            return 0;
        }
    }
    cout << " Verification: PASS" << endl;
    cout << endl;

    return 0;
}

