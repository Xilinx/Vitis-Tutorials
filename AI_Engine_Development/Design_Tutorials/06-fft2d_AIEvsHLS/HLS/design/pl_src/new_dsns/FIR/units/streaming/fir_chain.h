#include "ap_fixed.h"
#include "hls_fir.h"


const unsigned DATA_WIDTH = 16;
const unsigned OUTPUT_WIDTH = 32;
const unsigned DATA_FRACTIONAL_BITS = 0;
const unsigned COEFF_WIDTH = 16;
const unsigned COEFF_FRACTIONAL_BITS = 0;
const unsigned COEFF_NUM = 21;
const unsigned COEFF_SETS = 1;
const unsigned FIR1_LENGTH = 21;
const unsigned FIR2_LENGTH = 10;
const unsigned CHAN_NUM = 1;

typedef ap_fixed<DATA_WIDTH, DATA_WIDTH - DATA_FRACTIONAL_BITS> data_t;
typedef ap_fixed<DATA_WIDTH+8, DATA_WIDTH+8 - DATA_FRACTIONAL_BITS> data_intern_t;
typedef ap_fixed<OUTPUT_WIDTH, OUTPUT_WIDTH - DATA_FRACTIONAL_BITS> data_out_t;

void fir_top(data_t in[FIR1_LENGTH], data_out_t out[FIR2_LENGTH]);
