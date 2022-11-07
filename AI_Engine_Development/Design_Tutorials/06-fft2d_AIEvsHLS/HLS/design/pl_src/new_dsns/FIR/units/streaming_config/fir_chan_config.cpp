#include "hls_stream.h"
#include "fir_chan_config.h"

struct config1 : hls::ip_fir::params_t {
    static const unsigned total_num_coeff = 9;
    static const double coeff_vec[total_num_coeff];
    static const unsigned input_length = DATA_LENGTH;
    static const unsigned output_length = DATA_LENGTH;
    static const unsigned num_channels = CHAN_NUM;
    static const unsigned config_method = hls::ip_fir::by_channel;
    static const unsigned input_width = INPUT_WIDTH;
    static const unsigned input_fractional_bits = INPUT_FRACTIONAL_BITS;
    static const unsigned output_width = OUTPUT_WIDTH;
    static const unsigned output_fractional_bits = OUTPUT_FRACTIONAL_BITS;
    static const unsigned coeff_width = COEFF_WIDTH;
    static const unsigned coeff_fractional_bits = COEFF_FRACTIONAL_BITS;
    static const unsigned num_coeffs = 3;
    static const unsigned coeff_sets = 3;
    static const unsigned quantization = hls::ip_fir::quantize_only;

};
const double config1::coeff_vec[total_num_coeff] = 
    {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9};


void dummy_fe(s_data_t in[CHAN_NUM][DATA_LENGTH], hls::stream<s_data_t> &out_s, hls::stream<config_t> &config_s)
{
    assert(CHAN_NUM==3);
    static unsigned cnt = 0;
    config_t config[CHAN_NUM];
    switch (cnt)
    {
      case 0:
          config[0] = 0;
          config[1] = 1;
          config[2] = 2;
          break;
      case 1:
          config[0] = 1;
          config[1] = 2;
          config[2] = 0;
          break;
      case 2:
          config[0] = 2;
          config[1] = 0;
          config[2] = 1;
          break;

      default:
          config[0] = 0;
          config[1] = 0;
          config[2] = 0;
    }
    for (int j = 0; j < CHAN_NUM; j++)
        config_s.write(config[j]);
    
    for (int i = 0; i < DATA_LENGTH; i++)
        for (int j = 0; j < CHAN_NUM; j++) 
            out_s.write(in[j][i]);

    cnt++;
}

void dummy_be(hls::stream<m_data_t> &in_s, m_data_t out[CHAN_NUM][DATA_LENGTH])
{
    for (int i = 0; i < DATA_LENGTH; i++)
        for (int j = 0; j < CHAN_NUM; j++)
            out[j][i] = in_s.read();
}


// DUT
void fir_top(s_data_t in[CHAN_NUM][DATA_LENGTH],
             m_data_t out[CHAN_NUM][DATA_LENGTH])
{
#pragma HLS dataflow

    hls::stream<s_data_t> fir_in;
    hls::stream<m_data_t> fir_out;
    hls::stream<config_t> config;

    // Create FIR instance
    static hls::FIR<config1> fir1;
    
    //==================================================
    // Dataflow process
    dummy_fe(in, fir_in, config);
    fir1.run(fir_in, fir_out, config);
    dummy_be(fir_out, out);
    //==================================================
}
