typedef unsigned int uint;
typedef unsigned long ulong;

extern "C" {
void runOnCPU(unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* cpu_profileScore,unsigned int total_num_docs) ;
}
void run (uint* h_startingDocID_dimm1,uint* h_docSizes_dimm1,uint* h_docWordFrequencies_dimm1,uint* h_isWordInProfileHash,ulong* h_profileWeights,ulong* h_profileScore,unsigned int total_num_docs, unsigned int total_doc_size_1);
