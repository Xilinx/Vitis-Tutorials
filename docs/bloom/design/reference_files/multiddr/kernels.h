
extern "C" 
{

void runOnfpga (unsigned int* doc_sizes,unsigned int* input_doc_words,unsigned int* bloom_filter,unsigned long* profile_weights,unsigned long* h_profilescore,unsigned int total_num_docs,unsigned int out_doc_id,unsigned int total_size); 
}
