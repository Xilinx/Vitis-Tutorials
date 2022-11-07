#include "fft_top.h"
#include "ap_utils.h"


void dummy_proc_fe(
    bool direction,
    hls::stream<config_t> &config_s, 
    cmpxDataIn in[FFT_LENGTH],
    hls::stream<cmpxDataIn> &out_s)
{
    int i; 
    config_t config;
    config.setDir(direction);
    config.setSch(0x2AB);
    config_s.write(config);
    for (i=0; i< FFT_LENGTH; i++) {
    #pragma HLS pipeline II=1 rewind
        out_s.write(in[i]);
    }
}

void dummy_proc_be(
    hls::stream<status_t> &status_in_s, 
    bool* ovflo,
    hls::stream<cmpxDataOut> &in_s,
    cmpxDataOut out[FFT_LENGTH])
{
    int i; 
    for (i=0; i< FFT_LENGTH; i++) {
    #pragma HLS pipeline II=1 rewind
        out[i] = in_s.read();
    }
    status_t status_in = status_in_s.read();
    *ovflo = status_in.getOvflo() & 0x1;
}


// DUT
void fft_top(
    bool direction,
    complex<data_in_t> in[FFT_LENGTH],
    complex<data_out_t> out[FFT_LENGTH],
    bool* ovflo)
{
#pragma HLS interface ap_hs port=direction
#pragma HLS interface ap_fifo depth=1 port=ovflo
#pragma HLS interface ap_fifo depth=FFT_LENGTH port=in,out
#pragma HLS data_pack variable=in
#pragma HLS data_pack variable=out
#pragma HLS dataflow
    hls::stream<complex<data_in_t>> xn;
    hls::stream<complex<data_out_t>> xk;
    hls::stream<config_t> fft_config;
    hls::stream<status_t> fft_status;
   
    dummy_proc_fe(direction, fft_config, in, xn);
    // FFT IP
    hls::fft<config1>(xn, xk, fft_status, fft_config);
    dummy_proc_be(fft_status, ovflo, xk, out);
}
