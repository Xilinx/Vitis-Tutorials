#include "ap_fixed.h"
#include "hls_fir.h"


const unsigned INPUT_WIDTH = 8;
const unsigned INPUT_FRACTIONAL_BITS = 2;
const unsigned OUTPUT_WIDTH = 16;
const unsigned OUTPUT_FRACTIONAL_BITS = 4;
const unsigned COEFF_WIDTH = 8;
const unsigned COEFF_FRACTIONAL_BITS = 2;
const unsigned DATA_LENGTH = 12;
const unsigned CHAN_NUM = 3;
const unsigned FIR_LENGTH = DATA_LENGTH * CHAN_NUM;
const unsigned CONFIG_WIDTH = 8;
        

typedef ap_fixed<INPUT_WIDTH, INPUT_WIDTH - INPUT_FRACTIONAL_BITS> s_data_t;
typedef ap_fixed<OUTPUT_WIDTH, OUTPUT_WIDTH - OUTPUT_FRACTIONAL_BITS> m_data_t;
typedef ap_uint<CONFIG_WIDTH> config_t;

void fir_top(s_data_t in[CHAN_NUM][DATA_LENGTH], m_data_t out[CHAN_NUM][DATA_LENGTH]);
