
extern "C" 
{

void runOnfpga (unsigned int total_doc_size,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* h_profileScore,unsigned int out_doc_id); 
}
