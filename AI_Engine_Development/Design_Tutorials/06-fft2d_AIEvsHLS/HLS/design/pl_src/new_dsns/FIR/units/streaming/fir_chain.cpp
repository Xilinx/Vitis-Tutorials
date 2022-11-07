#include "fir_chain.h"


struct config1 : hls::ip_fir::params_t {
    static const unsigned output_width = DATA_WIDTH + 8;
    static const double coeff_vec[total_num_coeff];
};
const double config1::coeff_vec[total_num_coeff] = 
    {6,0,-4,-3,5,6,-6,-13,7,44,64,44,7,-13,-6,6,5,-3,-4,0,6};

struct config2 : hls::ip_fir::params_t {
    static const unsigned filter_type = hls::ip_fir::decimation;
    static const unsigned decim_rate = 2; 
    static const unsigned input_width = DATA_WIDTH + 8;
    static const unsigned output_width = OUTPUT_WIDTH;
    static const double coeff_vec[total_num_coeff];
    static const unsigned output_length = FIR2_LENGTH;
    static const unsigned coeff_structure = hls::ip_fir::non_symmetric;
};

const double config2::coeff_vec[total_num_coeff] = 
    {6,0,-4,-3,5,6,-6,-13,7,44,64,44,7,-13,-6,6,5,-3,-4,0,6};

template<typename data_t, int LENGTH>
void dummy_process1(data_t in[LENGTH], hls::stream<data_t> &out)
{
    for(unsigned i = 0; i < LENGTH; ++i)
        out.write(in[i]);
}

template<typename data_t, int LENGTH>
void dummy_process2(hls::stream<data_t> &in, data_t out[LENGTH])
{
    for(unsigned i = 0; i < LENGTH; ++i)
        out[i] = in.read();
}

// DUT
void fir_top(data_t in[FIR1_LENGTH],
             data_out_t out[FIR2_LENGTH])
{
#pragma HLS interface ap_fifo depth=FIR1_LENGTH port=in
#pragma HLS interface ap_fifo depth=FIR2_LENGTH port=out
#pragma HLS stream variable=in
#pragma HLS stream variable=out
#pragma HLS dataflow

    hls::stream<data_t> fir1_in;
    hls::stream<data_intern_t> fir1_out;
    hls::stream<data_out_t> fir2_out;

    // Create FIR instance
    static hls::FIR<config1> fir1;
    static hls::FIR<config2> fir2;
 
    //==================================================
    // Dataflow process
    dummy_process1<data_t, FIR1_LENGTH>(in, fir1_in);
    fir1.run(fir1_in, fir1_out);
    fir2.run(fir1_out, fir2_out);
    dummy_process2<data_out_t, FIR2_LENGTH>(fir2_out, out);
    //==================================================
}
