#pragma once

unsigned int MurmurHash2(const void* key ,int len,unsigned int seed);

void runOnCPU (
    unsigned int*  doc_sizes,
    unsigned int*  input_doc_words,
    unsigned int*  bloom_filter,
    unsigned long* profile_weights,
    unsigned long* profile_score,
    unsigned int   total_num_docs,
    unsigned int   total_size);

void runOnFPGA(	
	unsigned int*  doc_sizes,
	unsigned int*  input_doc_words,
	unsigned int*  bloom_filter,
	unsigned long* profile_weights,
	unsigned long* profile_score,
	unsigned int   total_num_docs, 
	unsigned int   total_doc_size,
	int            num_iter);
