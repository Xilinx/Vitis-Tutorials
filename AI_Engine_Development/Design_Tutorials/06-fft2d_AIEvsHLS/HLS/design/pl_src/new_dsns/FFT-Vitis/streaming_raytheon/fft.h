#ifndef FFT_H
#define FFT_H



#include <math.h>
#include <complex>
#include <ap_fixed.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <hls_fft.h>

using namespace hls;



#define PRAGMA_SUB(x) _Pragma (#x)
#define DO_PRAGMA(x) PRAGMA_SUB(x)

#define MAX_ROWS 128
#define MAX_COLS 256

#if MAX_ROWS > MAX_COLS
	#define FFT_SIZE MAX_ROWS
#else
	#define FFT_SIZE MAX_COLS
#endif

#define NUM_ROWS_LOG2_WIDTH 3 //log2(NUM_ROWS_WIDTH) -- TODO: These comments aren't correct
#define NUM_COLS_LOG2_WIDTH 4 //log2(NUM_COLS_WIDTH)

#define NUM_ROWS_WIDTH  8 //log2(MAX_ROWS)
#define NUM_COLS_WIDTH 16 //log2(MAX_COLS)


typedef std::complex<float> mytype;

// FFT
const int  FFT_DATA_WIDTH   = 32;
const char FFT_NFFT_MAX     = 8;
const int  FFT_PHASE_WIDTH  = 24;
const int  FFT_CONFIG_WIDTH = 24;

// FFT Configuration Struct, see Xilinx Documentation
// Only used for simulation puproses
struct xfft_config : hls::ip_fft::params_t {

//	input_width			unsigned	16						8-34
//	output_width		unsigned	16						input_width to (input_width + max_nfft + 1)
//	status_width		unsigned	8						Depends on FFT configuration
//	config_width		unsigned	16						Depends on FFT configuration
//	max_nfft			unsigned	10						3-16
//	has_nfft			bool		false					True, False
//	channels			unsigned	1						1-12
//	arch_opt			unsigned	pipelined_streaming_io	automatically_select
//															pipelined_streaming_io
//															radix_4_burst_io
//															radix_2_burst_io
//															radix_2_lite_burst_io
//	phase_factor_width	unsigned	16	8-34
//	ordering_opt		unsigned	bit_reversed_order		bit_reversed_order
//															natural_order
//	ovflo				bool		true					false
//															true
//	scaling_opt			unsigned	scaled					scaled
//															unscaled
//															block_floating_point
//	rounding_opt		unsigned	truncation				truncation
//															convergent_rounding
//	mem_data			unsigned	block_ram				block_ram
//															distributed_ram
//	mem_phase_factors	unsigned	block_ram				block_ram
//															distributed_ram
//	mem_reorder			unsigned	block_ram				block_ram
//															distributed_ram
//	stages_block_ram	unsigned	(max_nfft < 10) ? 0 :	0-11
//									(max_nfft - 9)
//	mem_hybrid			bool		false					false
//															true
//	complex_mult_type	unsigned	use_mults_resources		use_luts
//															use_mults_resources
//															use_mults_performance
//	butterfly_type		unsigned	use_luts				use_luts
//															use_xtremedsp_slices


	static const unsigned input_width         = FFT_DATA_WIDTH;
	static const unsigned output_width        = FFT_DATA_WIDTH;
	static const unsigned status_width        = 8;
	static const unsigned config_width        = 24;
	static const unsigned max_nfft            = FFT_NFFT_MAX;
	static const bool     has_nfft            = true;
	static const unsigned arch_opt            = hls::ip_fft::pipelined_streaming_io;
	static const unsigned phase_factor_width  =  FFT_PHASE_WIDTH;
	static const unsigned ordering_opt        = hls::ip_fft::natural_order;
//	static const unsigned scaling_opt         = hls::ip_fft::block_floating_point;
	static const unsigned stages_block_ram    = 0;
	static const unsigned use_mults_resources = hls::ip_fft::use_mults_performance;
	static const unsigned butterfly_type      = hls::ip_fft::use_xtremedsp_slices;
};

//struct config : hls::ip_fft::params_t {
//    static const unsigned phase_factor_width = FFT_PHASE_WIDTH;
//};


typedef hls::ip_fft::config_t<xfft_config> fft_config_t;
typedef hls::ip_fft::status_t<xfft_config> fft_status_t;
//fft_config_t fft_config;
//fft_status_t fft_status;






void dummy_proc_fe(
	unsigned nfft,
    bool direction,
	fft_config_t* config,
    hls::stream< mytype > &in,
	mytype out[FFT_SIZE]);

void dummy_proc_be(
	unsigned nfft,
	fft_status_t* status_in,
    bool* ovflo,
	mytype in[FFT_SIZE],
    hls::stream< mytype > &out);

void fft_top(
	unsigned nfft,
    bool direction,
    hls::stream< mytype > &in,
    hls::stream< mytype > &out,
    bool* ovflo);





////#include "ap_fixed.h"
//typedef float data_in_t;
//typedef float data_out_t;
////#include <complex>
//typedef hls::x_complex<data_in_t> cmpxData;
//typedef hls::x_complex<data_out_t> cmpxDataOut;



//typedef ap_uint<NUM_ROWS_LOG2_WIDTH> chip_row_log2_t;
//typedef ap_uint<NUM_COLS_LOG2_WIDTH> chip_col_log2_t;
//typedef ap_uint<NUM_ROWS_WIDTH> chip_row_t;
//typedef ap_uint<NUM_COLS_WIDTH> chip_col_t;
typedef unsigned chip_row_log2_t;
typedef unsigned chip_col_log2_t;
typedef unsigned chip_row_t;
typedef unsigned chip_col_t;





typedef struct {
	std::complex<float> data;
	ap_uint<1> last;
} complex_axis_t;



//#define PHASE_LUT_DEPTH 16384



void myfft (
	hls::stream<complex_axis_t> &chip_in,
	chip_row_log2_t num_rows_log2,
	chip_col_log2_t num_cols_log2,
	hls::stream<complex_axis_t> &chip_out);

#endif
