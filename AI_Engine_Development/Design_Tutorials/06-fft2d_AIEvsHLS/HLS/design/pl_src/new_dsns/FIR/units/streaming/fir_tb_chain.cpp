#include "fir_chain.h"
#include <iostream>
#define N 3

int main()
{
    data_t in[FIR1_LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    data_out_t out[FIR2_LENGTH];
    int err=0; 
    data_out_t ref[FIR2_LENGTH * 3] = {
        36,60,40,185,-120,1918,3844,5465,8132,7644,
        28792,74096,114397,156731,200986,211382,272505,302539,355754,408064,
        138432,24656,127957,136800,207719,219040,230361,301280,310123,413424
    };
    for (int j = 0; j < N; ++j) {
        fir_top(in, out); 
        for(unsigned i = 0; i < FIR2_LENGTH; ++i)
        {
            if(out[i] != ref[i + FIR2_LENGTH * j])
            {
            std::cout << "out[" << i << "] in " << j << " is " << out[i] << " -- expected value is " <<  ref[i+FIR2_LENGTH*j] << std::endl;
                err++;
            }
        }
    }
    
    if (err > 0 )
        printf ("FAIL!!!!\n");
    else
        printf ("PASS!!!!\n");
    return err;
}
