#include "fft.h"

void dummy_proc_fe(
	unsigned nfft,
    bool direction,
	hls::stream<fft_config_t> &config_s,
    hls::stream< complex_axis_t > &in,
    hls::stream< complex<float> > &out)
{
	fft_config_t config;
    int bound = 1<<nfft;

    config.setNfft(nfft);
    config.setDir(direction);
    config_s.write(config);

    for (int i = 0; i < bound; i++) {
    #pragma HLS pipeline II=1 rewind
        out.write(in.read().data);
    }
}

void dummy_proc_be(
	unsigned nfft,
	hls::stream<fft_status_t> &status_in_s,
	hls::stream<complex<float> > &in,
    hls::stream<complex_axis_t > &out)
{
    int bound = 1<<nfft;
    for (int i = 0; i < bound; i++) {
    #pragma HLS pipeline II=1 rewind
        complex_axis_t tmp;
        tmp.data = in.read();
        tmp.last = (i == (1<<nfft)-1);
        out.write(tmp);
    }
	fft_status_t status_in = status_in_s.read();
    bool ovflo = status_in.getOvflo() & 0x1;
}


//==============================================================================
// The top-level function
//==============================================================================
void myfft (
	hls::stream< complex_axis_t > &chip_in,
	chip_row_log2_t num_rows_log2,
	chip_col_log2_t num_cols_log2,
	hls::stream< complex_axis_t > &chip_out)
{
    bool direction = true;
	for (int i = 0; i < (1<<num_cols_log2); i++) {
		#pragma HLS dataflow

        hls::stream<complex<float>> xn;
        hls::stream<complex<float>> xk;
        hls::stream<fft_config_t> fft_config;
        hls::stream<fft_status_t> fft_status;

        dummy_proc_fe(num_rows_log2, direction, fft_config, chip_in, xn);
        hls::fft<xfft_config>(xn, xk, fft_status, fft_config);
        dummy_proc_be(num_rows_log2, fft_status, xk, chip_out);
	}
}
