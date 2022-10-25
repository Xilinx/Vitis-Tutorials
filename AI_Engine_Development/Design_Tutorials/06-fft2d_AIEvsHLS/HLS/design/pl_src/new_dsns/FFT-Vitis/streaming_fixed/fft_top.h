#include "ap_fixed.h"
#include "hls_fft.h"

/***************************************************************    
 configurable    
    char FFT_INPUT_WIDTH;    		//@- Input data width: 8-34 bits (32 if C_USE_FLT_PT = 1)
    int  FFT_LENGTH;         		//@- Transform length
    ip_fft::arch FFT_ARCH;           	//@- Architecture: 1=radix-4, 2=radix-2, 3=pipelined, 4=radix-2 Lite
    char FFT_TWIDDLE_WIDTH;  		//@- Twiddle factor width: 8-34 bits (24-25 if C_USE_FLT_PT = 1)
    ip_fft::scaling FFT_SCALING;
    char FFT_OUTPUT_WIDTH;    		//@- Output data width: 8-34 bits (32 if C_USE_FLT_PT = 1)
    ip_fft::rounding FFT_ROUNDING;
    ip_fft::ordering FFT_OUTPUT_ORDER;	//@- Output ordering: 0=natural order, 1=reverse order
    bool FFT_OVFLO;
    char FFT_NFFT_MAX;       //@- log2(maximum transform length): 3-16
    bool FFT_HAS_NFFT;       //@- Run-time configurable transform length: 0=no, 1=yes
    bool FFT_OUTPUT_ORDER;   //@- Output ordering: 0=natural order, 1=reverse order

not configurable yet
****************************************************************/
// configurable params
const char FFT_INPUT_WIDTH                     = 16;
const char FFT_OUTPUT_WIDTH                    = FFT_INPUT_WIDTH;
const char FFT_STATUS_WIDTH                    = 8;
const char FFT_CONFIG_WIDTH                    = 16;
const char FFT_CHANNELS                        = 1;
const int  FFT_LENGTH                          = 1024; 

const char FFT_NFFT_MAX                        = 10; 
const bool FFT_HAS_NFFT                        = 0;
const hls::ip_fft::arch FFT_ARCH               = hls::ip_fft::pipelined_streaming_io;
const char FFT_TWIDDLE_WIDTH                   = 16;
const hls::ip_fft::ordering FFT_OUTPUT_ORDER   = hls::ip_fft::natural_order;
const bool FFT_OVFLO                           = 1;
const bool FFT_CYCLIC_PREFIX_INSERTION         = 0;
const bool FFT_XK_INDEX                        = 0;
const hls::ip_fft::scaling FFT_SCALING         = hls::ip_fft::scaled;
const hls::ip_fft::rounding FFT_ROUNDING       = hls::ip_fft::truncation;
// not configurable yet
const hls::ip_fft::mem FFT_MEM_DATA            = hls::ip_fft::block_ram;
const hls::ip_fft::mem FFT_MEM_PHASE_FACTORS   = hls::ip_fft::block_ram;
const hls::ip_fft::mem FFT_MEM_REORDER         = hls::ip_fft::block_ram;
const char FFT_STAGES_BLOCK_RAM                = 4;
const bool FFT_MEM_OPTIONS_HYBRID              = 0;
const hls::ip_fft::opt FFT_COMPLEX_MULT_TYPE   = hls::ip_fft::use_luts;
const hls::ip_fft::opt FFT_BUTTERLY_TYPE       = hls::ip_fft::use_luts;


#include <complex>
using namespace std;

struct config1 : hls::ip_fft::params_t {
    static const unsigned ordering_opt = hls::ip_fft::natural_order;
    static const unsigned config_width = FFT_CONFIG_WIDTH;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;

typedef ap_fixed<FFT_INPUT_WIDTH,1> data_in_t;
typedef ap_fixed<FFT_OUTPUT_WIDTH,FFT_OUTPUT_WIDTH-FFT_INPUT_WIDTH+1> data_out_t;
typedef std::complex<data_in_t> cmpxDataIn;
typedef std::complex<data_out_t> cmpxDataOut;

void dummy_proc_fe(
    bool direction,
    config_t* config, 
    cmpxDataIn in[FFT_LENGTH], 
    cmpxDataIn out[FFT_LENGTH]);

void dummy_proc_be(
    status_t* status_in, 
    bool* ovflo,
    cmpxDataOut in[FFT_LENGTH], 
    cmpxDataOut out[FFT_LENGTH]);

void fft_top(
    bool direction,
    cmpxDataIn in[FFT_LENGTH],
    cmpxDataOut out[FFT_LENGTH],
    bool* ovflo);
